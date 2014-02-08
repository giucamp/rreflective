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

// is_integer_power_of_2( integer )
template <class UINT>
	inline bool is_integer_power_of_2( UINT integer )
{
	return ( integer & (integer >> 1) ) == 0;
}

// mem
namespace mem
{
				/**** address alignment ****/

	// is_aligned
	inline bool is_aligned( const void * address, size_t boundary )
	{
		REFLECTIVE_ASSERT( is_integer_power_of_2( boundary ) ); // boundary must be self-aligned
		const size_t mask = boundary - 1;		
		return ( ( reinterpret_cast<uintptr_t>( address ) ) & mask) == 0;
	}

	// upper_align
	template <class TYPE >
		inline TYPE upper_align( TYPE address, size_t boundary )
	{
		REFLECTIVE_ASSERT( boundary > 0 );
		const size_t mask = boundary - 1;		
		REFLECTIVE_ASSERT( is_integer_power_of_2( boundary ) ); // boundary must be self-aligned
		return (TYPE)( ( (uintptr_t)( address ) + mask) & ~mask );
	}

	// upper_align
	template <class TYPE >
		inline TYPE upper_align( TYPE address, size_t boundary, size_t addend )
	{
		REFLECTIVE_ASSERT( boundary > 0 );
		const size_t mask = boundary - 1;
		REFLECTIVE_ASSERT( is_integer_power_of_2( boundary ) ); // boundary must be self-aligned
		return (TYPE)( ( reinterpret_cast<uintptr_t >( address ) + (addend + mask) ) & ~mask );
	}

	// lower_align
	template <class TYPE >
		inline TYPE lower_align( TYPE address, size_t boundary )
	{
		const size_t mask = boundary - 1;
		REFLECTIVE_ASSERT( is_integer_power_of_2( boundary ) ); // boundary must be self-aligned
		return (TYPE)( reinterpret_cast<uintptr_t >( address ) & ~mask );
	}



			/**** buffer manipulation ****/

	// mem::set
	inline void set( void * start_address, void * end_address, uint8_t value )
	{
		REFLECTIVE_ASSERT( start_address <= end_address );
		memset( start_address, value, address_diff(end_address, start_address ) );
	}


	// mem::copy
	inline void copy( void * dest_start, void * dest_end, const void * source_start )
	{
		#if REFLECTIVE_IS_DEBUG
			REFLECTIVE_ASSERT( dest_start <= dest_end );

			const uintptr_t length = (uintptr_t)( ((uint8_t*)dest_end) - ((uint8_t*)dest_start) );
			const uint8_t * source_end = ((uint8_t*)source_start) + length;			

			REFLECTIVE_ASSERT( !are_overlapping( dest_start, dest_end, source_start ) );
			REFLECTIVE_UNUSED( source_end );
		#endif

		const size_t size = address_diff( dest_end, dest_start );
		memcpy( dest_start, source_start, size );
	}

	// mem::move
	inline void move( void * dest_start, void * dest_end, const void * source_start )
	{
		REFLECTIVE_ASSERT( dest_start <= dest_end );
		const uintptr_t length = address_diff( dest_end, dest_start );
		memmove( dest_start, source_start, length );
	}

	// mem::comp
	inline int comp( const void * first_start, const void * first_end, const void * second_start )
	{
		REFLECTIVE_ASSERT( first_start <= first_end );
		const uintptr_t length = address_diff( first_end, first_start );
		return memcmp( first_start, second_start, length );
	}

	// mem::find
	inline const void * find( const void * start_address, const void * end_address, uint8_t value )
	{
		REFLECTIVE_ASSERT( start_address <= end_address );
		return memchr( start_address, value, address_diff( end_address, start_address ) );
	}


	// are_overlapping
	inline bool are_overlapping( const void * first_start, const void * first_end, const void * second_start )
	{
		if( first_start < second_start )
			return first_end > second_start;
		else
		{
			const uintptr_t length = address_diff( first_end, first_start );
			return address_add( second_start, length ) > first_start;
		}
	}

	// address_inc
	inline void * address_inc( void * address )
	{
		return (void *)( reinterpret_cast<uintptr_t >( address ) + 1 );
	}
	inline const void * address_inc( const void * address )
	{
		return (const void *)( reinterpret_cast<uintptr_t >( address ) + 1 );
	}

	// address_add
	inline const void * address_add( const void * address, uintptr_t positive_offset )
	{
		const uintptr_t result = reinterpret_cast<uintptr_t >( address ) + positive_offset;
		REFLECTIVE_ASSERT( result >= reinterpret_cast<uintptr_t >( address ) );
		return (const void *)result;
	}

	// address_add
	inline void * address_add( void * address, uintptr_t positive_offset )
	{
		const uintptr_t result = reinterpret_cast<uintptr_t >( address ) + positive_offset;
		REFLECTIVE_ASSERT( result >= reinterpret_cast<uintptr_t >( address ) );
		return (void *)result;
	}

	// address_sub
	template <class TYPE> 
		inline TYPE * address_sub( TYPE * address, uintptr_t positive_offset )
	{
		REFLECTIVE_ASSERT( reinterpret_cast<uintptr_t >( address ) >= positive_offset );
		const uintptr_t result = reinterpret_cast<uintptr_t >( address ) - positive_offset;
		return (TYPE *)result;
	}

	// address_diff
	inline uintptr_t address_diff( const void * end_address, const void * start_address )
	{
		REFLECTIVE_ASSERT( end_address >= start_address );
		const uintptr_t int_start_address = reinterpret_cast< uintptr_t >( start_address );
		const uintptr_t int_end_address = reinterpret_cast< uintptr_t >( end_address );
		return int_end_address - int_start_address;
	}


} // namespace mem


