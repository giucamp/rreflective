
#include "reflective.h"

// LifoAllocator
class LifoAllocator
{
public:

	// constructor
	LifoAllocator( void * buffer, size_t size, 
		LifoAllocator * prev_allocator = nullptr );

	// destructor
	~LifoAllocator();

	/* allocate - reserves a block of memory with the specified 
		alignment and size. The function pointed by 
		destructor_caller, if any, will be called when the
		block is beeng deallocated. */
	void * allocate( size_t alignment, size_t size, 
		reflective::DestructorCallback destructor_caller = nullptr );

	// free
	void free( void * address ); /* only the allocation on
		top of thestack can be freed. The parameter 'address' is
		used only to che the consitency. */

	/* retrives a bookmark which can be used to free all the 
		successive allocations */
	const void * bookmark() const;

	/* free_to_bookmark - free all the allocations done
		since the bookmark was retrived. */
	void free_to_bookmark( const void * bookmark );

	/* free_all - free all the allocations done
		since the alocator was constructed. */
	void free_all();

	#if REFLECTIVE_DEBUG_MEMORY

		bool push_debug_context( const DebugContextConfig & config )
			{ REFLECTIVE_UNUSED( config ); return false; }

		void pop_debug_context() 
			{ }

	#endif

private:

	#if !REFLECTIVE_NO_CLEANUP_ON_EXIT
		struct _Header
		{
			void * _prev_pos;
			void * _allocation;
			reflective::DestructorCallback _destructor_caller;
			#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
				size_t _progressive;				
			#endif
		};
	#endif

private: // data members
	void * _curr_pos;
	void * _start_address;
	void * _end_address;
	LifoAllocator * _prev_allocator;
	#if REFLECTIVE_DEBUG_LIFO_DESTRUCTION
		size_t _allocation_count;
	#endif
};

