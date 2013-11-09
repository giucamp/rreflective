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
	// QualifiedType::to_string
	void QualifiedType::to_string( ToStringBuffer & dest_buffer ) const
	{
		const unsigned indirection_levels = _type_qualification.indirection_levels();

		if( indirection_levels == 0 )
		{
			_type->type_full_name_to_string( dest_buffer );

			if( _type_qualification.is_const( 0 ) )
				dest_buffer.append_literal( " const" );
		}
		else
		{
			_type_qualification.final_type()->type_full_name_to_string( dest_buffer );

			unsigned curr_indirection_level = 0;
			do {

				if( _type_qualification.is_const( indirection_levels - curr_indirection_level ) )
					dest_buffer.append_literal( " const" );

				if( curr_indirection_level < indirection_levels )
					dest_buffer.append_literal( " *" );	

			} while( ++curr_indirection_level <= indirection_levels );				
		}	
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
	reflective::Class * init_type(
		reflective::QualifiedType * null_pointer_1,
		reflective::QualifiedType * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::QualifiedType ThisClass;
		
		static Class * class_object = null;
		if( class_object != null )
			return class_object;
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "QualifiedType" );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>( 
			eUngrabbedConstructorCopyAssignmentDestructor ) );
		class_object->set_no_base_type();

		class_object->set_string_functions<ThisClass>( &ThisClass::to_string, 0 );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "type", &ThisClass::type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "qualification", &ThisClass::qualification ),
		};
		
		// assign members
		class_object->assign_properties( properties );		
		
		// return type
		return class_object;
	}

} // namespace reflective_externals
