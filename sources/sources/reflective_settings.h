
#pragma once

namespace reflective
{
	template < typename HASHER, typename STRING >
		class Identifier;

	template < typename UINT>
		class StringHasher;

	class PtrString;

	using SymbolName = Identifier< StringHasher<uint32_t>, PtrString >;

	template <typename TYPE>
		using Allocator = std::allocator<TYPE>;
}

#define REFLECTIVE_ASSERT_ENABLED 1

#define REFLECTIVE_ASSERT(i_value, i_error_message)		assert(i_value);
