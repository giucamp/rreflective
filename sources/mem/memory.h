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


namespace reflective
{
	template <typename TYPE>
		void default_destroyer( void * i_object )
	{
		static_cast<TYPE*>( i_object )->~TYPE();
	} 
}

#ifndef _INCLUDING_REFLECTIVE
	#error include "reflective.h" instead of including this file
#endif


#define REFLECTIVE_LIFO_NEW( TYPE, ... )				new( reflective_externals::mem_lifo_alloc( alignment_of(TYPE), sizeof(TYPE), &reflective::default_destructor_callback<TYPE> ) ) TYPE( __VA_ARGS__ )

template <class TYPE> void REFLECTIVE_LIFO_DELETE( TYPE * object )
{
	object->~TYPE();
	reflective_externals::mem_lifo_free( object );		
}

#define REFLECTIVE_DYN_NEW( TYPE, ... )				new( reflective_externals::mem_alloc( alignment_of(TYPE), sizeof(TYPE) ) ) TYPE( __VA_ARGS__ )

template <class TYPE> void REFLECTIVE_DELETE( TYPE * object )
{
	object->~TYPE();
	reflective_externals::mem_free( object );		
}

#define REFLECTIVE_NEW( TYPE, ... )				new( reflective_externals::mem_alloc( alignment_of(TYPE), sizeof(TYPE) ) ) TYPE( __VA_ARGS__ )

template <class UINT> bool is_integer_power_of_2( UINT integer );

// mem
namespace mem
{
	// memory address alignment
    bool is_aligned( const void * address, size_t boundary );
	template <class TYPE > TYPE upper_align( TYPE address, size_t boundary );
	template <class TYPE > TYPE upper_align( TYPE address, size_t boundary, size_t addend );
	template <class TYPE > TYPE lower_align( TYPE address, size_t boundary );
	template <class TYPE > void let_inner_aligned( TYPE * & start_address, TYPE * & end_address, size_t boundary );

	// address arithmetic
	void * address_inc( void * address );
	const void * address_inc( const void * address );
	void * address_add( void * address, uintptr_t positive_offset );
	const void * address_add( const void * address, uintptr_t positive_offset );
	template <class TYPE> TYPE * address_sub( TYPE * address, uintptr_t positive_offset );
	uintptr_t address_diff( const void * end_address, const void * start_address );

	// memory buffer manipulation
	bool are_overlapping( const void * first_start, const void * first_end, const void * second_start );
	void set( void * start_address, void * end_address, uint8_t value );
	bool test( const void * start_address, const void * end_address, uint8_t value );
	void copy( void * dest_start, void * dest_end, const void * source_start );
	void move( void * dest_start, void * dest_end, const void * source_start );
	bool equals( const void * first_start, const void * first_end, const void * second_start );
	int comp( const void * first_start, const void * first_end, const void * second_start );
	const void * find( const void * start_address, const void * end_address, uint8_t value );
	const void * find( const void * start_address, const void * end_address,
					  const void * target_start, const void * target_end );
	void invert( void * start_address, void * end_address );


	#if REFLECTIVE_IS_DEBUG
		void dbg_invalidate( void * start_address, void * end_address );
		void dbg_invalidate( volatile void * start_address, volatile void * end_address );
	#endif


} // namespace mem
