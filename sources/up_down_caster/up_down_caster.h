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

#ifndef REFLECTIVE_UPDOWN_CASTER_DEFAULT_IMPL
	#error REFLECTIVE_UPDOWN_CASTER_DEFAULT_IMPL must be defined with a member of UpDownCasterImplementation
#endif

namespace reflective
{
	// UpDownCasterImplementation
	enum UpDownCasterImplementation
	{
		UpDownCasterFullCompliance,
		UpDownCasterAllowConstCast,
		UpDownCasterAllowConstCast_Diagnostic,
		UpDownCasterOffsetting,
		UpDownCasterOffsetting_Diagnostic,
	}; 
	/* Determines how UpDownCaster will cast pointers from\to base classes. The possible values are:

		- UpDownCasterFullCompliance - no assumption on the class layout and no const_cast
			is done: four template function instance will be generated for every hineritance
			relationship. This is the most expensive choise.

		- UpDownCasterAllowConstCast - no assumption on the class layout, but a const_cast
			is used to reduce the needed functions from 4 to 2.

		- UpDownCasterAllowConstCast_Diagnostic - use both the techninque used by
			UpDownCasterFullCompliance and UpDownCasterAllowConstCast, and compare the results,
			reporting differences with an assert. Useful only for debug purpose.

		- UpDownCasterOffsetting - the up and down casting between two releated types is
			assumed to offset the pointer by a constant value, which is computed casting
			a constant pointer from the base\derived class, and then subtracting them. This
			techninque is the most efficient, because reduces the code size and no function
			call is performed to cast.

		- UpDownCasterOffsetting_Diagnostic - use both the techninque used by
			UpDownCasterFullCompliance and UpDownCasterOffsetting, and compare the results,
			reporting differences with an assert. Useful only for debug purpose. */

	// UpDownCaster< UpDownCasterImplementation >
	template < UpDownCasterImplementation IMPLEMENTATION = REFLECTIVE_UPDOWN_CASTER_DEFAULT_IMPL >
		class UpDownCaster
	{
	public:
		// constructor
		UpDownCaster();

		// from_types< BASE_CLASS, THIS_CLASS >
		template < class BASE_CLASS, class THIS_CLASS >
			static UpDownCaster from_types();

		// from_types_only_to_base< BASE_CLASS, THIS_CLASS >
		template < class BASE_CLASS, class THIS_CLASS >
			static UpDownCaster from_types_only_to_base();

		// pointer_to_base_type \ pointer_from_base_type
		void * pointer_to_base_type( void * pointer ) const;
		void * pointer_from_base_type( void * pointer ) const;
		const void * pointer_to_base_type( const void * pointer ) const;
		const void * pointer_from_base_type( const void * pointer ) const;
	};


	// specializations forwards
	template <> class UpDownCaster<UpDownCasterFullCompliance>;
	template <> class UpDownCaster<UpDownCasterAllowConstCast>;
	template <> class UpDownCaster<UpDownCasterAllowConstCast_Diagnostic>;
	template <> class UpDownCaster<UpDownCasterOffsetting>;
	template <> class UpDownCaster<UpDownCasterOffsetting_Diagnostic>;

} // namespace reflective

#include "up_down_caster_fc.h"
#include "up_down_caster_acc.h"
#include "up_down_caster_offsettng.h"
#include "up_down_caster_acc_diagnostic.h"
#include "up_down_caster_offsettng_diagnostic.h"

namespace reflective_externals
{

	// reflection of reflective::UpDownCaster<IMPLEMENTATION>
	template < reflective::UpDownCasterImplementation IMPLEMENTATION >
		void init_type( reflective::Type * volatile * o_result,
			reflective::UpDownCaster<IMPLEMENTATION> * null_pointer_1,
			reflective::UpDownCaster<IMPLEMENTATION> * null_pointer_2 );
	
} // namespace reflective_externals
