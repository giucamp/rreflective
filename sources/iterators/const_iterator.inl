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
			inline Iterator<const TYPE>::Iterator( const COLLECTION_TYPE & collection )
				: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection, iteration_type )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<const TYPE>::Iterator( const COLLECTION_TYPE & collection,
				const reflective::Type & iteration_type )
					: _iteration_type( iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection );
	}

	// Iterator::constructor( collection_type, collection )
	template <class TYPE>
		inline Iterator<const TYPE>::Iterator( const reflective::Type & collection_type, void * collection )
			: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( collection_type, collection );
	}

	// Iterator::constructor( collection_type, collection, iteration_type )
	template <class TYPE>
		inline Iterator<const TYPE>::Iterator( const reflective::Type & collection_type, void * collection,
			const reflective::Type & iteration_type )
				: _iteration_type( iteration_type )
	{
		_assign_collection( collection_type, collection );
	}


	// Iterator::constructor( collection, i_key_value )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<const TYPE>::Iterator( const COLLECTION_TYPE & collection, const void * i_key_value )
				: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, i_key_value );
	}

	// Iterator::constructor( collection, iteration_type, i_key_value )
	template <class TYPE>
		template <class COLLECTION_TYPE>
			inline Iterator<const TYPE>::Iterator( const COLLECTION_TYPE & collection, const reflective::Type & iteration_type,
				const void * i_key_value )
					: _iteration_type( iteration_type )
	{
		_assign_collection( get_type<COLLECTION_TYPE>(), &collection, i_key_value );
	}

	// Iterator::constructor( collection_type, collection, i_key_value )
	template <class TYPE>
		inline Iterator<const TYPE>::Iterator( const reflective::Type & collection_type, void * collection,
			const void * i_key_value )
				: _iteration_type( get_type<TYPE>() )
	{
		_assign_collection( collection_type, collection, i_key_value );
	}

	// Iterator::constructor( collection_type, collection, iteration_type, i_key_value )
	template <class TYPE>
		inline Iterator<const TYPE>::Iterator( const reflective::Type & collection_type, void * collection,
			const reflective::Type & iteration_type, const void * i_key_value )
				: _iteration_type( iteration_type )
	{
		_assign_collection( collection_type, collection, i_key_value );
	}

	// Iterator::destructor
	template <class TYPE>
		inline Iterator<const TYPE>::~Iterator()
	{
		if( _abstract_iterator )
		{
			REFLECTIVE_LIFO_DELETE( _abstract_iterator );
		}
	}

	// Iterator::type
	template <class TYPE>
		inline const reflective::Type & Iterator<const TYPE>::type() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *_group.type;
	}

	// Iterator::object
	template <class TYPE>
		inline const TYPE & Iterator<const TYPE>::object() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *static_cast<const TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator *
	template <class TYPE>
		inline const TYPE & Iterator<const TYPE>::operator * () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return *static_cast<const TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator ->
	template <class TYPE>
		inline const TYPE * Iterator<const TYPE>::operator -> () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return static_cast<const TYPE*>( _group.curr_in_group );
	}

	// Iterator::operator *
	template <class TYPE>
		inline Iterator<const TYPE>::operator const TYPE * () const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return static_cast<const TYPE*>( _group.curr_in_group );
	}

	// Iterator::is_not_over
	template <class TYPE>
		inline bool Iterator<const TYPE>::is_not_over() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator::operator bool
	template <class TYPE>
		inline Iterator<const TYPE>::operator bool() const
	{
		return _group.curr_in_group < _group.end_of_group;
	}

	// Iterator<const TYPE>::_assign_collection( collection_type, collection )
	template <class TYPE>
		inline void Iterator<const TYPE>::_assign_collection(
			const reflective::Type & collection_type, const void * collection )
	{
		_abstract_iterator = nullptr;
		_group.curr_in_group = nullptr;
		_group.end_of_group = nullptr;

		const reflective::ICollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( const_cast<void*>( collection ), 0 );
			if( _abstract_iterator )
			{
				_abstract_iterator->first_group( _group );
				if( _group.curr_in_group >= _group.end_of_group )
					return;

				// jump groups not compatible with _iteration_type
				if( !( _iteration_type <= *_group.type ) ) do {
					_abstract_iterator->next_group( _group );
					if( _group.curr_in_group >= _group.end_of_group )
						return;
				} while( !( _iteration_type <= *_group.type ) );
			}
		}
	}

	// Iterator<const TYPE>::_assign_collection( collection_type, collection )
	template <class TYPE>
		inline void Iterator<const TYPE>::_assign_collection(
			const reflective::Type & collection_type, const void * collection,
			const void * i_key_value )
	{
		_abstract_iterator = nullptr;
		_group.curr_in_group = nullptr;
		_group.end_of_group = nullptr;

		const reflective::ICollectionHandler * collection_handler = collection_type.collection_handler();
		if( collection_handler )
		{
			_abstract_iterator = collection_handler->create_iterator( collection, i_key_value );
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

	// Iterator<const TYPE>::move_next
	template <class TYPE>
		inline bool Iterator<const TYPE>::move_next()
	{
		REFLECTIVE_ASSERT( is_not_over() );

		// get the next of the group
		const size_t object_size = _group.type->size();
		_group.curr_in_group = mem::address_add( _group.curr_in_group, object_size );
		if( _group.curr_in_group < _group.end_of_group )
			return true;

		// group finished, ask another compatible with _iteration_type
		do {
			_abstract_iterator->next_group( _group );
			if( _group.curr_in_group >= _group.end_of_group )
				return false;
		} while( !( _iteration_type <= *_group.type ) );

		return true;
	}

	// Iterator<const TYPE>::operator ++
	template <class TYPE>
		inline bool Iterator<const TYPE>::operator ++ ()
	{
		return move_next();
	}

	// Iterator<const TYPE>::postfix operator ++
	template <class TYPE>
		inline void Iterator<const TYPE>::operator ++ ( int )
	{
		move_next();
	}


} // namespace reflective

