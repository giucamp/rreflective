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
	bool Property::set_value_from_string( void * i_object, const char * i_string, StringOutputStream & io_error_buffer ) const
	{	
		// check if the operation is supported by the property and by the type
		if( attributes() & ClassMember::READONLY )
		{
			io_error_buffer << name() << " is readonly";
			return false; 
		}
		const Type & final_type = *_qualified_type.final_type();
		if( !final_type.check_capabilities(Type::eHasFromStringAssigner) )
		{
			io_error_buffer << final_type.full_name() << " does not support from_string";
			return false;
		}

		const unsigned indirection_levels = _qualified_type.qualification().indirection_levels();

		// get the value of the property
		bool result = true;
		bool value_constructed = false;
		const Type & type = *_qualified_type.type();
		void * allocated_value = nullptr;
		void * value = get_value_inplace( i_object );
		if( value == nullptr )
		{
			allocated_value = value = reflective_externals::mem_lifo_alloc( type.alignment(), type.size() );
			if( allocated_value == nullptr )
			{
				io_error_buffer.append_literal( "lifo allocation failed" );
				return false;
			}
			else 
			{
				if( indirection_levels == 0 )
				{
					type.construct( value ); // non pointer
					value_constructed = true;
				}
				else
				{
					result = get_value( i_object, value ); // pointer: get the value
					value_constructed = result;
				}
			}
		}

		// deference the pointers
		if( result )
		{
			for( unsigned indirection_level = 0; indirection_level < indirection_levels; indirection_level++ )
			{
				if( value == nullptr )
				{
					io_error_buffer.append_literal( "encountered null deferencing the pointers" );
					result = false;
					break;
				}
				value = *static_cast<void**>( value );
			}
		}

		// parse the string
		if( result )
		{
			FromStringBuffer str_buff( i_string, strlen(i_string) );
			result = final_type.assign_from_string( str_buff, value, io_error_buffer );
			if( result && allocated_value != nullptr )
			{
				result = set_value( i_object, allocated_value );
			}
		}

		// destroy the value		
		if( allocated_value != nullptr )
		{
			if( value_constructed )
				type.destroy( allocated_value );
			reflective_externals::mem_lifo_free( allocated_value );
		}

		return result;
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Property
	void init_type( reflective::Type * volatile * o_result,
		reflective::Property * null_pointer_1,
		reflective::Property * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Property ThisClass;
		typedef ClassMember BaseClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Property" );
		*o_result = class_object;
		
		// services
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "qualified_type", &ThisClass::qualified_type ),
			new_property<ThisClass>( "type", &ThisClass::type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "qualification", &ThisClass::qualification ),
			new_property<ThisClass>( "type_has_tostring", &ThisClass::type_has_tostring ),			
		};
		
		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals

