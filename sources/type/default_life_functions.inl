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

#ifndef _INCLUDING_REFLECTIVE
	#error include "reflective.h" instead of including this file
#endif

namespace reflective
{
	#if REFLECTIVE_IS_DEBUG
		template < class TYPE > 
			void _dbg_check_range( const void * dest_start, const void * dest_end )
		{
			REFLECTIVE_ASSERT( dest_start <= dest_end );
			REFLECTIVE_ASSERT( mem::is_aligned( dest_start, alignment_of( TYPE ) ) );
			REFLECTIVE_ASSERT( mem::is_aligned( dest_end, alignment_of( TYPE ) ) );

			const size_t length = mem::address_diff( dest_end, dest_start );			
			REFLECTIVE_ASSERT( ( length % sizeof( TYPE ) ) == 0 );
			REFLECTIVE_UNUSED( length );
		}
	#endif

	// objects_construct_void<TYPE>( dest_start, dest_end )
	template < class TYPE > 
		void objects_construct_void( void * _dest_start, void * _dest_end )
	{
		#if REFLECTIVE_IS_DEBUG
			_dbg_check_range<TYPE>( _dest_start, _dest_end );
		#endif

		TYPE * dest_start = static_cast<TYPE *>( _dest_start );
		TYPE * dest_end = static_cast<TYPE *>( _dest_end );
		#if REFLECTIVE_IS_DEBUG
			mem::dbg_invalidate( dest_start, dest_end );
		#endif

		TYPE * object = dest_start; if( object < dest_end ) do {
			REFLECTIVE_ASSERT( object != nullptr );

			#ifdef _MSC_VER
				#pragma warning( push )
				#pragma warning( disable: 4345 ) /* behavior change: an object of POD 
					type constructed with an initializer of the form () will be default-initialized */
			#endif

			::new ( object ) TYPE();

			#ifdef _MSC_VER
				#pragma warning( pop )
			#endif

		} while( ++object < dest_end );
	}

	// objects_destroy_void<TYPE>( dest_start, dest_end )
	template < class TYPE >
		void objects_destroy_void( void * _dest_start, void * _dest_end )
	{
		#if REFLECTIVE_IS_DEBUG
			_dbg_check_range<TYPE>( _dest_start, _dest_end );
		#endif

		if( _dest_start >= _dest_end )
			return;

		TYPE * dest_start = static_cast<TYPE *>( _dest_start );
		TYPE * dest_end = static_cast<TYPE *>( _dest_end );

		TYPE * object = dest_end - 1; do {
			REFLECTIVE_ASSERT( object != nullptr );
			object->TYPE::~TYPE();
		} while( --object >= dest_start );

		#if REFLECTIVE_IS_DEBUG
			mem::dbg_invalidate( dest_start, dest_end );
		#endif
	}

	// objects_copy_void<TYPE>( dest_start, dest_end, source_start )
	template <class TYPE>
		void objects_copy_void( void * _dest_start, void * _dest_end, const void * _source_start )
	{
		#if REFLECTIVE_IS_DEBUG
			_dbg_check_range<TYPE>( _dest_start, _dest_end );
		#endif

		TYPE * dest_start = static_cast<TYPE *>( _dest_start );
		TYPE * dest_end = static_cast<TYPE *>( _dest_end );
		const TYPE * source_start = static_cast<const TYPE *>( _source_start );
		
		#if REFLECTIVE_IS_DEBUG
			mem::dbg_invalidate( dest_start, dest_end );
			_dbg_check_range<TYPE>( source_start, source_start + (dest_end - dest_start) );
		#endif

		TYPE * dest = dest_start;
		const TYPE * source = source_start;
		if( dest < dest_end ) do {
			REFLECTIVE_ASSERT( dest != source );
			REFLECTIVE_ASSERT( dest != nullptr );
			::new( dest ) TYPE( *source );
		} while( ++source, ++dest < dest_end );
	}

	// grabbed_objects_move_void<TYPE>( dest_start, dest_end, source_start )
	template <class TYPE> void grabbed_objects_move_void( void * _dest_start, void * _dest_end, void * _source_start )
	{
		if( _dest_start == _source_start || _dest_start == _dest_end )
			return; // exit unless source and dest are distint and non-empty

		TYPE * dest_start = static_cast<TYPE *>( _dest_start );
		TYPE * dest_end = static_cast<TYPE *>( _dest_end );
		TYPE * source_start = static_cast<TYPE *>( _source_start );

		const size_t cell_count = mem::address_diff( dest_end, dest_start );
		TYPE * source_end = static_cast<TYPE *>( mem::address_add( source_start, cell_count ) );
		if( source_start < dest_start )
		{
			if( dest_start < source_end )
			{
				//			SOURCE				overlapping
				//		|------------|
				//			    DEST
				//		    |------------|

				#if REFLECTIVE_IS_DEBUG
					int life_count = 0;
				#endif

				// backward copy-construct of the end
				TYPE * curr_dest = dest_end - 1;
				TYPE * curr_source = source_end - 1;
				if( curr_dest >= source_end ) do {			
					
					REFLECTIVE_ASSERT( curr_source != curr_dest );
					REFLECTIVE_ASSERT( curr_dest != nullptr );

					::new( curr_dest ) TYPE( *curr_source );

					#if REFLECTIVE_IS_DEBUG
						life_count++;
					#endif

				} while( --curr_source, --curr_dest >= source_end );

				// backward assign the overlapping range
				if( curr_dest >= dest_start ) do {			
					
					REFLECTIVE_ASSERT( curr_source != curr_dest );

					*curr_dest = *curr_source;

				} while( --curr_source, --curr_dest >= dest_start );

				// forward destroy remaining source
				curr_source = source_start;
				do {
					
					curr_source->TYPE::~TYPE();

					#if REFLECTIVE_IS_DEBUG
						life_count--;
					#endif

				} while( ++curr_source < dest_start );

				#if REFLECTIVE_IS_DEBUG
					REFLECTIVE_ASSERT( life_count == 0 );
				#endif
				return; // done
			}
		}
		else
		{
			if( dest_end > source_start )
			{
				//		        	SOURCE				overlapping
				//		        |------------|
				//			      DEST
				//		    |------------|

				#if REFLECTIVE_IS_DEBUG
					int life_count = 0;
				#endif

				// copy-construct initial dest
				TYPE * curr_dest = dest_start;
				TYPE * curr_source = source_start;
				if( curr_dest < source_start ) do {			
					
					REFLECTIVE_ASSERT( curr_source != curr_dest );
					REFLECTIVE_ASSERT( curr_dest != nullptr ); // this is an hint for the optimizing compiler

					::new( curr_dest ) TYPE( *curr_source );

					#if REFLECTIVE_IS_DEBUG
						life_count++;
					#endif

				} while( ++curr_source, ++curr_dest < source_start );

				// assignment in the overlapping range
				if( curr_dest < dest_end ) do {			
					
					REFLECTIVE_ASSERT( curr_source != curr_dest );

					*curr_dest = *curr_source;

				} while( ++curr_source, ++curr_dest < dest_end );

				// destroy remaining source
				curr_source = dest_end;
				if( curr_source < source_end ) do {
					
					curr_source->TYPE::~TYPE();

					#if REFLECTIVE_IS_DEBUG
						life_count--;
					#endif

				} while( ++curr_source < source_end );

				#if REFLECTIVE_IS_DEBUG
					REFLECTIVE_ASSERT( life_count == 0 );
				#endif
				return; // done
			}
		}

		// no overlapping: forward move
		TYPE * curr_dest = dest_start;
		TYPE * curr_source = source_start;
		REFLECTIVE_ASSERT( curr_dest < dest_end );
		do {			
			
			REFLECTIVE_ASSERT( curr_source != curr_dest );
			REFLECTIVE_ASSERT( curr_dest != nullptr );

			::new( curr_dest ) TYPE( *curr_source );
			curr_source->TYPE::~TYPE();

		} while( ++curr_source, ++curr_dest < dest_end );
	}

	// objects_move_void<TYPE>( dest_start, dest_end, source_start )
	template <class TYPE> void objects_move_void( void * _dest_start, void * _dest_end, void * _source_start )
	{
		mem::move( _dest_start, _dest_end, _source_start );
	}

	// _non_destructive_copy( dest_start, dest_end, source )
	template < class TYPE > 
		inline void _non_destructive_copy( TYPE * dest_start, TYPE * dest_end, const TYPE * source_start )
	{
		if( dest_start == source_start )
			return;

		const TYPE * source = source_start;
		TYPE * dest = dest_start; if( dest < dest_end ) do {
			REFLECTIVE_ASSERT( dest != source );
			REFLECTIVE_ASSERT( dest != nullptr );
			*dest = *source;
		} while( ++source, ++dest < dest_end );
	}

	// _non_destructive_copy( dest_start, dest_end, source )
	template < class TYPE > 
		inline void _non_destructive_reverse_copy( TYPE * dest_start, TYPE * dest_end, const TYPE * source_start )
	{
		const size_t length = mem::address_diff( dest_end, dest_start );		
		TYPE * dest = dest_end; 
		const TYPE * source = static_cast<const TYPE*>( mem::address_add( source_start, length ) );
		dest--, source--;		
		if( dest >= dest_start ) do {
			REFLECTIVE_ASSERT( dest != source );
			REFLECTIVE_ASSERT( dest != nullptr );
			*dest = *source;
		} while( --source, --dest >= dest_start );
	}

	// objects_construct
	template < class TYPE >
		inline void objects_construct( TYPE * dest )
	{
		objects_construct_void<TYPE>( dest, dest + 1 );
	}

	// objects_construct
	template < class TYPE > 
		inline void objects_construct( TYPE * dest_start, TYPE * dest_end )
	{
		objects_construct_void<TYPE>( dest_start, dest_end );
	}

	// objects_destruct
	template < class TYPE > 
		inline void objects_destruct( TYPE * start_address, TYPE * end_address )
	{
		objects_destroy_void<TYPE>( start_address, end_address );
	}

	// objects_destruct
	template < class TYPE > 
		inline void objects_destruct( TYPE * dest )
	{
		objects_destroy_void<TYPE>( dest, dest + 1 );
	}

	// objects_copy
	template < class TYPE >
		inline void objects_copy( TYPE * dest_start, TYPE * dest_end, const TYPE * source_start )
	{
		objects_copy_void<TYPE>( dest_start, dest_end, source_start );
	}

	// objects_copy
	template < class TYPE >
		inline void objects_copy( TYPE * dest, const TYPE * source )
	{
		objects_copy_void<TYPE>( dest, dest + 1, source );
	}

	// objects_move
	template < class TYPE >
		inline void objects_move( TYPE * dest_start, TYPE * dest_end, TYPE * source_start )
	{
		objects_move_void<TYPE>( dest_start, dest_end, source_start );
	}

	// objects_move
	template < class TYPE >
		inline void objects_move( TYPE * dest, TYPE * source )
	{
		objects_move_void<TYPE>( dest, dest + 1, source );
	}

} // namespace reflective
