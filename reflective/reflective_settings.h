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
#include <stdint.h>
#include <string>

namespace reflective
{
	template < typename HASHER, typename STRING > class Identifier; 
	template < typename UINT> class StringHasher;
	template <typename CHAR, typename CHAR_TRAITS > class BasicStringView;
		using SymbolName = Identifier< StringHasher<uint32_t>, BasicStringView<char, std::char_traits<char>> >;

	template <typename TYPE>
		using Allocator = std::allocator<TYPE>;

	#define REFLECTIVE_ENABLE_TESTING					1

	enum class InheritanceSupport
	{
		Functions,
		OffsettingAndFunctions,
		Offsetting,
	}; 
	static const InheritanceSupport s_inheritance_support = InheritanceSupport::Functions;

	#define REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE		1

	/** This function is called when the user requests an operation which is documented to be invalid,
		like quering the value of a property specifiing an owner object of the wrong type. */
	inline void on_invaid_operation(const char * i_error_message)
	{
		throw std::exception(i_error_message);
	}
}
#define REFLECTIVE_ASSERT(i_value, i_error_message)				if(!(i_value)) {__debugbreak();}
#define REFLECTIVE_TEST_ASSERT(i_value)							if(!(i_value)) {__debugbreak();}

#define REFLECTIVE_RUNTIME_CHECK(i_value, ...)					if(!(i_value)) {::reflective::on_invaid_operation("");}

/* Note: this assert does not allow lambda expressions inside the bool expression, as 
	the standard forbids lambda-expression in unevaluated expressions. */
#ifdef NDEBUG
	#define REFLECTIVE_INTERNAL_ASSERT(i_value)						(void)sizeof(i_value);
#else
	#define REFLECTIVE_INTERNAL_ASSERT(i_value)						if(!(i_value)) {__debugbreak();}
#endif