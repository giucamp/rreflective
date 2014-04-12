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
	class ClassTemplate;

	// get_class_template
	ClassTemplate * create_class_template( const StaticConstString & parent_namespace_path,
		const StaticConstString & name, const TemplateParameter * template_parameters, size_t template_parameters_count );
			// the template parameters are cloned, so the passed array can be destroyed after the call 

	// ClassTemplate
	class ClassTemplate : public Namespace
	{
	public:

		// constructor
		ClassTemplate( const StaticConstString & parent_namespace_path, const SymbolName & name );

		// template_arguments_list
		const ParameterList & template_arguments_list() const;
		ParameterList & edit_template_arguments_list();

	private: // data members
		ParameterList _template_arguments_list;
	};

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::ClassTemplate
	void init_type( reflective::Type * volatile * o_result,
		reflective::ClassTemplate * null_pointer_1,
		reflective::ClassTemplate * null_pointer_2 );

} // namespace reflective_externals
