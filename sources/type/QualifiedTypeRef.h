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
		and volatileness for each indirection level. 
		The number of indirection levels is the nuber of '*' or '&' or '&&' appearing in the C++ declaration of
		the type. A non-pointer types has zero indirection levels, while a pointer to a pointer has 2 indirection levels.
		The leftmost type is first type appearing in the C++ declartion of the type, that is the type remaining after 
		stripping away all the cv-quaification, pointer and reference parts from the type.
		The primary type is the actual type of the first indirection level. For non-pointer types it is the same of the
		leftmost type. For pointer types is always equal to the result of get_type<void*>().
		The easest way fo get a QualifiedTypeRef is using the function get_qualified_type<TYPE>().

		type						|primary type	|leftmost type	|indirection levels		|constness word		|volatileness word	
		----------------------------|:-------------:|:-------------:|:---------------------:|:-----------------:|:------------------:
		float						|float			|float			|0						|0					|0
		volatile float				|float			|float			|0						|0					|1
		const float &				|void *			|float			|1						|3					|0
		float *const*volatile**		|void *			|float			|4						|8					|4
		
		*/
	class QualifiedTypeRef
	{
	public:

		static const uint32_t s_max_indirection_levels = 14;
		
		QualifiedTypeRef(const Type * i_leftmost_type, uint32_t i_indirection_levels = 0, uint32_t i_constness_word = 0, uint32_t i_volatileness_word = 0);

		QualifiedTypeRef(const QualifiedTypeRef & i_source) = default;

		QualifiedTypeRef & operator = (const QualifiedTypeRef & i_source) = default;

		/** Retrieves the primary type. */
		const Type * primary_type() const;
			
		const Type * leftmost_type() const								{ return m_leftmost_type; }

		uint32_t indirection_levels() const								{ return m_indirection_levels; }
		
		uint32_t constness_word() const									{ return m_constness_word; }

		uint32_t volatileness_word() const								{ return m_volatileness_word; }

		bool is_const(uint32_t i_indirection_level) const				{ return (m_constness_word & (1 << i_indirection_level)) != 0; }

		bool is_volatile(uint32_t i_indirection_level) const			{ return (m_constness_word & (1 << i_indirection_level)) != 0; }

		bool operator == (const QualifiedTypeRef & i_source) const;

		bool operator != (const QualifiedTypeRef & i_source) const		{ return !operator == (i_source); }

		template <typename OUT_STREAM>
			void to_string(OUT_STREAM & i_dest);

	private:
		const Type * m_leftmost_type;
		uint32_t m_indirection_levels : 4;
		uint32_t m_constness_word : 14;
		uint32_t m_volatileness_word : 14;
	};

	template <typename TYPE>
		QualifiedTypeRef get_qualified_type();
}
