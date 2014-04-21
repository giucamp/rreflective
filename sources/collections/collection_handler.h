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
	/* ICollectionHandler - provides an non-intrusive collection-like interface to the
		objects of the a given type suitable for iteration and watching. The collection
		object is passed to the methods of ICollectionHandler by a void pointer. Having
		the ICollectionHandler associated with the type, the collection type is not required
		to derive and implement any interface (that's the non-intrusiveness).
		Each collection handler is supposed to be associated with a class object (see
		reflective::Class). So all the objects of the collection class share the
		same instance of ICollectionHandler. */
	class ICollectionHandler
	{
	public:

		// query_item_count
		virtual bool query_item_count( const void * collection_object, size_t * out_count ) const = 0;
			/* Not all the collections are able to return quickly their item count, so this method
			may return false, and out_count is invalidated by the call. Furthermore, the item count may
			too big to fit in a size_t. If the method returns true, out_count has been assigned with
			the correct item count. */

		// resize
		virtual bool resize( void * collection_object, size_t new_size ) const = 0;

		// query_item_base_type
		virtual const Type * query_item_base_type( const void * collection_object,
			bool * out_allow_derived_types ) const = 0; /* Returns the Type from which all the objects
				belonging to the collection are required to derive. query_item_base_type is set to
				true is the collection can handle derived classes. If query_item_base_type returns
				nullptr the collection can handle any type of objects. */

		// iteration
		virtual IIterator * create_iterator( void * i_collection_object, const void * i_key_value ) const = 0;

		// add_object
		virtual void * add_object( const QualifiedType & i_qualified_type, void * i_collection_object, const void * i_key_value ) const = 0;

		// get_key_type
		enum KeyFlags
		{
			ePositionalIndex = 1 << 0,
			eAllowDuplicates = 1 << 1,
		};
		virtual bool get_key_type( const void * i_collection_object, uint32_t * o_flags, QualifiedType * o_index_type ) const = 0;

		
				/* watching */

		virtual bool register_watch( Watch * watch, void * collection_object ) const = 0;

		virtual void unregister_watch( Watch * watch, void * collection_object ) const = 0;
	};
}
