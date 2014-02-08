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
	// MethodAction_0
	template < class OBJECT, class RETURN_TYPE >
		class MethodAction_0 : public Action
	{
	public:

		typedef RETURN_TYPE (OBJECT::*Method)();
		typedef bool (OBJECT::*CanInvokeMethod)( StringOutputStream & out_reason_text ) const;

		MethodAction_0( SymbolName name, Attributes attributes, Method method, CanInvokeMethod can_invoke_method )
				: Action( name, attributes, safe_get_type<RETURN_TYPE>(), 
					safe_get_type_qualification<RETURN_TYPE>() ), _method( method ), _can_invoke_method( can_invoke_method )
		{
		}

		// can_invoke
		bool can_invoke( const void * object, StringOutputStream & out_reason_text ) const	
		{
			if( !_can_invoke_method )
				return true;

			const OBJECT * typed_object = static_cast<const OBJECT *>( object );
			return (typed_object->*_can_invoke_method)( out_reason_text );
		}

	protected:

		void on_invoke( void * object, const void * actual_parameters, void * return_value ) const
		{
			REFLECTIVE_UNUSED( actual_parameters );

			OBJECT * typed_object = static_cast<OBJECT *>( object );

			REFLECTIVE_ASSERT( actual_parameters == nullptr );

			/* calls the method and copy-construct the return value in the
				buffer pointed by return_value. */
			Unreferencer<RETURN_TYPE>::copy_construct( 
				return_value, (typed_object->*_method)() );
		}

	private: // data members
		Method _method;
		CanInvokeMethod _can_invoke_method;
	};

	// MethodAction_0
	template < class OBJECT>
		class MethodAction_0<OBJECT, void> : public Action
	{
	public:

		typedef void (OBJECT::*Method)();
		typedef bool (OBJECT::*CanInvokeMethod)( StringOutputStream & out_reason_text ) const;

		MethodAction_0( SymbolName name, Attributes attributes, Method method, CanInvokeMethod can_invoke_method )
				: Action( name, attributes, safe_get_type<void>(), 
					safe_get_type_qualification<void>() ), _method( method ), _can_invoke_method( can_invoke_method )
		{
		}

		// can_invoke
		bool can_invoke( const void * object, StringOutputStream & out_reason_text ) const	
		{
			if( !_can_invoke_method )
				return true;

			const OBJECT * typed_object = static_cast<const OBJECT *>( object );
			return (typed_object->*_can_invoke_method)( out_reason_text );
		}

	protected:
		void on_invoke( void * object, const void * actual_parameters, void * return_value ) const
		{			
			REFLECTIVE_ASSERT( actual_parameters == nullptr );
			REFLECTIVE_ASSERT( return_value == nullptr );
			REFLECTIVE_UNUSED_2( actual_parameters, return_value );

			OBJECT * typed_object = static_cast<OBJECT *>( object );
			(typed_object->*_method)();
		}

	private: // data members
		Method _method;
		CanInvokeMethod _can_invoke_method;
	};

	// _new_action( name, Class::method, [attributes], [category] )
	template <class OBJECT, class RETURN_TYPE>
		inline Action * _new_action( 
			SymbolName name, 
			RETURN_TYPE (OBJECT::*method)(), 
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( StringOutputStream & out_reason_text ) const )
	{
		typedef MethodAction_0< OBJECT, RETURN_TYPE > MethodAction;

		REFLECTIVE_ASSERT( parameter_names == nullptr || *parameter_names == 0 );
		REFLECTIVE_UNUSED( parameter_names );

		void * allocation = reflective_externals::mem_lifo_alloc( 
			alignment_of(MethodAction), sizeof(MethodAction),
			&default_destroyer<ClassMember> );

		MethodAction * action = new( allocation ) MethodAction_0<OBJECT, RETURN_TYPE>( 
			name, attributes, method, can_invoke_method );
		return action;
	}

} // namespace reflective
