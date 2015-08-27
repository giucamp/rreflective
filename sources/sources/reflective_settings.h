
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

#define constexpr



