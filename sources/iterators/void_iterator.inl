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
	template <class COLLECTION_TYPE> 
		inline Iterator<void>::Iterator( const COLLECTION_TYPE & collection )
			: _iteration_type( nullptr )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection, iteration_type )
	template <class COLLECTION_TYPE> 
		inline Iterator<void>::Iterator( const COLLECTION_TYPE & collection, 
			const reflective::Type & iteration_type )
				: _iteration_type( &iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection_type, collection )
	inline Iterator<void>::Iterator( const reflective::Type & collection_type, void * collection )
		: _iteration_type( nullptr )
	{
		_assign_collection( collection_type, collection );
	}

	// Iterator::constructor( collection_type, collection, iteration_type )
	inline Iterator<void>::Iterator( const reflective::Type & collection_type, void * collection,
		const reflective::Type & iteration_type )
			: _iteration_type( &iteration_type )
	{
		_assign_collection( collection_type, collection );
	}


	// Iterator::constructor( collection, i_key_value )
	template <class COLLECTION_TYPE> 
		inline Iterator<void>::Iterator( const COLLECTION_TYPE & collection, const void * i_key_value )
			: _iteration_type( nullptr )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, i_key_value );
	}

	// Iterator::constructor( collection, iteration_type, i_key_value )
	template <class COLLECTION_TYPE> 
		inline Iterator<void>::Iterator( const COLLECTION_TYPE & collection, const reflective::Type & iteration_type, const void * i_key_value )
				: _iteration_type( &iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, i_key_value );
	}

	// Iterator::constructor( collection_type, collection, i_key_value )
	inline Iterator<void>::Iterator( const reflective::Type & collection_type, void * collection, const void * i_key_value )
			: _iteration_type( nullptr )
	{
		_assign_collection( collection_type, collection, i_key_value );
	}

	// Iterator::constructor( collection_type, collection, iteration_type, i_key_value )
	inline Iterator<void>::Iterator( const reflective::Type & collection_type, void * collection,
		const reflective::Type & iteration_type, const void * i_key_value )
			: _iteration_type( &iteration_type )
	{
		_assign_collection( collection_type, collection, i_key_value );
	}

	// Iterator::destructor
	inline Iterator<void>::~Iterator()
	{
		if( _abstract_iterator )
		{
			REFLECTIVE_LIFO_DELETE( _abstract_iterator );
		}
	}

	// Iterator::object
	inline void * Iterator<void>::object() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return _group.curr_in_group;
	}

	// Iterator::type
	inline const reflective::Type & Iterator<void>::type() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *_group.type;
	}

	// Iterator::is_not_over
	inline bool Iterator<void>::is_not_over() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator::operator bool
	inline Iterator<void>::operator bool() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator<void>::_assign_collection( collection_type, collection )
	inline void Iterator<void>::_assign_collection(
		const reflective::Type & collection_type, void * collection )
	{
		_abstract_iterator = nullptr;
		_group.curr_in_group = nullptr;
		_group.end_of_group = nullptr;
		_index_in_group = 0;

		const reflective::ICollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( collection, 0 );	
			if( _abstract_iterator )
			{
				_abstract_iterator->first_group( _group );

				// jump groups not compatible with _iteration_type
				if( _iteration_type != nullptr )
				{
					if( !( *_iteration_type <= *_group.type ) ) do {
						_abstract_iterator->next_group( _group );
						if( _group.curr_in_group >= _group.end_of_group )
							return;
					} while( !( *_iteration_type <= *_group.type ) );
				}
			}
		}
	}

	// Iterator<void>::_assign_collection( collection_type, collection )
	inline void Iterator<void>::_assign_collection( 
		const reflective::Type & collection_type, void * collection, const void * i_key_value )
	{
		_abstract_iterator = nullptr;
		_group.curr_in_group = nullptr;
		_group.end_of_group = nullptr;
		_index_in_group = 0;

		const reflective::ICollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( collection, i_key_value );	
			if( _abstract_iterator )
			{
				_abstract_iterator->first_group( _group );

				if( _iteration_type != nullptr )
				{
					// jump groups not compatible with _iteration_type
					if( !( *_iteration_type <= *_group.type ) ) do {
						_abstract_iterator->next_group( _group );
						if( _group.curr_in_group >= _group.end_of_group )
							return;
					} while( !( *_iteration_type <= *_group.type ) );
				}
			}
		}
	}

	// Iterator<void>::move_next
	inline bool Iterator<void>::move_next()
	{
		REFLECTIVE_ASSERT( is_not_over() );

		// get the next of the group
		const size_t object_size = _group.type->size();
		_group.curr_in_group = mem::address_add( _group.curr_in_group, object_size );
		_index_in_group++;
		if( _group.curr_in_group < _group.end_of_group )
			return true;

		// group finished, ask another one compatible with _iteration_type
		_index_in_group = 0;
		do {
			_abstract_iterator->next_group( _group );
			if( _group.curr_in_group >= _group.end_of_group )
				return false;
		} while( !_iteration_type || !( *_iteration_type <= *_group.type ) );

		return true;
	}

	// Iterator<void>::operator ++
	inline bool Iterator<void>::operator ++ ()
	{
		return move_next();
	}

	// Iterator<void>::postfix operator ++
	inline void Iterator<void>::operator ++ ( int )
	{
		move_next();
	}

	// Iterator<void>::insert( source_object )
	template <class TYPE>
		inline bool Iterator<void>::insert( const TYPE & source_object )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( 1, &source_object, type );
	}

	// Iterator<void>::insert( source_object, object_type )
	inline bool Iterator<void>::insert( const void * source_object, const Type & object_type )
	{
		return multi_insert( 1, source_object, object_type );
	}

	// Iterator<void>::insert( source_object, object_type )
	inline bool Iterator<void>::insert( const void * source_object, const Type & object_type, const void * key_value )
	{
		const bool result = _abstract_iterator->insert( _index_in_group,
			object_type, source_object, 1, key_value, _group );

		if( result )
			_index_in_group = 0;

		return result;
	}

	// Iterator<void>::multi_insert( object_count )
	template <class TYPE>
		inline bool Iterator<void>::multi_insert( size_t object_count )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( object_count, nullptr, type );
	}

	// Iterator<void>::multi_insert( object_count, source_object )
	template <class TYPE>
		inline bool Iterator<void>::multi_insert( size_t object_count, const TYPE & source_object )
	{
		const Type & type = get_type< TYPE >();

		return multi_insert( object_count, &source_object, type );
	}

	// Iterator<void>::multi_insert( object_count, source_object, object_type )
	inline bool Iterator<void>::multi_insert( size_t object_count, const void * source_object, const Type & object_type )
	{
		const bool result = _abstract_iterator->insert( _index_in_group,
			object_type, source_object, object_count, nullptr, _group );

		if( result )
			_index_in_group = 0;

		return result;
	}

	// Iterator<void>::remove
	inline bool Iterator<void>::remove()
	{
		return multi_remove( 1 );
	}

	// Iterator<void>::multi_remove
	inline bool Iterator<void>::multi_remove( size_t object_count )
	{
		const bool result = _abstract_iterator->remove( _index_in_group, object_count, _group );

		if( result )
			_index_in_group = 0;

		return result;
	}

} // namespace reflective

