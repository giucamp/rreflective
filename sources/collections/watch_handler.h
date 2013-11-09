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
	#error include "reflective.h" instead of incuding this file
#endif

namespace reflective
{
	/* WatchHandler - wraps a watch - the watch is created when a collection is set, 
		and is destroyed when the collection is unset. The watch handler stores the
		watch size and start index, which are set to the watch when it is created. 
		By default the start index is zero and the size is ~0.
		*/
	/*template <class HOSTING_CLASS>
		class WatchHandler
	{
	public:

		WatchHandler();
		~WatchHandler();

		void set_handler( HOSTING_CLASS & notification_target,
			void (HOSTING_CLASS::*object_event_handler)( Watch::ObjectsEventParam & param ),
			void (HOSTING_CLASS::*count_event_handler)( Watch::CountEventParams & param ) );
		
		// collection setters
		bool set_collection( const Type & collection_type, void * collection );
		template <class COLLECTION_TYPE> bool set_collection( const COLLECTION_TYPE & collection );
		void unset_collection();

		// collection getters
		void * collection() const;
		const Type & collection_type() const;

		/* move_to - resize - move or resize a window. */
		void move_to( size_t first_watched_index );
		void move_to( size_t first_watched_index, size_t new_size );
		void resize( size_t new_size );

		// properties
		size_t first_watched_index() const;
		size_t watch_size() const;

		// watch
		const Watch * watch() const;

	private: // data members
		Watch * _watch;
		size_t _first_watched_index;
		size_t _watch_size;
		Handler< HOSTING_CLASS, Watch::ObjectsEventParam > _object_event_handler;
		Handler< HOSTING_CLASS, Watch::CountEventParams > _count_event_handler;
	};

} // namespace reflective
