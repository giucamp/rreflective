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

	// Action::invoke
	bool Action::invoke( void * object, FromStringBuffer & parameters_string_buffer, 
			ToStringBuffer & out_return_value_text, ToStringBuffer & out_error_text ) const
	{
		if( !_return_type.check_capabilities( Type::eHasFromStringAssigner ) )
		{
			return false;
		}

		void * return_value = reflective_externals::mem_lifo_alloc( _return_type.alignment(), _return_type.size() );

		void * param_values = reflective_externals::mem_lifo_alloc( _parameter_list.total_alignment(), _parameter_list.total_size() );


		// parse parameter actual list

		ToStringBuffer error;

		size_t constructed_parameters_count = 0;

		ParameterStringizer stringizer;

		const bool stringizer_result = stringizer.parse_actual_list( _parameter_list, parameters_string_buffer,
			param_values, out_error_text, &constructed_parameters_count );
		
		bool result = stringizer_result;

		if( !result )
		{
			out_error_text.append_literal( "| action invoke failed bacauase of parameter " );

			to_string( out_error_text, constructed_parameters_count );
		}
		else
		{
			// invoke

			result = invoke( object, param_values, return_value );

			if( result )
			{
				// stringize return value
				if( !_return_type.check_capabilities( Type::eHasFromStringAssigner ) )				
					_return_type.to_string( out_return_value_text, return_value );
				else
				{
					_return_type.type_full_name_to_string( out_return_value_text );
					out_return_value_text.append( " does not support to_string" );
				}
			}
			else
			{
				// ask reason
				bool can_invoke_result = can_invoke( object, out_error_text );
				REFLECTIVE_ASSERT( !can_invoke_result );
				if( can_invoke_result )
				{
					out_error_text.append_literal( "action invoke failed for unspecified reason" );
				}
			}
		}

		// delete params and return value

		ParameterList::Walker<void *> param_walker( _parameter_list, param_values );

		while( param_walker.is_not_over() && constructed_parameters_count > 0 )
		{
			void * value = param_walker.value();

			const Type & type = param_walker.parameter().type();

			type.destroy( value );

			constructed_parameters_count--;
			++param_walker;
		}

		REFLECTIVE_ASSERT( constructed_parameters_count == 0 );

		if( result )
		{
			_return_type.destroy( return_value );
		}

		reflective_externals::mem_lifo_free( param_values );		

		reflective_externals::mem_lifo_free( return_value );	

		return result;
	}


} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Action
	reflective::Class * init_type(
		reflective::Action * null_pointer_1,
		reflective::Action * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Action ThisClass;
		typedef ClassMember BaseClass;
		
		static Class * result = null;
		if( result != null )
			return result;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Action" );		
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eNoLifeFunctions ) );

		result =  class_object;
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "return_type", &ThisClass::return_type, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "return_type_qualification", &ThisClass::return_type_qualification ),
			new_property<ThisClass>( "parameter_list", &ThisClass::parameter_list ),
		};
	
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "edit_parameter_list", &ThisClass::edit_parameter_list ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	
		// return type
		return class_object;
	}

} // namespace reflective_externals

