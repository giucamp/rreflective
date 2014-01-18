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
	// new_class<CLASS>()
	template <class CLASS>
		Class * new_class()
	{
		StaticConstString parent_namespace_path;
		SymbolName name;

		const char * full_type_name = typeid( CLASS ).name();
		separate_symbol_name( full_type_name, &parent_namespace_path, &name, false );

		Class * class_object = REFLECTIVE_LIFO_NEW( Class, parent_namespace_path, name, alignment_of( CLASS ), sizeof( CLASS ) );

		return class_object;
	}

	// new_class<CLASS>( template_parameters[] )
	template <class CLASS, size_t TEMPLATE_PARAMETER_COUNT> Class * new_class(
		const TemplateParameter (&template_parameters)[ TEMPLATE_PARAMETER_COUNT ] )
	{
		StaticConstString parent_namespace_path;
		
		const char * full_type_name = typeid( CLASS ).name();
		StaticConstString name;
		separate_symbol_name( full_type_name, &parent_namespace_path, &name, true );

		return new_class( parent_namespace_path, name, 
			alignment_of( CLASS ), sizeof( CLASS ), template_parameters, TEMPLATE_PARAMETER_COUNT );
	}

	// new_class<CLASS>( namespace_path, name )
	template <class CLASS>
		Class * new_class( const StaticConstString & parent_namespace_path, const SymbolName & name )
	{
		Class * class_object = REFLECTIVE_LIFO_NEW( Class, parent_namespace_path, name, alignment_of( CLASS ), sizeof( CLASS ) );
		return class_object;
	}

	// new_class<CLASS>( namespace_path, name, template_parameters[] )
	template < class CLASS, size_t TEMPLATE_PARAMETER_COUNT >
		inline Class * new_class( const StaticConstString & parent_namespace_path,
			const char * name, const TemplateParameter (&template_parameters)[ TEMPLATE_PARAMETER_COUNT ] )
	{
		return new_class( parent_namespace_path, name, alignment_of( CLASS ), sizeof( CLASS ),
			template_parameters, TEMPLATE_PARAMETER_COUNT );
	}

	// Class::destructor
	inline Class::~Class()
	{		
	}

	// Class::is_template_instance
	inline bool Class::is_template_instance() const
	{
		return _template_paramaters_values != nullptr;
	}

	// Class::class_template
	inline const ClassTemplate * Class::class_template() const
	{
		if( !_template_paramaters_values )
			return nullptr;

		const Namespace & parent = parent_namespace();
		return static_cast<const ClassTemplate*>( &parent );
	}

	// Class::template_parameter_count
	inline size_t Class::template_parameter_count() const
	{
		if( !_template_paramaters_values )
			return 0;

		const Namespace & parent = parent_namespace();
		const ClassTemplate & class_template = static_cast<const ClassTemplate&>( parent );

		return class_template.template_arguments_list().count();
	}

	// Class::formal_template_parameters
	inline const ParameterList * Class::formal_template_parameters() const
	{
		if( !_template_paramaters_values )
			return nullptr;

		const Namespace & parent = parent_namespace();
		const ClassTemplate & class_template = static_cast<const ClassTemplate&>( parent );

		return &class_template.template_arguments_list();
	}

	// Class::base
	inline const Class * Class::base() const
	{		
		return static_cast<const Class *>( base_type( 0 ) );
	}

	// Class::inner_namespace
	inline const Namespace & Class::inner_namespace() const
	{
		return *_inner_namespace;
	}

	// Class::properties
	inline const PropertyList & Class::properties() const
	{
		return _properties;
	}

	// Class::actions
	inline const ActionList & Class::actions() const
	{
		return _actions;
	}

	// Class::events
	inline const EventDescriptorList & Class::events() const
	{
		return _events;
	}

	// Class::assign_properties
	template < size_t ITEM_COUNT > 
		void Class::assign_properties( const Property * (&items)[ ITEM_COUNT ] )
	{
		_properties.assign( items );

		_properties_assigned();
	}

	// Class::assign_properties
	template < size_t ITEM_COUNT > 
		void Class::assign_actions( const Action * (&items)[ ITEM_COUNT ] )
	{
		_actions.assign( items );
	}

	// Class::edit_actions
	inline ActionList & Class::edit_actions()
	{
		return _actions;
	}

	// Class::edit_events
	inline EventDescriptorList & Class::edit_events()
	{
		return _events;
	}


	// Class::identifier_property
	inline const Property * Class::identifier_property() const
	{
		return _identifier_property;
	}

} 

