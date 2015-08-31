
#pragma once
#include "core\Identifier.h"
#include "core\StringHash.h"
#include "core\PtrString.h"

namespace reflective
{
	using SymbolName = Identifier< StringHasher<uint32_t>, PtrString >;

	template <typename TYPE>
		using TypeAllocator = std::allocator<TYPE>;

	template <typename TYPE>
		using MapAllocator = std::allocator<TYPE>;
}

#define REFLECTIVE_ASSERT_ENABLED 1

#define REFLECTIVE_ASSERT(i_value, i_error_message)		assert(i_value);




