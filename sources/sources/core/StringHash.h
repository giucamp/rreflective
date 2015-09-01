
#pragma once
#include "../reflective_common.h"

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

		ResultType operator() (const char * i_string) const
		{
			PtrString a(i_string);
			return this->operator()(PtrString(i_string));
		}

		template <typename STRING>
			ResultType operator() (const STRING & i_string) const
		{
			/*	djb2 - http://www.cse.yorku.ca/~oz/hash.html
				"this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. another version of
				this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number
				33 (why it works better than many other constants, prime or not) has never been adequately explained." */
			ResultType result = s_empty_hash;
			for (const auto c : i_string)
			{
				result = (result << 5) + (result + c);
			}
			return result;
		}
	};
}

