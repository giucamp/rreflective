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

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	template <UpDownCasterImplementation IMPL = s_upDownCasterImplementation>
		class UpDownCaster;

	template <>
		class UpDownCaster<UpDownCasterImplementation::Functions>
	{
	public:

		using Caster = void * (*)(void * i_source);
		
		UpDownCaster()
			: m_derived_to_base_func(nullptr), m_base_to_derived_func(nullptr)
				{ }

		UpDownCaster(Caster i_derived_to_base_func, Caster i_base_to_derived_func)
			: m_derived_to_base_func(i_derived_to_base_func), m_base_to_derived_func(i_base_to_derived_func)
				{ }

		template <typename BASE, typename DERIVED>
			static UpDownCaster from_types()
		{
			return UpDownCaster(&cast_to_base<BASE, DERIVED>, &cast_to_derived<BASE, DERIVED>);
		}

		void * derived_to_base(void * i_derived_ptr) const
		{
			return (*m_derived_to_base_func)(i_derived_ptr);
		}

		void * base_to_derived(void * i_base_ptr) const
		{
			return (*m_base_to_derived_func)(i_base_ptr);
		}

		const void * derived_to_base(const void * i_derived_ptr) const
		{
			/* the implementation uses a const_cast to avoid having a separate pair of 
				functions fo each base type to support const pointers. */
			return (*m_derived_to_base_func)(const_cast<void*>( i_derived_ptr ) );
		}

		const void * base_to_derived(const void * i_base_ptr) const
		{
			/* the implementation uses a const_cast to avoid having a separate pair of 
				functions fo each base type to support const pointers. */
			return (*m_base_to_derived_func)(const_cast<void*>(i_base_ptr));
		}

	private:
		
		template <typename BASE, typename DERIVED>
			static void * cast_to_base(void * i_derived_ptr)
		{
			REFLECTIVE_ASSERT(i_derived_ptr != nullptr, "cast_to_base can't handle nul pointers");
			DERIVED * derived_ptr = static_cast<DERIVED*>(i_derived_ptr);
			return static_cast<BASE*>(derived_ptr);
		}

		template <typename BASE, typename DERIVED>
			static void * cast_to_derived(void * i_base_ptr)
		{
			REFLECTIVE_ASSERT(i_base_ptr != nullptr, "cast_to_derived can't handle nul pointers");
			BASE * base_ptr = static_cast<BASE*>(i_base_ptr);
			return static_cast<DERIVED*>(base_ptr);
		}

	private:
		
		Caster m_derived_to_base_func;
		Caster m_base_to_derived_func;
	};
}

