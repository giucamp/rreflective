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
	/** Lightweight object holding a pointer to a type, a number of indirection levels, and the constness 
		and volatileness for each indirection level. A is composed by:
			- The **number of indirection** levels is the nuber of '*' or '&' or '&&' appearing in the C++ declaration of
			   the type. A non-pointer types has zero indirection levels, while a pointer to a pointer has 2 indirection levels.
			- The **primary type** is the actual type of the first indirection level. For non-pointer types it is the same of the
			   leftmost type. For pointer types is always equal to the result of get_type<void*>(). If an object of has to be 
			   constructed, copied, or assigned, the primary type is what matters.
			- The **leftmost type** is first type appearing in the C++ declartion of the type, that is the type remaining after 
			  stripping away all the cv-quaification, pointer and reference parts from the type.
		
		The easest way fo get a QualifiedTypePtr is using the function get_qualified_type<TYPE>().

		type						|primary type	|leftmost type	|indirection levels		|constness word		|volatileness word	
		----------------------------|:-------------:|:-------------:|:---------------------:|:-----------------:|:------------------:
		float						|float			|float			|0						|0					|0
		volatile float				|float			|float			|0						|0					|1
		const float &				|void *			|float			|1						|3					|0
		float *const*volatile**		|void *			|float			|4						|8					|4
		


		*/
	class QualifiedTypePtr
	{
	public:

		/** Maximum indirection level that this class can handle. This is 14 for if uintptr_t is 32-bit wide or smaller, 28 otherwise.
			The global function get_qualified_type<TYPE>() checks this imit at compile-time (with a static_assert). */
		static const size_t s_max_indirection_levels = std::numeric_limits<uintptr_t>::digits <=32 ? 14 : 28;
		
		QualifiedTypePtr(const Type * i_leftmost_type, size_t i_indirection_levels = 0, size_t i_constness_word = 0, size_t i_volatileness_word = 0);

		QualifiedTypePtr(const QualifiedTypePtr & i_source) = default;

		QualifiedTypePtr & operator = (const QualifiedTypePtr & i_source) = default;

		bool is_empty() const											{ return m_leftmost_type == nullptr; }

		/** Retrieves the primary type. */
		const Type * primary_type() const;
			
		const Type * leftmost_type() const								{ return m_leftmost_type; }

		size_t indirection_levels() const								{ return m_indirection_levels; }
		
		size_t constness_word() const									{ return m_constness_word; }

		size_t volatileness_word() const								{ return m_volatileness_word; }

		bool is_const(size_t i_indirection_level) const					{ return (m_constness_word & (static_cast<uintptr_t>(1) << i_indirection_level)) != 0; }

		bool is_volatile(size_t i_indirection_level) const				{ return (m_volatileness_word & (static_cast<uintptr_t>(1) << i_indirection_level)) != 0; }

		bool operator == (const QualifiedTypePtr & i_source) const;

		bool operator != (const QualifiedTypePtr & i_source) const		{ return !operator == (i_source); }

		void * full_indirection(void * i_object );

		template <typename OUT_STREAM>
			void to_string(OUT_STREAM & i_dest) const;

	private:
		const Type * m_leftmost_type;
		uintptr_t m_indirection_levels : (std::numeric_limits<uintptr_t>::digits - s_max_indirection_levels * 2);
		uintptr_t m_constness_word : s_max_indirection_levels;
		uintptr_t m_volatileness_word : s_max_indirection_levels;
		static_assert(std::numeric_limits<uintptr_t>::radix == 2, "uintptr_t is expected to be binary");
	};

	template <typename TYPE>
		QualifiedTypePtr get_qualified_type();
}
