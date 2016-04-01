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
	template < typename UINT>
		class StringHasher;

	template <>
		class StringHasher<uint32_t>
	{
	public:

		using ResultType = uint32_t;

		static const uint32_t s_empty_hash = 5381;

		ResultType operator() (const char * i_null_terminated_string) const
		{
			return this->operator()(i_null_terminated_string, strlen(i_null_terminated_string));
		}

		ResultType operator() (StringView i_string) const
		{
			return this->operator()(i_string.data(), i_string.size());
		}

		ResultType operator() (const char * i_string, size_t i_length) const
		{
			/*	djb2 - http://www.cse.yorku.ca/~oz/hash.html Bernstein hash function */
			/*Hash hash = 5381;
			int c;
			while (c = *str++)
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
			*/

			uint32_t result = s_empty_hash;
			const char * str = i_string;
			size_t length = i_length;
			if (length >= 4) do {
				result = (result << 5) + (result + str[0]);
				result = (result << 5) + (result + str[1]);
				result = (result << 5) + (result + str[2]);
				result = (result << 5) + (result + str[3]);
				str += 4;
				length -= 4;
			} while (length >= 4);

			if (length) do {
				result = (result << 5) + (result + *str++);
			} while (--length);

			return result;
		}
	};
}

