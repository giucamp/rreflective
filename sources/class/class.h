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
	class Class;
	class ClassTemplate;

	// new_class<CLASS>()
	template <class CLASS> Class * new_class();

	// new_class<CLASS>( template_parameters[] )
	template <class CLASS, size_t TEMPLATE_PARAMETER_COUNT> Class * new_class(
		const TemplateParameter (&template_parameters)[ TEMPLATE_PARAMETER_COUNT ] );

	// new_class<CLASS>( parent_namespace_path, name )	
	template <class CLASS> Class * new_class( const StaticConstString & parent_namespace_path, const SymbolName & name );

	// new_class<CLASS>( namespace_path, name, template_parameters[] )
	template <class CLASS, size_t TEMPLATE_PARAMETER_COUNT> Class * new_class(
			const StaticConstString & parent_namespace_path,  const char * name,
			const TemplateParameter (&template_parameters)[ TEMPLATE_PARAMETER_COUNT ] );

	// new_class( namespace_path, name, template_parameters, template_parameter_count, alignment, size )
	Class * new_class( const StaticConstString & parent_namespace_path,
		const StaticConstString & name, size_t alignment, size_t size,
		const TemplateParameter * template_parameters, size_t template_parameter_count );

	// decorate_class_instance_name - decorates the name with the template argument values
	SymbolName decorate_class_instance_name( const StaticConstString & class_name, 
		const TemplateParameter * template_parameters, size_t template_parameter_count );

	// Class
	class Class : public Type
	{
	public:

					/* ctors */

		// Class( parent_namespace, name, alignment, size )		
		Class( Namespace & parent_namespace, const SymbolName & name, size_t alignment, size_t size );
		Class( const StaticConstString & parent_namespace_path, 
			const SymbolName & name, size_t alignment, size_t size );

		// Class( parent_class_template, name, alignment, size )
		Class( ClassTemplate * parent_class_template, SymbolName name, size_t alignment, size_t size );

		// destructor
		~Class();


				/* template parameters */

		bool is_template_instance() const; /* returns true if this class is an instance of a class template.
			This is the same as checking if class_template() returns nullptr. */

		const ClassTemplate * class_template() const; /* returns the class template this class belongs to, 
			if any.  Otherwise nullptr is returned. All the instances of a class template share the same 
			ClassTemplate object. */

		size_t template_parameter_count() const; /* returns the number of template parameters. If this
			class is not an instance of a class template this function always returns zero. */

		const ParameterList * formal_template_parameters() const;

		// properties
		const PropertyList & properties() const;
		template < size_t ITEM_COUNT > 
			void assign_properties( const Property * (&items)[ ITEM_COUNT ] );

		// actions
		const ActionList & actions() const;
		ActionList & edit_actions();
		template < size_t ITEM_COUNT > 
			void assign_actions( const Action * (&items)[ ITEM_COUNT ] );

		// events
		const EventDescriptorList & events() const;
		EventDescriptorList & edit_events();

		// base class
		const Class * base() const;	

		// namespace
		const Namespace & inner_namespace() const; /* the namespace that contains the enums and inner 
			classes defined inside this class. */

		const Property * identifier_property() const;

	private: // internal services
		
		void _properties_assigned();

	private: // data members		
		PropertyList _properties;
		ActionList _actions;
		EventDescriptorList _events;
		const void * _template_paramaters_values;
		Namespace * _inner_namespace;
		const Property * _identifier_property;
	};
}

namespace reflective_externals
{
	// reflection of reflective::Class
	reflective::Class * init_type(
		reflective::Class * null_pointer_1,
		reflective::Class * null_pointer_2 );
}


