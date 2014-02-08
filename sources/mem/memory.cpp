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

#if REFLECTIVE_IS_DEBUG

	// mem::dbg_invalidate
	void mem::dbg_invalidate( volatile void * start_address, volatile void * end_address )
	{
		dbg_invalidate( const_cast<void *>( start_address ), const_cast<void *>( end_address ) );
	}

	// mem::dbg_invalidate
	void mem::dbg_invalidate( void * start_address, void * end_address )
	{
		// uintptr_t chek
		uintptr_t * aligned_word = reinterpret_cast<uintptr_t *>( 
			upper_align( start_address, sizeof( uintptr_t ) ) );

		uintptr_t * aligned_end = reinterpret_cast<uintptr_t *>( 
			lower_align( end_address, sizeof( uintptr_t ) ) );

		// prolog 
		uint8_t * cell = reinterpret_cast<uint8_t *>( start_address );
		while( cell < reinterpret_cast<uint8_t *>( aligned_word ) )
		{
			*cell ^= 30;
			cell++;
		}

		// aligned words
		if( aligned_word < aligned_end ) do {
			*aligned_word ^= 29;
		} while( ++aligned_word < aligned_end );

		// epilog
		cell = reinterpret_cast<uint8_t *>( aligned_word );
		while( cell < reinterpret_cast<uint8_t *>( end_address ) )
		{
			*cell ^= 31;
			cell++;
		}
	}

#endif // #if REFLECTIVE_IS_DEBUG

