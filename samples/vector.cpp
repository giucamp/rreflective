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

#include "sample_common.h"

template < class COORD_TYPE, int DIM > 
	class Vector
{
public:

	Vector()
	{
		// ...
	}

	void normalize()
	{
		// ...
	}

	COORD_TYPE get_module() const
	{
		return 0;
	}

private:
	COORD_TYPE _coord[ DIM ];
};


namespace reflective_externals
{
	template < class COORD_TYPE, int DIM > 
		reflective::Class * init_type(
			Vector< COORD_TYPE, DIM > * null_pointer_1,
			Vector< COORD_TYPE, DIM > * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef Vector< COORD_TYPE, DIM > ThisClass;
		
		// template parameters
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "COORD_TYPE", safe_get_qualified_type< COORD_TYPE >() ),
			TemplateParameter( "DIM", DIM ),
		};

		// class object
		Class * class_object = new_class<ThisClass>( template_parameters );
		class_object->set_no_base_type();
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "module", &ThisClass::get_module ),
		};
		
		// actions
		const Action * actions[] = 
		{
			new_action<ThisClass>( "normalize", &ThisClass::normalize ),
		};
		
		// assign members
		class_object->assign_properties( properties );
		class_object->assign_actions( actions );
		
		// return type
		return class_object;
	}
		
} // namespace reflective_externals



int vector_main()
{
	using namespace reflective;

	typedef Vector< double, 3 > Vector3;
	const ParameterList & parameters = get_class< Vector3 >()->class_template()->template_arguments_list();

	// write the template formal parameters
	char chars[ 1024 ];
	ToStringBuffer char_buffer( chars );
	for( Iterator< const Parameter > parameter_iterator( parameters );
		parameter_iterator; parameter_iterator++ )
	{
		to_string( char_buffer, *parameter_iterator );
		char_buffer.append( '\n' );
	}

	// print on the ouput the text
	printf( chars );

	return 0;
}
