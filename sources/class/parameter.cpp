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
	// Parameter::to_string
	void Parameter::to_string( ToStringBuffer & dest_buffer ) const
	{
		_qualified_type.to_string( dest_buffer );
		dest_buffer.append( ' ' );
		name().to_string( dest_buffer );
	}

	// ParameterList::_compute_totals
	void ParameterList::_compute_totals()
	{	
		_total_alignment = 1;
		_total_size = 0;

		size_t parameter_index;
		const size_t parameter_count = count();
		if( parameter_count == 0 )
			return;	

		// find total alignment
		parameter_index = 0; do {

			const Parameter & parameter = (*this)[ parameter_index ];
			const Type & type = parameter.type();
			const size_t alignment = type.alignment();

			if( alignment > _total_alignment )
				_total_alignment = alignment;
			
		} while( ++parameter_index < parameter_count );

		// find total size
		const size_t alignment_mask = _total_alignment - 1;
		parameter_index = 0; do {

			const Parameter & parameter = (*this)[ parameter_index ];
			const Type & type = parameter.type();
			size_t size = type.size();

			// upper align the size
			size += alignment_mask;
			size &= ~alignment_mask;

			// accumulate
			_total_size += size;
			
		} while( ++parameter_index < parameter_count );
	}

	// ParameterList::actual_params_to_string
	void ParameterList::actual_params_to_string( ToStringBuffer & dest, const void * actual_parameters )
	{
		Walker<const void*> walker( *this, actual_parameters );
		bool first = true;
		if( walker ) do {

			if( !first )
			{
				dest.append_literal( ", " );
			}
			first = false;
			
			const Type & type = walker.parameter().type();
			const void * value = walker.value();
			type.to_string( dest, value );

		} while( ++walker );
	}

	// ParameterList::parse_names
	unsigned ParameterList::parse_names( const char * string, SymbolName * names, unsigned count )
	{
		if( !string )
			return 0;

		unsigned curr_name_index = 0;

		curr_name_index = 0;
		const char * curr_char_pointer = string;
		const char * curr_name_start = string;
		do {
			const char curr_char = *curr_char_pointer;

			if( curr_char >= 'a' && curr_char <= 'z' ||
				curr_char >= 'A' && curr_char <= 'Z' ||
				curr_char >= '0' && curr_char <= '9' ||
				curr_char == '_' || curr_char == ' ' )
			{
				if( !curr_name_start )
					curr_name_start = curr_char_pointer;
			}
			else 
			{
				REFLECTIVE_ASSERT( curr_char == ',' );
				if( curr_name_start )
				{
					while( *curr_name_start == ' ' )
						curr_name_start++;
					names[curr_name_index] = SymbolName( curr_name_start,
						curr_char_pointer - curr_name_start );
					curr_name_start = nullptr;
					curr_name_index++;
					if( curr_name_index >= count )
						return curr_name_index;
				}
			}

		} while( *++curr_char_pointer );

		if( curr_name_start )
		{
			while( *curr_name_start == ' ' )
				curr_name_start++;

			names[curr_name_index] = SymbolName( curr_name_start,
				curr_char_pointer - curr_name_start );
			//curr_name_start = nullptr;
			curr_name_index++;
		}

		return curr_name_index;
	}


} // namespace reflective



namespace reflective_externals
{
	// reflection of reflective::Parameter
	reflective::Class * init_type(
		reflective::Parameter * null_pointer_1,
		reflective::Parameter * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Parameter ThisClass;
		typedef reflective::Symbol BaseClass;
		
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "Parameter" );
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "type", &ThisClass::type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "qualification", &ThisClass::qualification ),
		};
		
		// assign members
		class_object->assign_properties( properties );

		class_object->set_string_functions<ThisClass>( &ThisClass::to_string, 0 );

		class_object->set_collection_handler( get_type< BaseClass >().collection_handler() );
		
		// return type
		return class_object;
	}


	// reflection of reflective::ParameterList
	reflective::Class * init_type(
		reflective::ParameterList * null_pointer_1,
		reflective::ParameterList * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ParameterList ThisClass;
		typedef SymbolList<Parameter, false> BaseClass;
	
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
	
		// class object
		class_object = new_class<ThisClass>( "reflective", "ParameterList" );
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "total_alignment", &ThisClass::total_alignment ),
			new_property<ThisClass>( "total_size", &ThisClass::total_size ),
		};
	
		// actions
		const Action * actions[] = 
		{
			//new_action<ThisClass>( "destroy_values", &ThisClass::destroy_values, "actual_parameters" ),
			new_action<ThisClass>( "actual_params_to_string", &ThisClass::actual_params_to_string, "dest, actual_parameters" ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	
		// return type
		return class_object;
	}

} // namespace reflective_externals
