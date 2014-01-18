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

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{
	// WatchChain::notify_added_objects
	void WatchChain::notify_added_objects( size_t first_index, size_t objects_count )
	{
		Watch::NotifyParams notify_params( first_index, objects_count );

		Watch * curr_watch = _first_watch;

		if( curr_watch == nullptr ) 
			return;

		do {

			curr_watch->notify_added_objects( notify_params );

			curr_watch = curr_watch->_next_watch;

		} while( curr_watch != nullptr );
	}

	// WatchChain::notify_changed_objects
	void WatchChain::notify_changed_objects( size_t first_index, size_t objects_count )
	{
		Watch::NotifyParams notify_params( first_index, objects_count );

		Watch * curr_watch = _first_watch;

		if( curr_watch == nullptr ) 
			return;

		do {

			curr_watch->notify_changed_objects( notify_params );

			curr_watch = curr_watch->_next_watch;

		} while( curr_watch != nullptr );
	}

	// WatchChain::notify_removing_objects
	void WatchChain::notify_removing_objects( size_t first_index, size_t objects_count )
	{
		Watch::NotifyParams notify_params( first_index, objects_count );

		Watch * curr_watch = _first_watch;

		if( curr_watch == nullptr ) 
			return;

		do {

			curr_watch->notify_removing_objects( notify_params );

			curr_watch = curr_watch->_next_watch;

		} while( curr_watch != nullptr );
	}

	// WatchChain::unregister_watch
	void WatchChain::unregister_watch( Watch * watch )
	{
		Watch * curr_watch = _first_watch;
		Watch * prev_watch = nullptr;
		if( curr_watch ) do {
			
			if( curr_watch == watch )
			{
				if( prev_watch )
				{
					REFLECTIVE_ASSERT( _first_watch != curr_watch );
					prev_watch->_next_watch = watch->_next_watch;
					REFLECTIVE_ASSERT( prev_watch->_next_watch != prev_watch );
				}
				else
				{
					REFLECTIVE_ASSERT( _first_watch == curr_watch );
					_first_watch = watch->_next_watch;
				}
			}

			prev_watch = curr_watch;
			curr_watch = curr_watch->_next_watch;

		} while( curr_watch != nullptr );

		watch->_parent_chain = nullptr;
		watch->_next_watch = nullptr;
		watch->_collection_type = nullptr;
		watch->_collection = nullptr;
	}

	// WatchChain::unregister_all_watches
	void WatchChain::unregister_all_watches()
	{
		Watch * curr_watch = _first_watch;

		if( curr_watch == nullptr ) 
			return;

		do {

			curr_watch->_parent_chain = nullptr;
			curr_watch->_next_watch = nullptr;

			curr_watch = curr_watch->_next_watch;

		} while( curr_watch != nullptr );
	}

	// WatchChain::register_watch
	void WatchChain::register_watch( Watch * watch, const Type & collection_type, void * collection )
	{
		REFLECTIVE_ASSERT( watch->_parent_chain == nullptr ); // unregister first

		watch->_parent_chain = this;
		watch->_next_watch = _first_watch;
		watch->_collection_type = &collection_type;
		watch->_collection = collection;
		REFLECTIVE_ASSERT( watch->_next_watch != watch );
		_first_watch = watch;		
	}

} // namespace reflective
