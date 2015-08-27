
#include "lifo_allocator.h"

// LifoAllocator::allocate
void * LifoAllocator::allocate( size_t alignment, size_t size,
	reflective::DestructorCallback destructor_caller )
{
	REFLECTIVE_ASSERT( ( alignment & (alignment << 1) ) == 0 ); // alignment must be power of 2

	#if !REFLECTIVE_NO_CLEANUP_ON_EXIT
		// the alignment cannot be less than alignment_of( _Header )
		alignment = mem::upper_align( alignment, alignment_of( _Header ) );
	#endif

	// align the current address and reserve the size
	void * allocation = mem::upper_align( _curr_pos, alignment );
	void * end_of_allocation = mem::address_add( allocation, size );

	#if REFLECTIVE_NO_CLEANUP_ON_EXIT

		// check for end of space
		if( end_of_allocation > _end_address )
			return null;
		_curr_pos = end_of_allocation;

	#else

		// check for end of space
		_Header * header = static_cast<_Header *>( end_of_allocation );
		void * new_pos = header + 1;
		if( new_pos > _end_address )
			return nullptr;

		// initialize the header
		header->_prev_pos = _curr_pos;
		header->_allocation = allocation;
		header->_destructor_caller = destructor_caller;
		#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
			header->_progressive = _allocation_count;
			_allocation_count++;
		#endif

		// the current address is set to the end of the header
		_curr_pos = header + 1;

	#endif

	// done
	return allocation;
}

// LifoAllocator::free
void LifoAllocator::free( void * address )
{		
	_Header * header = static_cast<_Header *>( _curr_pos ) - 1;
	REFLECTIVE_ASSERT( address == header->_allocation );
	REFLECTIVE_UNUSED( header );

	free_to_bookmark( address );
}

// LifoAllocator::free_to_bookmark
void LifoAllocator::free_to_bookmark( const void * bookmark )
{
	#if !REFLECTIVE_NO_CLEANUP_ON_EXIT

		void * current_pos = _curr_pos;

		if( current_pos > bookmark ) do {

			// the header is just before current_pos
			_Header * header = static_cast<_Header *>( current_pos ) - 1;

			#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
				REFLECTIVE_ASSERT( _allocation_count > 0 );
				_allocation_count--;
				REFLECTIVE_ASSERT( header->_progressive == _allocation_count );
			#endif

			// call the destruction callback
			if( header->_destructor_caller )
				(*header->_destructor_caller)( header->_allocation );

			current_pos = header->_prev_pos;

			#if REFLECTIVE_IS_DEBUG
				mem::dbg_invalidate( header, header + 1 );
			#endif

		} while( current_pos > bookmark );

		// store the current pos
		_curr_pos = current_pos;

	#endif
}

// LifoAllocator::free_all
void LifoAllocator::free_all()
{
	free_to_bookmark( _start_address );
}

// LifoAllocator::bookmark
const void * LifoAllocator::bookmark() const
{
	return _curr_pos;
}

// LifoAllocator::constructor
LifoAllocator::LifoAllocator( void * buffer, size_t size,
	LifoAllocator * prev_allocator )
{
	_curr_pos = _start_address = buffer;
	_end_address = mem::address_add( buffer, size );
	_prev_allocator = prev_allocator;
	#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
		_allocation_count = 0;
	#endif
}

// LifoAllocator::destructor
LifoAllocator::~LifoAllocator()
{
	free_all();
	#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
		REFLECTIVE_ASSERT( _allocation_count == 0 );
	#endif
	REFLECTIVE_ASSERT( _curr_pos == _start_address ); // must be empty
}

