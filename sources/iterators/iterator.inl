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
	// Iterator::constructor( collection )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<TYPE>::Iterator( COLLECTION_TYPE & collection )
				: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection, iteration_type )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<TYPE>::Iterator( COLLECTION_TYPE & collection,
				const reflective::Type & iteration_type )
					: _iteration_type( iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection_type, collection )
	template <class TYPE>
		inline Iterator<TYPE>::Iterator( const reflective::Type & collection_type, void * collection )
			: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( collection_type, collection );
	}

	// Iterator::constructor( collection_type, collection, iteration_type )
	template <class TYPE>
		inline Iterator<TYPE>::Iterator( const reflective::Type & collection_type, void * collection,
			const reflective::Type & iteration_type )
				: _iteration_type( iteration_type )
	{
		_assign_collection( collection_type, collection );
	}


	// Iterator::constructor( collection, offset_index )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<TYPE>::Iterator( COLLECTION_TYPE & collection, size_t offset_index )
				: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, offset_index );
	}

	// Iterator::constructor( collection, iteration_type, offset_index )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<TYPE>::Iterator( COLLECTION_TYPE & collection, const reflective::Type & iteration_type, size_t offset_index )
				: _iteration_type( iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, offset_index );
	}

	// Iterator::constructor( collection_type, collection, offset_index )
	template <class TYPE>
		inline Iterator<TYPE>::Iterator( const reflective::Type & collection_type, void * collection, size_t offset_index )
			: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( collection_type, collection, offset_index );
	}

	// Iterator::constructor( collection_type, collection, iteration_type, offset_index )
	template <class TYPE>
		inline Iterator<TYPE>::Iterator( const reflective::Type & collection_type, void * collection,
			const reflective::Type & iteration_type, size_t offset_index )
				: _iteration_type( iteration_type )
	{
		_assign_collection( collection_type, collection, offset_index );
	}

	// Iterator::destructor
	template <class TYPE>
		inline Iterator<TYPE>::~Iterator()
	{
		if( _abstract_iterator )
		{
			REFLECTIVE_LIFO_DELETE( _abstract_iterator );
		}
	}

	// Iterator::type
	template <class TYPE>
		inline const reflective::Type & Iterator<TYPE>::type() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *_group.type;
	}

	// Iterator::object
	template <class TYPE>
		inline TYPE & Iterator<TYPE>::object() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *static_cast<TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator *
	template <class TYPE>
		inline TYPE & Iterator<TYPE>::operator * () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *static_cast<TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator ->
	template <class TYPE>
		inline TYPE * Iterator<TYPE>::operator -> () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return static_cast<TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator *
	template <class TYPE>
		inline Iterator<TYPE>::operator TYPE * () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return static_cast<TYPE*>( _group.curr_in_group );
	}

	// Iterator::is_not_over
	template <class TYPE>
		inline bool Iterator<TYPE>::is_not_over() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator::operator bool
	template <class TYPE>
		inline Iterator<TYPE>::operator bool() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator<TYPE>::_assign_collection( collection_type, collection )
	template <class TYPE>
		inline void Iterator<TYPE>::_assign_collection(
			const reflective::Type & collection_type, void * collection )
	{
		_abstract_iterator = null;
		_group.curr_in_group = null;
		_group.end_of_group = null;
		_index_in_group = 0;

		const reflective::CollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( collection, 0 );
			if( _abstract_iterator )
			{
				_abstract_iterator->first_group( _group );

				// jump groups not compatible with _iteration_type
				if( !( _iteration_type <= *_group.type ) ) do {
					_abstract_iterator->next_group( _group );
					if( _group.curr_in_group >= _group.end_of_group )
						return;
				} while( !( _iteration_type <= *_group.type ) );
			}
		}
	}

	// Iterator<TYPE>::_assign_collection( collection_type, collection )
	template <class TYPE>
		inline void Iterator<TYPE>::_assign_collection(
			const reflective::Type & collection_type, void * collection, size_t offset_index )
	{
		_abstract_iterator = null;
		_group.curr_in_group = null;
		_group.end_of_group = null;
		_index_in_group = 0;

		const reflective::CollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( collection, offset_index );
			if( _abstract_iterator )
			{
				_abstract_iterator->first_group( _group );

				// jump groups not compatible with _iteration_type
				if( !( _iteration_type <= *_group.type ) ) do {
					_abstract_iterator->next_group( _group );
					if( _group.curr_in_group >= _group.end_of_group )
						return;
				} while( !( _iteration_type <= *_group.type ) );
			}
		}
	}

	// Iterator<TYPE>::move_next
	template <class TYPE>
		inline bool Iterator<TYPE>::move_next()
	{
		REFLECTIVE_ASSERT( is_not_over() );		

		// get the next of the group
		const size_t object_size = _group.type->size();
		_group.curr_in_group = mem::address_add( _group.curr_in_group, object_size );
		_index_in_group++;
		if( _group.curr_in_group < _group.end_of_group )
			return true;

		// group finished, ask another compatible with _iteration_type
		_index_in_group = 0;
		do {
			_abstract_iterator->next_group( _group );
			if( _group.curr_in_group >= _group.end_of_group )
				return false;
		} while( !( _iteration_type <= *_group.type ) );

		return true;
	}

	// Iterator<TYPE>::operator ++
	template <class TYPE>
		inline bool Iterator<TYPE>::operator ++ ()
	{
		return move_next();
	}

	// Iterator<TYPE>::postfix operator ++
	template <class TYPE>
		inline void Iterator<TYPE>::operator ++ ( int )
	{
		move_next();
	}

	// Iterator<TYPE>::insert()
	template <class TYPE>
		inline bool Iterator<TYPE>::insert()
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( 1, null, type );
	}

	// Iterator<TYPE>::insert( source_object )
	template <class TYPE>
		inline bool Iterator<TYPE>::insert( const TYPE & source_object )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( 1, &source_object, type );
	}

	// Iterator<TYPE>::insert( source_object, object_type )
	template <class TYPE>
		inline bool Iterator<TYPE>::insert( const void * source_object, const Type & object_type )
	{
		return multi_insert( 1, source_object, object_type );
	}

	// Iterator<TYPE>::multi_insert( object_count )
	template <class TYPE>
		inline bool Iterator<TYPE>::multi_insert( size_t object_count )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( object_count, null, type );
	}

	// Iterator<TYPE>::multi_insert( object_count, source_object )
	template <class TYPE>
		inline bool Iterator<TYPE>::multi_insert( size_t object_count, const TYPE & source_object )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( object_count, &source_object, type );
	}

	// Iterator<TYPE>::multi_insert( object_count, source_object, object_type )
	template <class TYPE>
		inline bool Iterator<TYPE>::multi_insert( size_t object_count, const void * source_object, const Type & object_type )
	{
		const bool result = _abstract_iterator->insert( _index_in_group,
			object_type, source_object, object_count, _group );

		if( result )
			_index_in_group = 0;

		return result;
	}

	// Iterator<TYPE>::remove
	template <class TYPE>
		inline bool Iterator<TYPE>::remove()
	{
		return multi_remove( 1 );
	}

	// Iterator<TYPE>::multi_remove
	template <class TYPE>
		inline bool Iterator<TYPE>::multi_remove( size_t object_count )
	{
		const bool result = _abstract_iterator->remove( _index_in_group, _group );

		if( result )
			_index_in_group = 0;

		return result;
	}

} // namespace reflective

