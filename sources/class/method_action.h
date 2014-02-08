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

#ifndef _INCLUDING_REFLECTIVE
	#error include "reflective.h" instead of including this file
#endif

namespace reflective
{
	// _new_action( name, Class::method, [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE>
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)(), 
			const char * parameter_names,
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE, class PARAM_1 >
		Action * _new_action( SymbolName name, RETURN_TYPE (OBJECT::*method)( PARAM_1 par1 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2 >
		Action * _new_action( SymbolName name, RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2 ), 
			const char * parameter_names,	
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3 ), 
			const char * parameter_names,				
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3,
			class PARAM_4 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3, 
											PARAM_4 par4 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3,
			class PARAM_4, class PARAM_5 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3, 
											PARAM_4 par4, PARAM_5 par5 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3,
			class PARAM_4, class PARAM_5, class PARAM_6 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3, 
											PARAM_4 par4, PARAM_5 par5, PARAM_6 par6 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3,
			class PARAM_4, class PARAM_5, class PARAM_6,
			class PARAM_7 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3, 
											PARAM_4 par4, PARAM_5 par5, PARAM_6 par6,
											PARAM_7 par7 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE,
			class PARAM_1, class PARAM_2, class PARAM_3,
			class PARAM_4, class PARAM_5, class PARAM_6,
			class PARAM_7, class PARAM_8 >
		Action * _new_action( SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1, PARAM_2 par2, PARAM_3 par3, 
											PARAM_4 par4, PARAM_5 par5, PARAM_6 par6,
											PARAM_7 par7, PARAM_8 par8 ), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const );


	// new_action( name, Class::method, parameter_names, attributes, can_invoke_method )
	template < class CLASS, class METHOD, class CAN_INVOKE_METHOD >
		Action * new_action( SymbolName name, 
			METHOD method,
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			CAN_INVOKE_METHOD can_invoke_method )
	{
		return _new_action<CLASS>( name, method, parameter_names, attributes, can_invoke_method );
	}

	// new_action( name, Class::method, parameter_names, can_invoke_method )
	template < class CLASS, class METHOD >
		Action * new_action( SymbolName name, 
			METHOD method,
			const char * parameter_names,
			ClassMember::Attributes attributes )
	{
		return _new_action<CLASS>( name, method, parameter_names, attributes, 0 );
	}

	// new_action( name, Class::method, parameter_names )
	template < class CLASS, class METHOD >
		Action * new_action( SymbolName name, 
			METHOD method,
			const char * parameter_names )
	{
		return _new_action<CLASS>( name, method, parameter_names, ClassMember::NO_ATTRIBUTES, 0 );
	}

	// new_action( name, Class::method, parameter_names )
	template < class CLASS, class METHOD >
		Action * new_action( SymbolName name, 
			METHOD method )
	{
		return _new_action<CLASS>( name, method, nullptr, ClassMember::NO_ATTRIBUTES, 0 );
	}

} // namespace reflective
