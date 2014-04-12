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
	// create_class_template
	ClassTemplate * create_class_template( const StaticConstString & parent_namespace_path,
		const StaticConstString & name, const TemplateParameter * template_parameters, size_t template_parameters_count )
	{
		// access (or create_instance) parent namespace
		Namespace * parent_namespace = Namespace::edit_global().find_or_add_child_namespace( parent_namespace_path );
		if( parent_namespace )
		{
			Namespace * exiting_namespace = parent_namespace->find_child_namespace( SymbolName( name ) );
			if( exiting_namespace )
			{
				ClassTemplate * class_template = static_cast<ClassTemplate *>( exiting_namespace );	
				//REFLECTIVE_ASSERT( type_of( *class_template ) <= safe_get_type<ClassTemplate>() );
				return class_template;
			}
		}

		// create_instance class template
		ClassTemplate * class_template = REFLECTIVE_LIFO_NEW( ClassTemplate, parent_namespace_path, name );

		// create_instance parameters
		const Parameter * * parameters = static_cast<const Parameter * *>( 
				reflective_externals::mem_lifo_alloc( 
				alignment_of( Parameter * ), template_parameters_count * sizeof( Parameter * ) ) );
		size_t parameter_index = 0; if( template_parameters_count > 0 ) do {

			const TemplateParameter & source_parameter = template_parameters[ parameter_index ];

			parameters[ parameter_index ] = new_parameter( 
				source_parameter.name(), source_parameter.type(), source_parameter.qualification() );

		} while( ++parameter_index < template_parameters_count );

		class_template->edit_template_arguments_list().assign( parameters, template_parameters_count );

		return class_template;
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::ClassTemplate
	void init_type( reflective::Type * volatile * o_result,
		reflective::ClassTemplate * null_pointer_1,
		reflective::ClassTemplate * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ClassTemplate ThisClass;
		typedef Namespace BaseClass;
	
		if( *o_result != nullptr )
			return;
	
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "ClassTemplate" );
		*o_result = class_object;
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eCopyAssignmentDestructor ) );
	
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "template_arguments_list", &ThisClass::template_arguments_list ),
		};
	
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "edit_template_arguments_list", &ThisClass::edit_template_arguments_list ),
		};
	
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
	}

} // namespace reflective_externals

