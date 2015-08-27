
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <string.h>
#include <type_traits>
#include <unordered_map>
#include <assert.h>

#include "reflective_common.h"

namespace reflective
{
				// address functions

	inline bool is_power_of_2(size_t i_number)
	{
		return (i_number & (i_number - 1)) == 0;
	}
	
	inline bool is_address_aligned(const void * i_address, size_t i_alignment)
	{
		assert( is_power_of_2(i_alignment) );
		return (reinterpret_cast<uintptr_t>(i_address) & (i_alignment - 1)) == 0;
	}

	template <typename TYPE>
		inline bool is_address_aligned(const TYPE * i_address)
	{
		return is_address_aligned(i_address, std::alignment_of<TYPE>::value);
	}

	template <typename TYPE>
		inline bool is_valid_range(const TYPE * i_objects_start, const TYPE * i_objects_end)
	{
		return i_objects_start <= i_objects_end &&
			is_address_aligned(i_objects_start) && is_address_aligned(i_objects_end) &&
			(reinterpret_cast<uintptr_t>(i_objects_end)-reinterpret_cast<uintptr_t>(i_objects_start)) % sizeof(TYPE) == 0;
	}
}
