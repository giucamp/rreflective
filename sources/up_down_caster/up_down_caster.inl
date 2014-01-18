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

#include "up_down_caster_fc.inl"
#include "up_down_caster_acc.inl"
#include "up_down_caster_offsettng.inl"
#include "up_down_caster_acc_diagnostic.inl"
#include "up_down_caster_offsettng_diagnostic.inl"

namespace reflective_externals
{

	// reflection of reflective::UpDownCaster<IMPLEMENTATION>
	template < reflective::UpDownCasterImplementation IMPLEMENTATION >
		reflective::Class * init_type(
			reflective::UpDownCaster<IMPLEMENTATION> * null_pointer_1,
			reflective::UpDownCaster<IMPLEMENTATION> * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef reflective::UpDownCaster<IMPLEMENTATION> ThisClass;
		
		static Class * class_object = nullptr;
		if( class_object != nullptr )
			return class_object;
		
		
		// template parameters
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "IMPLEMENTATION", IMPLEMENTATION ),
		};
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "UpDownCaster", template_parameters );
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eUngrabbedConstructorCopyAssignmentDestructor ) );
		
		// assign members
		
		// return type
		return class_object;
	}
	
} // namespace reflective_externals
