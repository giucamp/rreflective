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
	bool Property::set_value_from_string( void * object, const char * str_value, StringOutputStream & error_buffer ) const
	{
		FromStringBuffer str_buff( str_value, strlen(str_value) );
		
		const Type & final_type = *_qualified_type.final_type();
		void * value = reflective_externals::mem_lifo_alloc( final_type.alignment(), final_type.size() );
		if( value == nullptr )
		{
			error_buffer.append_literal( "lifo allocation failed" );
			return false;
		}

		final_type.construct( value );
		bool result = final_type.assign_from_string( str_buff, value, error_buffer );
		if( result )
			set_value( object, value );
		final_type.destroy( value );
		reflective_externals::mem_lifo_free( value );
		return result;
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Property
	reflective::Class * init_type(
		reflective::Property * null_pointer_1,
		reflective::Property * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Property ThisClass;
		typedef ClassMember BaseClass;
		
		static Class * result = nullptr;
		if( result != nullptr )
			return result;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Property" );
		result = class_object;
		
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
		
		// return type
		return result;
	}

} // namespace reflective_externals

