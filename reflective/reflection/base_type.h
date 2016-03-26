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
	template <InheritanceSupport IMPL> class BaseRelationship;

	using BaseType = BaseRelationship<s_inheritance_support>;

	template <>
		class BaseRelationship<InheritanceSupport::Functions> final
	{
	public:

		using CasterFuncPtr = void * (*)(void * i_source);
		
		BaseRelationship()
			: m_base_type(nullptr), m_is_virtual(false),
			  m_derived_to_base_func(nullptr), m_base_to_derived_func(nullptr)
				{ }

		BaseRelationship(const Type * i_base_type, bool i_virtual, CasterFuncPtr i_derived_to_base_func, CasterFuncPtr i_base_to_derived_func)
			: m_base_type(i_base_type), m_is_virtual(i_virtual),
			  m_derived_to_base_func(i_derived_to_base_func), m_base_to_derived_func(i_base_to_derived_func)
				{ }

		template <typename DERIVED, typename BASE>
			static BaseRelationship from_types()
		{
			return BaseRelationship(&get_naked_type<BASE>(), false, &cast_to_base<DERIVED, BASE>, &cast_to_derived<DERIVED, BASE>);
		}

		template <typename DERIVED, typename BASE>
			static BaseRelationship from_types_virtual()
		{
			return BaseRelationship(&get_naked_type<BASE>(), true, &cast_to_base<DERIVED, BASE>, &cast_virtual_to_derived<DERIVED, BASE>);
		}

		const Type * base_type() const { return m_base_type; }

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

		bool is_virtual() const
		{
			return m_is_virtual;
		}

	private:
		
		template <typename DERIVED, typename BASE>
			static void * cast_to_base(void * i_derived_ptr)
		{
			REFLECTIVE_ASSERT(i_derived_ptr != nullptr, "cast_to_base can't handle null pointers");

			auto derived_ptr = static_cast<DERIVED*>(i_derived_ptr);
			dbg_object_validate(*derived_ptr);

			auto base_ptr = static_cast<BASE*>(derived_ptr);
			dbg_object_validate(*base_ptr);
			return base_ptr;
		}

		template <typename DERIVED, typename BASE>
			static void * cast_to_derived(void * i_base_ptr)
		{
			REFLECTIVE_ASSERT(i_base_ptr != nullptr, "cast_to_derived can't handle null pointers");

			auto base_ptr = static_cast<BASE*>(i_base_ptr);
			dbg_object_validate(*base_ptr);

			auto derived_ptr = static_cast<DERIVED*>(base_ptr);
			dbg_object_validate(*derived_ptr);
			return derived_ptr;
		}

		template <typename DERIVED, typename BASE>
			static void * cast_virtual_to_derived(void * i_base_ptr)
		{
			REFLECTIVE_ASSERT(i_base_ptr != nullptr, "cast_to_derived can't handle null pointers");

			auto base_ptr = static_cast<BASE*>(i_base_ptr);
			dbg_object_validate(*base_ptr);

			auto derived_ptr = dynamic_cast<DERIVED*>(base_ptr);
			dbg_object_validate(*derived_ptr);
			return derived_ptr;
		}

	private:
		const Type * m_base_type;
		bool m_is_virtual;
		CasterFuncPtr m_derived_to_base_func;
		CasterFuncPtr m_base_to_derived_func;
	};
}

