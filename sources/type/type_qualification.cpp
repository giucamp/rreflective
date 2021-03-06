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
	// TypeQualification::full_indirection
	const void * TypeQualification::full_indirection( const void * object ) const
	{
		unsigned levels = indirection_levels();
		while( levels > 0 && object != nullptr )
		{
			object = *reinterpret_cast<const void * const *>( object );
			levels--;
		}
		return object;
	}

	// TypeQualification::full_indirection
	const void * TypeQualification::full_indirection( const void * object,
		const Type * * out_resulting_type ) const
	{
		*out_resulting_type = &pointer_type();
		unsigned levels = indirection_levels();
		while( levels > 0 ) 
		{
			if( !object )
				return nullptr;
			object = *reinterpret_cast<const void * const *>( object );
			levels--;
		}
		*out_resulting_type = m_final_type;
		return object;
	}
}

namespace reflective_externals
{
	// reflection of reflective::TypeQualification
	void init_type( reflective::Type * volatile * o_result,
		reflective::TypeQualification * null_pointer_1,
		reflective::TypeQualification * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::TypeQualification ThisClass;
	
		if( *o_result != nullptr )
			return;
	
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "TypeQualification" );
		*o_result = class_object;
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "final_type", &ThisClass::final_type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "indirection_levels", &ThisClass::indirection_levels ),
			new_property<ThisClass>( "constness_word", &ThisClass::constness_word ),
			new_property<ThisClass>( "is_pointer", &ThisClass::is_pointer ),
			new_property<ThisClass>( "is_reference", &ThisClass::is_reference ),
			new_property<ThisClass>( "indirection_word", &ThisClass::get_indirection_word ),
		};
	
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "init", &ThisClass::init, "indirection_word, final_type" ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	}

} // namespace reflective_externals
