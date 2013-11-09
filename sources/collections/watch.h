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
	class Watch;
	class WatchChain;

		/* watch registration */

	bool register_watch( Watch * watch, const Type & collection_type, void * collection );	
	void unregister_watch( Watch * watch );

	template <class COLLECTION_TYPE> inline bool register_watch( Watch * watch, COLLECTION_TYPE & collection )
	{
		const Type & collection_type = type_of( collection );
		return register_watch( watch, collection_type, &collection );
	}


	// Watch 
	class Watch
	{
	public:

			/* constructor \ destructor */

		Watch();

		virtual ~Watch(); // the watch cannot be attacched to a collection at destruction



				/* notifications */

		struct NotifyParams
		{
			size_t first_index;
			size_t objects_count;

			NotifyParams( size_t first_index_, size_t objects_count_ )
				: first_index( first_index_ ), objects_count( objects_count_ ) 
					{ }

		private: // private interface
			NotifyParams & operator = ( const NotifyParams & ); // not supported
		};

		virtual void notify_added_objects( const NotifyParams & notify_params ) = 0;

		virtual void notify_changed_objects( const NotifyParams & notify_params ) = 0;

		virtual void notify_removing_objects( const NotifyParams & notify_params ) = 0;



				/* target collection */

		const Type * collection_type() const;

		void * collection() const;

	private: // private interface
		Watch & operator = ( const Watch & ); // not supported
		friend class WatchChain;	

	private: // data members
		WatchChain * _parent_chain;
		Watch * _next_watch;
		const Type * _collection_type;
		void * _collection;
	};

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Watch
	reflective::Class * init_type(
		reflective::Watch * null_pointer_1,
		reflective::Watch * null_pointer_2 );
}


