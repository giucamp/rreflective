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
	// MethodAction_1 (1 param)
	template < class OBJECT, class RETURN_TYPE, class PARAM_1 >
		class MethodAction_1 : public Action
	{
	public:

		typedef RETURN_TYPE (OBJECT::*Method)( PARAM_1 par1 );
		typedef bool (OBJECT::*CanInvokeMethod)( ToStringBuffer & out_reason_text ) const;

		// constructor
		MethodAction_1( SymbolName name, Attributes attributes, Method method,
			const char * parameter_names, CanInvokeMethod can_invoke_method )
				: Action( name, attributes, safe_get_type<RETURN_TYPE>(), 
				safe_get_type_qualification<RETURN_TYPE>() ), _method( method ), _can_invoke_method( can_invoke_method )
		{
			const unsigned parameter_count = sizeof( _parameters ) / sizeof( _parameters[0] );

			// assign parameter list
			SymbolName names[ parameter_count ];
			ParameterList::parse_names( parameter_names, names, parameter_count );
			_parameters[0] = new_parameter<PARAM_1>( names[0] );

			edit_parameter_list().assign( _parameters, parameter_count );
		}

	protected:

		// on_invoke
		void on_invoke( void * object, const void * actual_parameters, void * return_value ) const
		{
			REFLECTIVE_ASSERT( return_value == null );
			REFLECTIVE_UNUSED( return_value );

			// parameters
			ParameterList::Walker<const void *> param_walker( parameter_list(), actual_parameters );			
			const void * param_1 = param_walker.value(); // ++param_walker;

			#ifdef _MSC_VER
				#pragma warning( push )
				#pragma warning( disable: 4267 ) /* this warning (conversion from 'size_t' to 'type', possible loss 
					of data) may be generated compiling with /Wp64, bacause the compiler uses the same template instance 
					for size_t and unsigned int. */
			#endif

			/* calls the method and copy-construct the return value in the
				buffer pointed by return_value. */
			OBJECT * typed_object = static_cast<OBJECT *>( object );
			Unreferencer<RETURN_TYPE>::copy_construct( 
				return_value, (typed_object->*_method)( 
					Unreferencer<PARAM_1>::void_indirection( param_1 ) ) );

			#ifdef _MSC_VER
				#pragma warning( pop )
			#endif
		}

	private: // data members
		Method _method;
		CanInvokeMethod _can_invoke_method;
		const Parameter * _parameters[1];
	};

	// MethodAction_1 (1 param, no return value)
	template < class OBJECT, class PARAM_1 >
		class MethodAction_1< OBJECT, void, PARAM_1> : public Action
	{
	public:

		typedef void (OBJECT::*Method)( PARAM_1 par1 );
		typedef bool (OBJECT::*CanInvokeMethod)( ToStringBuffer & out_reason_text ) const;

		// constructor
		MethodAction_1( SymbolName name, Attributes attributes, Method method,
			const char * parameter_names, CanInvokeMethod can_invoke_method )
				: Action( name, attributes, safe_get_type<void>(), 
				safe_get_type_qualification<void>() ), _method( method ), _can_invoke_method( can_invoke_method )
		{
			const unsigned parameter_count = sizeof( _parameters ) / sizeof( _parameters[0] );

			// assign parameter list
			SymbolName names[ parameter_count ];
			ParameterList::parse_names( parameter_names, names, parameter_count );
			_parameters[0] = new_parameter<PARAM_1>( names[0] );

			edit_parameter_list().assign( _parameters, parameter_count );
		}

	protected:

		// on_invoke
		void on_invoke( void * object, const void * actual_parameters, void * return_value ) const
		{			
			REFLECTIVE_ASSERT( return_value == NULL );
			REFLECTIVE_UNUSED( return_value );

			// parameters
			ParameterList::Walker<const void *> param_walker( parameter_list(), actual_parameters );			
			const void * param_1 = param_walker.value(); // ++param_walker;

			#ifdef _MSC_VER
				#pragma warning( push )
				#pragma warning( disable: 4267 ) /* this warning (conversion from 'size_t' to 'type', possible loss 
					of data) may be generated compiling with /Wp64, bacause the compiler uses the same template instance 
					for size_t and unsigned int. */
			#endif

			// calls the method (no return value)
			OBJECT * typed_object = static_cast<OBJECT *>( object );
			(typed_object->*_method)( 
				Unreferencer<PARAM_1>::void_indirection( param_1 ) );

			#ifdef _MSC_VER
				#pragma warning( pop )
			#endif
		}

	private: // data members
		Method _method;
		CanInvokeMethod _can_invoke_method;
		const Parameter * _parameters[1];
	};

	// _new_action( name, Class::method, [parameter_names], [attributes], [category] )
	template < class OBJECT, class RETURN_TYPE, class PARAM_1 >
		inline Action * _new_action( 
			SymbolName name, 
			RETURN_TYPE (OBJECT::*method)( PARAM_1 par1 ), 			
			const char * parameter_names,			
			ClassMember::Attributes attributes,
			bool (OBJECT::*can_invoke_method)( ToStringBuffer & out_reason_text ) const )
	{
		typedef MethodAction_1< OBJECT, RETURN_TYPE, PARAM_1 > MethodAction;
		void * allocation = reflective_externals::mem_lifo_alloc( 
			alignment_of(MethodAction), sizeof(MethodAction),
			&default_destroyer<ClassMember> );

		MethodAction * action = new( allocation ) MethodAction( 
			name, attributes, method, parameter_names, can_invoke_method );

		return action;
	}

} // namespace reflective
