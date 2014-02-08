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
	// decorate_class_instance_name
	SymbolName decorate_class_instance_name( const StaticConstString & class_name, 
		const TemplateParameter * template_parameters, size_t template_parameter_count )
	{
		// compute length
		StringOutputStream class_name_buff( nullptr, 0 );
		class_name_buff.append( class_name.start_of_chars(), class_name.length() );
		class_name_buff.append_literal( "< " );
		TemplateParameter::actual_parameters_to_string( class_name_buff, template_parameters, template_parameter_count );
		class_name_buff.append_literal( " >" );
		REFLECTIVE_ASSERT( class_name_buff.needed_length() > 0 );
		const size_t class_name_length = class_name_buff.needed_length() - 1;

		char * decorated_class_name = static_cast<char *>( 
			reflective_externals::mem_lifo_alloc( 
			alignment_of( char ), (class_name_length + 1) * sizeof( char ) ) );

		// write name
		class_name_buff.set_string_buffer( decorated_class_name, class_name_length + 1 );
		class_name_buff.append( class_name.start_of_chars(), class_name.length() );
		class_name_buff.append_literal( "< " );
		TemplateParameter::actual_parameters_to_string( class_name_buff, template_parameters, template_parameter_count );
		class_name_buff.append_literal( " >" );
		REFLECTIVE_ASSERT( !class_name_buff.is_truncated() );

		return SymbolName( decorated_class_name, class_name_length );
	}

	// new_class
	Class * new_class( const StaticConstString & parent_namespace_path, const StaticConstString & name,
		size_t alignment, size_t size,
		const TemplateParameter * template_parameters, size_t template_parameter_count )
	{
		ClassTemplate * class_template = create_class_template( parent_namespace_path,
			name, template_parameters, template_parameter_count );

		const SymbolName decorated_name = decorate_class_instance_name( name, template_parameters, template_parameter_count );

		Class * class_object = REFLECTIVE_LIFO_NEW( Class, class_template, decorated_name, alignment, size );

		return class_object;
	}	

	// Class::constructor( parent_namespace, name, size, alignment )
	Class::Class( Namespace & parent_namespace, const SymbolName & name,
		size_t alignment, size_t size )
			: Type( parent_namespace, name, size, alignment ),
				_template_paramaters_values( nullptr ),
				_identifier_property( nullptr )
	{		
		set_type_id( eTypeClass );

		_inner_namespace = edit_parent_namespace().find_or_add_child_namespace( name );
		REFLECTIVE_ASSERT( _inner_namespace != nullptr );
	}

	// Class::constructor( namespace_path, name, size, alignment )
	Class::Class( const StaticConstString & parent_namespace_path,
		const SymbolName & name, size_t alignment, size_t size )
			: Type( parent_namespace_path, name, size, alignment ),
				_template_paramaters_values( nullptr ),
				_identifier_property( nullptr )
	{		
		set_type_id( eTypeClass );

		_inner_namespace = edit_parent_namespace().find_or_add_child_namespace( name );
		REFLECTIVE_ASSERT( _inner_namespace != nullptr );
	}

	// Class::constructor( name, size, alignment, namespace_path )
	Class::Class( ClassTemplate * parent_class_template, SymbolName name, 
		size_t alignment, size_t size )
			: Type( *parent_class_template, name, size, alignment )		 
	{
		set_type_id( eTypeClass );

		_inner_namespace = nullptr;
		_identifier_property = nullptr;

		Namespace & parent = edit_parent_namespace();
		//parent.add_child_namespace( _inner_namespace );

		const ClassTemplate & class_template = static_cast<ClassTemplate&>( parent );
		const ParameterList & template_parameter_list = class_template.template_arguments_list();

		_template_paramaters_values = reflective_externals::mem_lifo_alloc( 
			template_parameter_list.total_alignment(), template_parameter_list.total_size() );
		
		///_inner_namespace = class_template;
	}

	// Class::_properties_assigned
	void Class::_properties_assigned()
	{
		const size_t property_count = _properties.count();

		size_t property_index = 0;

		for( ; property_index < property_count; property_index++ )
		{
			const Property & property = _properties[ property_index ];
			if( property.attributes() & ClassMember::IDENTIFIER )
			{
				_identifier_property = &property;
				break;
			}
		}

		#if REFLECTIVE_IS_DEBUG
			if( _identifier_property )
			{
				property_index++;
				for( ; property_index < property_count; property_index++ )
				{
					const Property & property = _properties[ property_index ];
					REFLECTIVE_ASSERT( ( property.attributes() & ClassMember::IDENTIFIER ) == 0 );
				}
			}
		#endif
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Class
	reflective::Class * init_type(
		reflective::Class * null_pointer_1,
		reflective::Class * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Class ThisClass;
		typedef Type BaseClass;
	
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
	
		// class object
		class_object = new_class<ThisClass>( "reflective", "Class" );
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "is_template_instance", &ThisClass::is_template_instance ),
			new_property<ThisClass>( "class_template", &ThisClass::class_template ),
			new_property<ThisClass>( "template_parameter_count", &ThisClass::template_parameter_count ),
			new_property<ThisClass>( "formal_template_parameters", &ThisClass::formal_template_parameters ),
			new_property<ThisClass>( "properties", &ThisClass::properties ),
			new_property<ThisClass>( "actions", &ThisClass::actions ),
			new_property<ThisClass>( "events", &ThisClass::events ),
			new_property<ThisClass>( "base", &ThisClass::base, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "inner_namespace", &ThisClass::inner_namespace ),
			new_property<ThisClass>( "identifier_property", &ThisClass::identifier_property ),
		};
	
		// actions
		const Action * actions[] = 
		{
			//new_action<ThisClass>( "assign_properties", &ThisClass::assign_properties, "items" ),
			new_action<ThisClass>( "edit_actions", &ThisClass::edit_actions ),
			//new_action<ThisClass>( "assign_actions", &ThisClass::assign_actions, "items" ),
			new_action<ThisClass>( "edit_events", &ThisClass::edit_events ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );

		return class_object;
	}
}
