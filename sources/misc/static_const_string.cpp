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
	// StaticConstString::to_string
	void StaticConstString::to_string( StringOutputStream & dest_buffer,
				const Type & type, const void * object )
	{
		REFLECTIVE_UNUSED( type );

		const StaticConstString & string_obj = 
			*static_cast< const StaticConstString * >( object );

		dest_buffer.append( string_obj.start_of_chars(), string_obj.length() );
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::StaticConstString
	void init_type( reflective::Type * volatile * o_result,
		reflective::StaticConstString * null_pointer_1,
		reflective::StaticConstString * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::StaticConstString ThisClass;
		
		if( *o_result != nullptr )
			return;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "StaticConstString" );
		*o_result = class_object;

		// base types
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>( eUngrabbedConstructorCopyAssignmentDestructor ) );
		class_object->set_no_base_type();
		class_object->set_string_functions( StaticConstString::to_string, nullptr );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "start_of_chars", &ThisClass::start_of_chars ),
			new_property<ThisClass>( "length", &ThisClass::length ),
			new_property<ThisClass>( "is_empty", &ThisClass::is_empty ),
		};
		
		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals
