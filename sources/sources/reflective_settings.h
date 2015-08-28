
#pragma once
#include "core\Identifier.h"
#include "core\StringHash.h"
#include "core\PtrString.h"

namespace reflective
{
	using size_t = size_t;

	using indirection_levels_t = uint32_t;
	using indirection_mask_t = uint32_t;

	using SymbolName = Identifier< StringHasher<uint32_t>, PtrString >;
}

#define REFLECTIVE_ASSERT_ENABLED 1

#define REFLECTIVE_ASSERT(i_value, i_error_message)		assert(i_value);

#define constexpr



