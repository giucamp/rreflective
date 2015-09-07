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
	template <typename TYPE>
		class StaticQualification;

	/** generic template for non pointer, non const and non volatile types */
	template <typename TYPE> class StaticQualification
	{
	public:

		using UnderlyingType = TYPE; /* it's the final type, that is the type with maximum
			indirection level. For example the following types have all 'int' as final 
			type: 'int', 'int &', 'int * * * const * const &'. 
			C++ language remainder: a reference is like a const pointer, that is 'int &' is 
			the same as 'int * const', except that the first exposes directly its last 
			level of indirection. */

		static const size_t s_indirection_levels = 0; /* the number of indirection levels,
			which is zero for non-pointers, 1 for pointers, 2 for pointers to pointers, 
			and so on. */

		static const size_t s_constness_word = 0; /* specifies, for each indirection
				level,  the constness of the type. 
			- 'int', 'int**', "int&": no const is present, constness_word is zero.
			- 'int * * const': the first indirection level is const (that is, the
				pointer cannot be assigned after initialization), so the 
				constness_word is 1 (only bit 0 set).
			- 'const int ********': only the last indirection level (8) is const,
				so the 8th bit of the constness_word is 1 (constness_word=256).
			C++ language remainder: the const keyword can be prefix or postfix to 
				the type it applies to: 'const int *' and 'int const *' are the 
				same declarations. */

		static const size_t s_volatileness_word = 0;

		static const bool s_is_lvalue_reference = false;
		static const bool s_is_rvalue_reference = false;
	
	private:
		
	};

	/** partial specialization for lvalue reference types (they are threated like const pointers) */
	template <typename TYPE> class StaticQualification<TYPE&>
	{
	public:
		using UnderlyingType = TYPE;
		static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
		static const size_t s_constness_word = 1 | (StaticQualification<TYPE>::s_constness_word << 1);
		static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
		static const bool s_is_lvalue_reference = true;
		static const bool s_is_rvalue_reference = false;
	};

	/** partial specialization for rvalue reference types */
	template <typename TYPE> class StaticQualification<TYPE&&>
	{
	public:
		using UnderlyingType = TYPE;
		static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
		static const size_t s_constness_word = 1 | (StaticQualification<TYPE>::s_constness_word << 1);
		static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
		static const bool s_is_lvalue_reference = false;
		static const bool s_is_rvalue_reference = true;
	};

	/** partial specialization for pointer types */
	template <typename TYPE> class StaticQualification<TYPE*>
	{
	public:
		using UnderlyingType = TYPE;
		static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
		static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word << 1;
		static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
		static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
		static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
	};

	/** partial specialization for const types */
	template <typename TYPE> class StaticQualification<const TYPE>
	{
	public:
		using UnderlyingType = TYPE;
		static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels;
		static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word | 1;
		static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word;
		static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
		static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
	};

	/** partial specialization for volatile types */
	template <typename TYPE> class StaticQualification<volatile TYPE>
	{
	public:
		using UnderlyingType = TYPE;
		static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels;
		static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word;
		static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word | 1;
		static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
		static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
	};
}
