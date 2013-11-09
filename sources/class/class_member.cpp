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

namespace reflective
{


} // namespace reflective


namespace reflective_externals
{
	// reflection of reflective::ClassMember
	reflective::Class * init_type(
		reflective::ClassMember * null_pointer_1,
		reflective::ClassMember * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ClassMember ThisClass;
		typedef Symbol BaseClass;
	
		static Class * class_object = null;
		if( class_object != null )
			return class_object;
	
		// class object
		class_object = new_class<ThisClass>( "reflective", "ClassMember" );
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "attributes", &ThisClass::attributes ),
			new_property<ThisClass>( "is_public", &ThisClass::is_public ),
			new_property<ThisClass>( "is_private", &ThisClass::is_private ),
			new_property<ThisClass>( "is_protected", &ThisClass::is_protected ),
			new_property<ThisClass>( "is_back_reference", &ThisClass::is_back_reference ),
		};
	
		// assign members
		class_object->assign_properties( properties );
	
		// return type
		return class_object;
	}

	// reflection of reflective::ClassMember::Attributes
	reflective::Enum * init_type(
		reflective::ClassMember::Attributes * null_pointer_1,
		reflective::ClassMember::Attributes * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		
		static Enum * result = null;
		if( result != null )
			return result;
		
		const Enum::Member * members[] = 
		{
			new_enum_member( "NO_ATTRIBUTES", reflective::ClassMember::NO_ATTRIBUTES ),
			new_enum_member( "PROTECTED", reflective::ClassMember::PROTECTED ),
			new_enum_member( "PRIVATE", reflective::ClassMember::PRIVATE ),
			new_enum_member( "BACK_REFERENCE", reflective::ClassMember::BACK_REFERENCE ),
			new_enum_member( "READONLY", reflective::ClassMember::READONLY ),
			new_enum_member( "IDENTIFIER", reflective::ClassMember::IDENTIFIER ),
			new_enum_member( "DO_NOT_SERIALIZE", reflective::ClassMember::DO_NOT_SERIALIZE ),
			new_enum_member( "ALL_ATTRIBUTES", reflective::ClassMember::ALL_ATTRIBUTES ),
		};
		
		Enum * enum_object = new_enum( "reflective::ClassMember", "Attributes" );
		enum_object->edit_members().assign( members );
		
		// assign and return type
		result = enum_object;
		return result;
	}

} // namespace reflective_externals
