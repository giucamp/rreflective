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
	/* CollectionHandler - provides an non-intrusive collection-like interface to the
		objects of the a given type suitable for iteration and watching. The collection
		object is passed to the methods of CollectionHandler by a void pointer. Having
		the CollectionHandler associated with the type, the collection type is not required
		to derive and implement any interface (that's the non-intrusiveness).
		Each collection handler is supposed to be associated with a class object (see
		reflective::Class). So all the objects of the collection class share the
		same instance of CollectionHandler.
	*/
	class CollectionHandler
	{
	protected:

		/** Constructs the handler, assigning the size and alignment required for an iterator */
		CollectionHandler( size_t i_iterator_size, size_t i_iterator_alignment )
			: m_iterator_size( i_iterator_size ), m_iterator_alignment( i_iterator_alignment ) { }

	public:

		enum Flags
		{	
			eNoFlags					= 0,
			eSupportPositionalIndex		= 1 << 0, /**< the collection support get, set, insertion and deletion of items 
														given a positional index. If the collection has this flag, it 
														should have eSupportGetCount too. */
			eSupportGetCount			= 1 << 1, /**< the collection supports the method get_count */
			eSupportKey					= 1 << 2, /**< the collection support get, set, insertion and deletion of items 
														given a key. The type of the key can be retrieved by get_key_type.  */
			//eSupportDuplicateKeys		= 1 << 3, /**< the key of the collection is not unique */
			eAllowDerivedItems			= 1 << 4, /**< the items of the collection can be instances of the type returned by 
													   get_item_type or of a derived type. */
			eAllowEdit					= 1 << 5, /**< the collection can be altered */
			eSupportReserve				= 1 << 6, /**< the collection supports the reserve, to efficiently fill it
													   when an estimate of the total item count is known in advance */
			eSupportWatch				= 1 << 7
		};

		/** Flags values can be combined with the bitwise or, and the result is still a valid Flags value */
		friend inline Flags operator | ( Flags i_first, Flags i_second )
			{ return static_cast<Flags>( static_cast<int>(i_first) | static_cast<int>(i_second) ); }

		/** Returns the flags associated to the specified collection */
		virtual Flags get_flags( const void * i_colection_object ) const = 0;

		/** Returns the type of the items in the collection. If the collection has the flag eAllowDerivedItems,
			items may be instance of a type derived from the returned one. If the returned type is empty (its type()
			method returns true), the collection can contain items of any type. */
		virtual QualifiedType get_item_type( const void * i_collection_object ) const = 0; 

		/** Returns the type of the keys. 
			Required flags: eSupportKey */
		virtual QualifiedType get_key_type( const void * /*i_collection_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return QualifiedType::s_empty; }

		/** Returns the count of items in the collection.
			Required flags: eSupportGetCount */
		virtual size_t get_count( const void * /*i_collection_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return 0; }

		/** Returns the item at the specified index. 
			@param i_index the index must be < the result of get_count. If the index is out of range the behaviour is undefined.
			Required flags: eSupportPositionalIndex, eSupportGetCount */
		virtual ObjectPointerWrapper get_at_index( const void * /*i_collection_object*/, size_t /*i_index*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return ObjectPointerWrapper(); }

		/** Returns the item at the specified key. If there's no object in the collection with the specified key, the return
			value is an empty ObjectPointerWrapper.
			Required flags: eSupportKey */
		virtual ObjectPointerWrapper get_at_key( const void * /*i_collection_object*/, const void * /*i_key*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return ObjectPointerWrapper(); }

		/** Tries to set the item at the specified index. This method maybe unsupported by the collection.
			@return pointer to the item in the collection
			Required flags: eSupportPositionalIndex, eAllowEdit */
		virtual void * set_at_index( void * /*i_collection_object*/, size_t /*i_index*/, const ObjectPointerWrapper & /*i_new_item*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return nullptr; }
		
		/** Reserve memory space in the collection for the specified item count.
			Required flags: eAllowEdit, eSupportReserve */
		virtual void reserve( void * /*i_collection_object*/, size_t /*i_total_item_count*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ }

		/** Tries to insert an item with the specified key. The collection may not support this method.
			If an object with the same key is already present, no item is inserted, the parameter i_source_object 
			is ignored, and a pointer to the existing item is returned.
			@param i_source_object object to use as source to copy-construct the new item. If the object of i_source_object
				is null, the new item is default-constructed. The object of i_source_object mus always be valid.
			@return pointer to the item in the collection, or nullptr on failure
			Required flags: eSupportKey, eAllowEdit */
		virtual void * insert_item_at_key( void * /*i_collection_object*/, const void * /*i_key*/, const ObjectPointerWrapper & /*i_source_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return nullptr; }
				
		/** Tries to add an item to the collection at the specified index
			Required flags: eSupportPositionalIndex, eAllowEdit */
		virtual void * insert_item_at_index( void * /*i_collection_object*/, size_t /*i_index*/, const ObjectPointerWrapper & /*i_source_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return nullptr; }

		/** Tries to delete the item of the collection at the specified key 
			Required flags: eSupportKey, eAllowEdit */
		virtual bool delete_item_at_key( void * /*i_collection_object*/, const void * /*i_key_value*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return false; }

		/** Tries to delete the item of the collection at the specified index
			Required flags: eSupportPositionalIndex, eAllowEdit */
		virtual bool delete_item_at_index( void * /*i_collection_object*/, size_t /*i_index*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return false; }
		

				/* iterator */

		/** Returns the size of the iterator, that can be used to allocate a buffer before calling construct_iterator */
		size_t iterator_size() const			{ return m_iterator_size; }

		/** Returns the alignment of the iterator, that can be used to allocate a buffer before calling construct_iterator */
		size_t iterator_alignment() const		{ return m_iterator_alignment; }
		
		/** Constructs an iterator for the specified collection in the provided buffer. The size and alignment required for the
				buffer can be retrieved with the methods iterator_size and iterator_alignment 
			@return the iterator or nullptr is the iterator could not be constructed. */
		virtual AbstractIterator * construct_iterator( void * i_collection_object, void * i_iterator_buffer ) const = 0;

		/** creates an iterator for the specified collection, allocating it in the lifo memory container.
			@return the iterator or nullptr is the iterator could not be created. The iterator must be destroyed with REFLECTIVE_LIFO_DELETE. */
		AbstractIterator * create_iterator_lifo( void * i_collection_object ) const
		{
			void * alloc = reflective_externals::mem_lifo_alloc( m_iterator_alignment, m_iterator_size, &default_destructor_callback<AbstractIterator> );
			if( alloc != nullptr )
			{
				AbstractIterator * iter = construct_iterator(i_collection_object, alloc );
				if( iter != nullptr )
					return iter;
				reflective_externals::mem_lifo_free(alloc);
			}
			return nullptr;
		}

		/** creates an iterator for the specified collection, allocating it in the dynamic memory container.
			@return the iterator or nullptr is the iterator could not be created. The iterator must be destroyed with REFLECTIVE_DELETE. */
		AbstractIterator * create_iterator_dynamic( void * i_collection_object ) const
		{
			void * alloc = reflective_externals::mem_alloc( m_iterator_alignment, m_iterator_size );
			if( alloc != nullptr )
			{
				AbstractIterator * iter = construct_iterator(i_collection_object, alloc );
				if( iter != nullptr )
					return iter;
				reflective_externals::mem_free(alloc);
			}
			return nullptr;
		}

				/* watching */

		virtual bool register_watch( Watch * /*i_watch*/, void * /*i_collection_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ return false; }

		virtual void unregister_watch( Watch * /*i_watch*/, void * /*i_collection_object*/ ) const
			{ REFLECTIVE_ASSERT(false); /* unsupported */ }

	private:
		size_t m_iterator_size, m_iterator_alignment;
	};
}
