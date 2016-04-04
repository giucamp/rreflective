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
#include "..\reflective_settings.h"

#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
	#define REFLECTIVE_CONSTEXPR
	#define REFLECTIVE_NOEXCEPT
	#define REFLECTIVE_NOEXCEPT_V(value)
	#define REFLECTIVE_ASSERT_NOEXCEPT(expr)
#else
	#define REFLECTIVE_CONSTEXPR					constexpr
	#define REFLECTIVE_NOEXCEPT						noexcept
	#define REFLECTIVE_NOEXCEPT_V(value)			noexcept(value)
	#define REFLECTIVE_ASSERT_NOEXCEPT(expr)		static_assert(noexcept(expr), "The expression " #expr " is required not be noexcept");
#endif


namespace reflective
{
				// address functions

	/** Returns true whether the given unsigned integer number is a power of 2 (1, 2, 4, 8, ...)
		@param i_number must be > 0, otherwise the behavior is undefined */
	inline bool is_power_of_2(size_t i_number) REFLECTIVE_NOEXCEPT
	{
		REFLECTIVE_ASSERT(i_number > 0, "invalid argument");
		return (i_number & (i_number - 1)) == 0;
	}

	/** Returns true whether the given address has the specified alignment
		@param i_address address to be checked
		@i_alignment must be > 0 and a power of 2 */
	inline bool is_address_aligned(const void * i_address, size_t i_alignment) REFLECTIVE_NOEXCEPT
	{
		REFLECTIVE_ASSERT(i_alignment > 0 && is_power_of_2(i_alignment), "the alignment is not a power of 2 or is zero");
		return (reinterpret_cast<uintptr_t>(i_address) & (i_alignment - 1)) == 0;
	}

	/** Returns true whether the given address has the alignment for the type it points to
		@param i_address address to be checked */
	template <typename TYPE>
		inline bool is_address_aligned(const TYPE * i_address) REFLECTIVE_NOEXCEPT
	{
		return is_address_aligned(i_address, std::alignment_of<TYPE>::value);
	}

	template <typename TYPE>
		inline TYPE * address_upper_align(void * i_address) REFLECTIVE_NOEXCEPT
	{
		const size_t alignment_mask = std::alignment_of<TYPE>::value - 1;
		return reinterpret_cast<TYPE*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
	}

	inline void * address_upper_align(void * i_address, size_t i_alignment) REFLECTIVE_NOEXCEPT
	{
		REFLECTIVE_INTERNAL_ASSERT(is_power_of_2(i_alignment));
		const size_t alignment_mask = i_alignment - 1;
		return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
	}

	template <typename TYPE>
		inline TYPE * address_add(TYPE * i_address, size_t i_offset) REFLECTIVE_NOEXCEPT
	{
		return reinterpret_cast<TYPE*>(reinterpret_cast<uintptr_t>(i_address) + i_offset);
	}

	/** Returns true whether the given pair of pointers enclose a valid array of objects of the type. This function is intended to validate
			an input array.
		@param i_objects_start inclusive lower bound of the array
		@param i_objects_end exclusive upper bound of the array
		@return true if and only if all the following conditions are true:
			- i_objects_start <= i_objects_end
			- the difference (in bytes) between i_objects_end and i_objects_start is a multiple of the size of TYPE
			- both i_objects_start and i_objects_end respects the alignment for TYPE. */
	template <typename TYPE>
		inline bool is_valid_range(const TYPE * i_objects_start, const TYPE * i_objects_end) REFLECTIVE_NOEXCEPT
	{
		if (i_objects_start > i_objects_end)
		{
			return false;
		}
		if( !is_address_aligned(i_objects_start) )
		{
			return false;
		}
		if( !is_address_aligned(i_objects_end) )
		{
			return false;
		}
		const uintptr_t diff = reinterpret_cast<uintptr_t>(i_objects_end) -  reinterpret_cast<uintptr_t>(i_objects_start);
		if (diff % sizeof(TYPE) != 0)
		{
			return false;
		}
		return true;
	}

	struct AlignmentHeader
	{
		void * m_block;
	};

	template <typename ALLOCATOR>
		void * aligned_alloc(ALLOCATOR & i_allocator, size_t i_size, size_t i_alignment)
	{
		REFLECTIVE_INTERNAL_ASSERT(is_power_of_2(i_alignment));

		if (i_alignment <= std::alignment_of<void*>::value)
		{
			typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<void *> other_alloc(i_allocator);
			return other_alloc.allocate((i_size + sizeof(void*) - 1) / sizeof(void*));
		}
		else
		{
			const size_t extra_size = (i_alignment >= sizeof(AlignmentHeader) ? i_alignment : sizeof(AlignmentHeader));
			const size_t actual_size = i_size + extra_size;

			typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(i_allocator);
			void * complete_block = char_alloc.allocate(actual_size);
			auto uint_address = reinterpret_cast<uintptr_t>(complete_block);

			uint_address += extra_size;
			uint_address &= ~(i_alignment - 1);

			AlignmentHeader * header = reinterpret_cast<AlignmentHeader*>(uint_address) - 1;
			header->m_block = complete_block;

			return reinterpret_cast<void*>(uint_address);
		}
	}

	template <typename ALLOCATOR>
		void aligned_free(ALLOCATOR & i_allocator, void * i_block, size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
	{
		if (i_block != nullptr)
		{
			if (i_alignment <= std::alignment_of<void*>::value)
			{
				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<void *> other_alloc(i_allocator);
				other_alloc.deallocate(static_cast<void**>(i_block), (i_size + sizeof(void*) - 1) / sizeof(void*));
			}
			else
			{
				const size_t extra_size = (i_alignment >= sizeof(AlignmentHeader) ? i_alignment : sizeof(AlignmentHeader));
				const size_t actual_size = i_size + extra_size;

				AlignmentHeader * header = static_cast<AlignmentHeader*>(i_block) - 1;

				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(i_allocator);
				char_alloc.deallocate(static_cast<char*>(header->m_block), actual_size);
			}
		}
	}

} // namespace reflective
