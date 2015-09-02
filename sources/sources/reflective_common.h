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
	template <typename TYPE>
		using vector = std::vector< TYPE, Allocator<TYPE> > ;

				// address functions

	inline bool is_power_of_2(size_t i_number)
	{
		return (i_number & (i_number - 1)) == 0;
	}
	
	inline bool is_address_aligned(const void * i_address, size_t i_alignment)
	{
		REFLECTIVE_ASSERT( is_power_of_2(i_alignment), "the alignment is not a power of 2" );
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
			(reinterpret_cast<uintptr_t>(i_objects_end) - reinterpret_cast<uintptr_t>(i_objects_start)) % sizeof(TYPE) == 0;
	}
}
