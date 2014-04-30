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
	QualifiedType QualifiedType::s_empty;

	// QualifiedType::to_string
	void QualifiedType::to_string( StringOutputStream & dest_buffer ) const
	{
		const unsigned indirection_levels = _type_qualification.indirection_levels();

		const Type * final_type; 
		if( indirection_levels == 0 )
		{
			final_type = _type;
		}
		else
		{
			final_type = _type_qualification.final_type();
		}
		final_type->type_full_name_to_string( dest_buffer );

		unsigned curr_indirection_level = 0;
		do {

			if( _type_qualification.is_const( indirection_levels - curr_indirection_level ) )
				dest_buffer.append_literal( " const" );

			if( curr_indirection_level < indirection_levels )
				dest_buffer.append_literal( " *" );	

		} while( ++curr_indirection_level <= indirection_levels );	
	}

	// QualifiedType::assign_from_string
	bool QualifiedType::assign_from_string( FromStringBuffer & source_buffer, StringOutputStream & error_buffer )
	{
		uint32_t constness_word = 0;
		unsigned indirection_levels = 0; 
		bool is_const, is_pointer;
		for(;;)
		{
			is_const = source_buffer.accept_literal_from_end( " const", FromStringBuffer::eIgnoreLeadingSpaces );
			is_pointer = source_buffer.accept_literal_from_end( "*", FromStringBuffer::eIgnoreLeadingSpaces );

			if( is_const )
				constness_word |= (1 << indirection_levels);
			
			if( !is_pointer )
				break;
			
			indirection_levels++;

			if( indirection_levels > TypeQualification::MAX_INDIRECTION_LEVELS )
			{
				error_buffer.append_literal( "Bad type qualification: exceeded the maximum indirection levels" );
				return false;
			}
		}

		StaticConstString type_name( source_buffer.chars(), source_buffer.remaining_length() );
		const Type * type = Namespace::global().find_child_type( type_name );
		if( type == nullptr )
		{
			error_buffer.append_literal( "Could not find the type: " );
			error_buffer.append( source_buffer.chars(), source_buffer.remaining_length() );
			return false;
		}

		if( indirection_levels == 0 )
		{
			_type = type;
		}
		else
		{
			_type = &get_type<void*>();
		}
		_type_qualification = TypeQualification( indirection_levels, *type, constness_word );
				
		return true;
	}

	// QualifiedType::final_type
	const Type * QualifiedType::final_type() const
	{
		const reflective::Type * const qualification_final_type = _type_qualification.final_type();
		if( qualification_final_type )
			return qualification_final_type;

		return _type;
	}

} // namespace reflective


namespace reflective_externals
{	
	// reflection of reflective::QualifiedType
	void init_type( reflective::Type * volatile * o_result,
		reflective::QualifiedType * null_pointer_1,
		reflective::QualifiedType * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::QualifiedType ThisClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "QualifiedType" );
		*o_result = class_object;
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>( 
			eUngrabbedConstructorCopyAssignmentDestructor ) );
		class_object->set_no_base_type();

		class_object->set_string_functions<ThisClass>( &ThisClass::to_string, &ThisClass::assign_from_string );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "type", &ThisClass::type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "qualification", &ThisClass::qualification ),
		};
		
		// assign members
		class_object->assign_properties( properties );	
	}

} // namespace reflective_externals
