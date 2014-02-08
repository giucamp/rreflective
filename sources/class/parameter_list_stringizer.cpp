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
	// ParameterStringizer::constructor
	ParameterStringizer::ParameterStringizer()
	{
	}


	// ParameterStringizer::parse_actual_list
	bool ParameterStringizer::parse_actual_list( const ParameterList & parameter_list,
			FromStringBuffer & source_string_buffer, void * destination,
			StringOutputStream & error_message, size_t * out_processed_parameters_count )
	{
		// const char * chars = source_string_buffer.chars();
		// const size_t formal_parameter_count = parameter_list.count();
		void * curr_dest = destination;

		size_t parameter_index = 0;		
		//const char * curr_char = chars;

		const size_t parameter_list_count = parameter_list.count();

		for(;;)
		{
			const char * start_of_token = source_string_buffer.chars();

			char character;
			bool non_white_char = false;
			for(;;)
			{
				character = source_string_buffer.consume_char();

				non_white_char = non_white_char || isspace( character ) == 0;

				if( character == ',' || character == 0 )
					break;
			}

			const char * end_of_token = source_string_buffer.chars();
			
			FromStringBuffer value_string( start_of_token, end_of_token - start_of_token );


			// write value			

			const Parameter & parameter = parameter_list[ parameter_index ];

			const Type & parameter_type = parameter.type();			

			curr_dest = mem::upper_align( curr_dest, parameter_type.alignment() );

			parameter_type.construct( curr_dest );

			if( non_white_char )
			{
				if( !parameter_type.check_capabilities( Type::eHasFromStringAssigner ) )
				{
					parameter_type.type_full_name_to_string( error_message );
					error_message.append_literal( " does not support assigmement from string" );

					if( out_processed_parameters_count )
						*out_processed_parameters_count = parameter_index;

					return false;
				}

				bool result = parameter_type.assign_from_string( value_string, curr_dest, error_message );			
				if( !result )
				{
					if( out_processed_parameters_count )
						*out_processed_parameters_count = parameter_index;
					return false;
				}
			}

			// next parameter 

			curr_dest = mem::address_add( curr_dest, parameter_type.size() );

			parameter_index++;
			if( parameter_index >= parameter_list_count )
				break;
		}
		
		if( out_processed_parameters_count )
			*out_processed_parameters_count = parameter_index;
		return true;
	}

	// ActionInvoker::constructor
	ActionInvoker::ActionInvoker()
		: _action( nullptr ), _parameters( nullptr ), _return_value( nullptr )
	{
	}

	// ActionInvoker::destructor
	ActionInvoker::~ActionInvoker()
	{
		close();
	}

	// ActionInvoker::setup
	bool ActionInvoker::setup( const reflective::Action * action, 
			FromStringBuffer & parameters_string_buffer,
			StringOutputStream & out_error_message, 
			size_t * out_processed_parameters_count )
	{
		close();

		const ParameterList & parameter_list = action->parameter_list();

		bool allocation_failed = false;

		_parameters = reflective_externals::mem_alloc( parameter_list.total_alignment(), parameter_list.total_size() );
		if( _parameters == nullptr )
			allocation_failed = true;

		const Type & return_type = action->return_type();

		if( return_type != reflective::get_type< void >() )
		{
			_return_value = reflective_externals::mem_alloc( return_type.alignment(), return_type.size() );

			if( _return_value == nullptr )
				allocation_failed = true;
		}

		if( allocation_failed )
		{
			if( _parameters != nullptr )
				reflective_externals::mem_free( _parameters );

			if( _return_value != nullptr )
				reflective_externals::mem_free( _return_value );

			out_error_message.append_literal( "allocation failed" );
			return false;
		}		

		_action = action;

		ParameterStringizer parameter_stringizer;
		size_t processed_params = 0;
		const bool parse_result = parameter_stringizer.parse_actual_list( parameter_list,  
			parameters_string_buffer, _parameters, out_error_message, &processed_params );		

		*out_processed_parameters_count = processed_params;

		if( parse_result )
			return true;

		ParameterList::Walker<void *> param_walker( parameter_list, _parameters );

		while( param_walker.is_not_over() && processed_params > 0 )
		{
			void * value = param_walker.value();

			const Type & type = param_walker.parameter().type();

			type.destroy( value );

			processed_params--;
			++param_walker;
		}

		reflective_externals::mem_free( _parameters );

		if( _return_value != nullptr )
			reflective_externals::mem_free( _return_value );

		_action = nullptr;

		_parameters = nullptr;

		_return_value = nullptr;	

		return false;
	}

	// ActionInvoker::setup
	void ActionInvoker::setup( const Action * action )
	{
		_action = action;

		_parameters = nullptr;

		_return_value = nullptr;	
	}

	// ActionInvoker::invoke
	bool ActionInvoker::invoke( void * object, 
		StringOutputStream * out_return_value,
		StringOutputStream * out_reason_text ) const
	{
		if( _action == nullptr )
		{
			if( out_reason_text )
				out_reason_text->append_literal( "action not set" );

			return false;
		}

		const bool invoke_result = _action->invoke( object, _parameters, _return_value );
		if( invoke_result )
		{
			const Type & return_type = _action->return_type();

			if( _return_value != nullptr )
			{
				if( out_return_value )
				{
					if( return_type.check_capabilities( Type::eHasFromStringAssigner ) )	
					{
						return_type.to_string( *out_return_value, _return_value );
					}
					else
					{
						return_type.type_full_name_to_string( *out_return_value );
						out_return_value->append_literal( " does not support to_string" );
					}
				}

				return_type.destroy( _return_value );
			}
			else
			{
				if( out_return_value )
					out_return_value->append_literal( "void" );
			}

			return true;
		}

		if( out_reason_text )
		{
			const size_t prev_reason_length = out_reason_text->needed_length();
			bool can_invoke_result = _action->can_invoke( object, *out_reason_text );

			REFLECTIVE_ASSERT( can_invoke_result );

			if( !can_invoke_result && prev_reason_length == out_reason_text->needed_length() )
			{
				out_reason_text->append_literal( "invoke failed for unspecified reason" );
			}
		}

		return false;
	}

	// ActionInvoker::close
	void ActionInvoker::close()
	{
		if( _action == nullptr )
			return;

		const ParameterList & parameter_list = _action->parameter_list();

		ParameterList::Walker<void *> param_walker( parameter_list, _parameters );

		while( param_walker.is_not_over() )
		{
			void * value = param_walker.value();

			const Type & type = param_walker.parameter().type();

			type.destroy( value );

			++param_walker;
		}

		reflective_externals::mem_free( _parameters );

		if( _return_value != nullptr )
			reflective_externals::mem_free( _return_value );

		_action = nullptr;

		_parameters = nullptr;

		_return_value = nullptr;	
	}

} // namespace reflective


namespace reflective_externals
{	
	// reflection of reflective::ActionInvoker
	reflective::Class * init_type(
		reflective::ActionInvoker * null_pointer_1,
		reflective::ActionInvoker * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ActionInvoker ThisClass;
		
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "ActionInvoker" );
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorDestructor ) );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "action", &ThisClass::action ),
		};
		
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "close", &ThisClass::close ),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
		
		// return type
		return class_object;
	}
	
	// reflection of reflective::ParameterStringizer
	reflective::Class * init_type(
		reflective::ParameterStringizer * null_pointer_1,
		reflective::ParameterStringizer * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ParameterStringizer ThisClass;
		
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "ParameterStringizer" );
		class_object->set_no_base_type();

		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "parse_actual_list", &ThisClass::parse_actual_list, "parameter_list, source_string_buffer, destination, error_message, out_processed_parameters_count" ),
		};
	
		// assign members
		class_object->assign_actions( actions );
		
		// return type
		return class_object;
	}

} // namespace reflective_externals
