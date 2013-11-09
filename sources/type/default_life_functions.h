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
	/* objects_construct - calls the default constructor on an object or an array of objects s*/
	template < class TYPE > void objects_construct( TYPE * dest );
	template < class TYPE > void objects_construct( TYPE * dest_start, TYPE * dest_end );

	// objects_construct - calls the destructor on an object or an array of objects
	template < class TYPE > void objects_destruct( TYPE * dest );
	template < class TYPE > void objects_destruct( TYPE * start_address, TYPE * end_address );

	/* objects_copy - calls the copy constructor on a object or an array of objects 
		the source and the destination cannot overlap. */
	template < class TYPE > void objects_copy( TYPE * dest, const TYPE * source );
	template < class TYPE > void objects_copy( TYPE * dest_start, TYPE * dest_end, const TYPE * source_start );	

	/* grabbed_objects_move - calls the copy constructor on a object or an array of objects, and then calls the 
		destructor on the source object(s). The source and the destination can overlap. */
	template < class TYPE > void grabbed_objects_move( TYPE * dest, TYPE * source );
	template < class TYPE > void grabbed_objects_move( TYPE * dest_start, TYPE * dest_end, TYPE * source_start );

	/* objects_move - equvalent to grabbed_objects_move, but faster, for non-grabbed types. These functions 
		assume that an object can be moved by memcpy to a new location and will work. This is true not only for
		even pods, bur even	for object with construction\destruction side effects, such allocations and external 
		resource instancing. For objects relying on their address (which are very rare) this function cannot be used, 
		but they need grabbed_objects_move. */
	template < class TYPE > void objects_move( TYPE * dest, TYPE * source );
	template < class TYPE > void objects_move( TYPE * dest_start, TYPE * dest_end, TYPE * source_start );

	// objects_construct - objects_destroy_void - objects_copy - objects_move
	template < class TYPE > void objects_construct_void( void * dest_start, void * dest_end );
	template < class TYPE > void objects_destroy_void( void * start_address, void * end_address );
	template < class TYPE > void objects_copy_void( void * dest_start, void * dest_end, const void * source_start );
	template < class TYPE > void objects_move_void( void * dest_start, void * dest_end, void * source_start );
	template < class TYPE > void grabbed_objects_move_void( void * dest_start, void * dest_end, void * source_start );	

} // namespace reflective
