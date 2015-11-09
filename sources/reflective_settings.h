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
	using SymbolName = Identifier< StringHasher<uint32_t>, StringView >;

	template <typename TYPE>
		using Allocator = std::allocator<TYPE>;

	static const InheritanceSupport s_inheritance_support = InheritanceSupport::Functions;

	static const size_t s_global_registry_reserve = 512;

	#define REFLECTIVE_ENABLE_TESTING					1

	#define REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE		1
}
	
#define REFLECTIVE_DEBUG 1

#define REFLECTIVE_ASSERT_ENABLED 1

#define REFLECTIVE_ASSERT(i_value, i_error_message)				if(!(i_value)) {__debugbreak();}
#define REFLECTIVE_TEST_ASSERT(i_value)							if(!(i_value)) {__debugbreak();}
#define REFLECTIVE_INTERNAL_ASSERT(i_value)						if(!(i_value)) {__debugbreak();}
