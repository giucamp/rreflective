/***********************************************************************************

Copyright 2011-2012 Giuseppe Campana - giu.campana@gmail.com
All rights reserved

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY GIUSEPPE CAMPANA ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GIUSEPPE CAMPANA OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************/

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{
	// Enum::new_enum
	Enum * new_enum( const StaticConstString & parent_namespace_path, const SymbolName & name )
	{
		void * allocation = reflective_externals::mem_lifo_alloc(
			alignment_of( Enum ), sizeof( Enum ) );

		Enum * enum_object = new( allocation ) Enum( parent_namespace_path, name );
		return enum_object;
	}

	// Enum::constructor
	Enum::Enum( const StaticConstString & parent_namespace_path, const SymbolName & name )
		: Type( parent_namespace_path, name, sizeof( int ), alignment_of( int ) )
	{
		set_type_id( eTypeEnum );

		set_life_functions( LifeFunctions( 
			PrimitiveType::constructor_func, 
			PrimitiveType::destructor_func, 
			PrimitiveType::copier, 
			PrimitiveType::mover ) );
		set_string_functions( to_string, from_string );
	}


	// Enum::from_string
	bool Enum::from_string( FromStringBuffer & source_buffer, const Type & type,
		void * object, StringOutputStream & error_buffer )
	{
		REFLECTIVE_ASSERT( type_of( type ).can_cast_to( get_type<Enum>() ) );

		const Enum & enum_type = static_cast<const Enum &>( type );

		const Enum::MemberList & members = enum_type.members();
		//const size_t member_count = members.count();

		int value = 0;

		bool missing_pipe = false;
		const char * start_of_token = nullptr;
		size_t token_length = 0;
		size_t recognized_members = 0;

		for(;;)
		{
			// remove leading spaces
			while( source_buffer.accept( ' ' ) );

			if( source_buffer.remaining_length() == 0 )
				break;

			if( missing_pipe )
			{
				error_buffer.append_literal( "missing a pipe ( | ) after " );
				error_buffer.append( start_of_token, token_length );
				error_buffer.append_literal( " the enum is " );
				reflective::to_string( error_buffer, enum_type.name() );
			}


			// read token
			
			start_of_token = source_buffer.chars();

			while( source_buffer.remaining_length() > 0 && isalnum( *source_buffer.chars() ) )
			{
				source_buffer.consume_char();
			};

			token_length = source_buffer.chars() - start_of_token;

			const SymbolName token( start_of_token, token_length );

			const Enum::Member * member = members.find( token );

			if( member == nullptr )
			{				
				// member not found
				error_buffer.append( start_of_token, token_length );
				error_buffer.append_literal( " is not a member of the enum " );
				reflective::to_string( error_buffer, enum_type.name() );
				return false;
			}

			recognized_members++;

			value |= member->value();

			missing_pipe = !source_buffer.accept_literal( "|", FromStringBuffer::eIgnoreLeadingSpaces );
		}

		if( recognized_members == 0 )
		{
			error_buffer.append_literal( "no member specified for of the enum " );
			reflective::to_string( error_buffer, enum_type.name() );
			return false;
		}

		*static_cast<int*>( object ) = value;

		return true;
	}

	// Enum::to_string
	void Enum::to_string( StringOutputStream & dest_buffer, const Type & type, 
		const void * object )
	{
		REFLECTIVE_ASSERT( safe_type_of( type ).can_cast_to( safe_get_type<Enum>() ) );

		// value to 
		const int value = *static_cast<const int*>( object );
		const Enum & enum_type = static_cast<const Enum &>( type );

		#if REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES

			// members \ member_count			
			const Enum::MemberList & members = enum_type.members();
			const size_t member_count = members.count();

			// loop members in reverse order - enum members are always sorted in non-decreasing order
			int residue_value = value;
			size_t member_index = member_count - 1;
			bool some_member_out = false;
			if( member_count > 0 ) do {

				Enum::Member curr_member = members[ member_index ];
				const int curr_member_val = curr_member.value();
				if( curr_member_val == 0 )
					break;
					
				/* the residue value is grater or equal to the value of the current member,
					so the latter is included in the string. */
				if( residue_value >= curr_member_val )
				{
					residue_value -= curr_member_val;

					// append the separator
					if( some_member_out )
						dest_buffer.append_literal( " | " );
					some_member_out = true;

					// append the name to the string
					const SymbolName & curr_member_name = curr_member.name();
					curr_member_name.to_string( dest_buffer );
				}

			} while( member_index-- > 0 );

			if( !some_member_out )
			{
				if( member_count > 0 )
				{
					Enum::Member first_member = members[ 0 ];
					const int first_member_val = first_member.value();
					if( first_member_val == 0 )
					{
						const SymbolName & first_member_name = first_member.name();
						first_member_name.to_string( dest_buffer );
						return;
					}
				}
				dest_buffer.append( '0' );
			}

		#else

			reflective::to_string( dest_buffer, value );

		#endif
	}

	// Enum::find_member_by_value
	const Enum::Member * Enum::find_member_by_value( int value, size_t * inout_index ) const
	{
		/* if inout_index is not nullptr, the search will start from inout_index, and if
		   the member is found, inout_index wil be filled with the index of the member + 1.
		   To get multiple members as result of the search, initialize a size_t with zero, 
		   and pass it as inout_index untill nu is returned. */

		size_t index = 0;
		if( inout_index )
			index = *inout_index;

		const size_t count = _members.count();
		if( index < count ) do {
			if( _members[index].value() == value )
			{
				*inout_index = index + 1;
				return &_members[index];
			}
		} while( ++index < count );

		*inout_index = ~size_t(0);
		return nullptr;
	}

} // namespace reflective


namespace reflective_externals
{
	// reflection of reflective::Enum
	void init_type( reflective::Type * volatile * o_result,
		reflective::Enum * null_pointer_1,
		reflective::Enum * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Enum ThisClass;
		typedef Type BaseClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Enum" );
		*o_result = class_object;
		
		// services
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "members", &ThisClass::members ),
		};
		
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "edit_members", &ThisClass::edit_members ),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->edit_actions().assign( actions );
	}
	
	// reflection of reflective::Enum::Member
	void init_type( reflective::Type * volatile * o_result,
		reflective::Enum::Member * null_pointer_1,
		reflective::Enum::Member * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Enum::Member ThisClass;
		typedef Symbol BaseClass;
		
		if( *o_result != nullptr )
			return;

		// class object
		Class * class_object = new_class<ThisClass>( "reflective::Enum", "Member" );
		*o_result = class_object;
		
		// services
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "value", &ThisClass::value ),
		};
		
		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals
