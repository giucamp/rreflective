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

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace reflective
{
				// address functions

	/** Returns true whether the given unsigned integer number is a power of 2 (1, 2, 4, 8, ...)
		@param i_number must be > 0, otherwise the behavior is undefined */
	inline bool is_power_of_2(size_t i_number)
	{
		REFLECTIVE_ASSERT(i_number > 0, "invalid argument");
		return (i_number & (i_number - 1)) == 0;
	}

	/** Returns true whether the given address has the specified alignment
		@param i_address address to be checked
		@i_alignment must be > 0 and a power of 2 */
	inline bool is_address_aligned(const void * i_address, size_t i_alignment)
	{
		REFLECTIVE_ASSERT(i_alignment > 0 && is_power_of_2(i_alignment), "the alignment is not a power of 2 or is zero");
		return (reinterpret_cast<uintptr_t>(i_address) & (i_alignment - 1)) == 0;
	}

	/** Returns true whether the given address has the alignment for the type it points to
		@param i_address address to be checked */
	template <typename TYPE>
		inline bool is_address_aligned(const TYPE * i_address)
	{
		return is_address_aligned(i_address, std::alignment_of<TYPE>::value);
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
		inline bool is_valid_range(const TYPE * i_objects_start, const TYPE * i_objects_end)
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
		const uintptr_t diff = reinterpret_cast<uintptr_t>(i_objects_end)-  reinterpret_cast<uintptr_t>(i_objects_start);
		if (diff % sizeof(TYPE) != 0)
		{
			return false;
		}
		return true;
	}

	template <typename TYPE>
		inline void dbg_object_validate( const TYPE & /*i_object*/ )
	{

	}
				
	template <typename TYPE>
		using unique_ptr = std::unique_ptr< TYPE >;

	using std::make_unique;
	
	template <typename TYPE>
		using vector = std::vector< TYPE > ;

	template <typename KEY, typename TYPE, typename HASHER = std::hash<KEY>, typename KEY_EQUAL_PRED = std::equal_to<KEY> >
		using unordered_map = std::unordered_map< KEY, TYPE, HASHER, KEY_EQUAL_PRED >;


	class Ext
	{
	public:
		
		template <typename CONTAINER, typename ELEMENT>
			static auto find(CONTAINER & i_container, ELEMENT && i_value) -> decltype(std::find(i_container.begin(), i_container.end(), i_value))
		{
			return std::find(i_container.begin(), i_container.end(), i_value);
		}

		template <typename CONTAINER, typename PREDICATE>
			static auto find_if(CONTAINER & i_container, PREDICATE && i_predicate) -> decltype(std::find_if(i_container.begin(), i_container.end(), i_predicate))
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate);
		}

		template <typename CONTAINER, typename ELEMENT>
			static bool contains(CONTAINER & i_container, ELEMENT && i_value)
		{
			return std::find(i_container.begin(), i_container.end(), i_value) != i_container.end();
		}

		template <typename CONTAINER, typename PREDICATE>
			static bool contains_if(CONTAINER & i_container, PREDICATE && i_predicate)
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate) != i_container.end();
		}
	};
}
