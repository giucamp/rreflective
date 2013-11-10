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

#include "lifo_allocator.h"
#include "windows.h"
#include "stdio.h"
#undef min
#undef max


namespace reflective_externals
{
	static LifoAllocator & lifo_allocator()
	{
		static LifoAllocator * s_lifo_allocator = nullptr;
		if( s_lifo_allocator != nullptr )
			return *s_lifo_allocator;

		{
			size_t buffer_length = 1024 * 1024;
			void * buffer = malloc( buffer_length );
			static LifoAllocator _s_lifo_allocator( buffer, buffer_length );
			s_lifo_allocator = &_s_lifo_allocator;
			return *s_lifo_allocator;
		}		
	}

	void output_debug_string( const char * expression )
	{
		printf( expression );
	}

	void debug_break()
	{
		__asm int 3
	}

	// assert
	void assert_failure( const char * expression )
	{
		output_debug_string( "assert failure: " );
		output_debug_string( expression );
		output_debug_string( "\n" );
 		__asm int 3
	}


	void on_implicit_type_instance( const char * name,
		bool has_default_constructor, bool has_copy_constructor, 
		bool has_assignment, bool has_destructor  )
	{
		char buff[ 2048 ] = {0};
		
		strcat_s( buff, "reflective implicit type: " );
		strcat_s( buff, name );

		if( has_default_constructor )
			strcat_s( buff, ", has_default_constructor" );

		if( has_copy_constructor )
			strcat_s( buff, ", has_copy_constructor" );

		if( has_assignment )
			strcat_s( buff, ", has_assignment" );

		if( has_destructor )
			strcat_s( buff, ", has_destructor" );

		strcat_s( buff, "\n" );

		output_debug_string( buff );
	}

	void * mem_alloc( size_t i_alignment, size_t i_size )
	{
		return _aligned_malloc( i_size, i_alignment );
	}

	void mem_free( void * i_allocation )
	{
		_aligned_free( i_allocation );
	}

	void * mem_lifo_alloc( size_t i_alignment, size_t i_size, reflective::DestructorCallback i_destroyer )
	{
		return lifo_allocator().allocate( i_alignment, i_size, i_destroyer );
	}

	void mem_lifo_free( void * i_allocation )
	{
		lifo_allocator().free( i_allocation );
	}

} // namespace reflective_externals
