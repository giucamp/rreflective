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
	/* Iterator - allows the iteration of any reflected type. If the type is not iterable the
			iteration wll be empty, that is is_over() and operator bool return true soon after 
			the construction. */
	template <>
		class Iterator<void>
	{
	public:

		/* 	constructor( [collection_type], collection, [iteration_type], [offset_index] )
		
			collection - object to iterate. If the object is not a collection, that is its type
				does not implement the IIterator interface, the iteration is empty.
			collection_type - optional - the type of the collection. If omitted, the type of
				the collection is deduced by the 'collection' parameter.
			iteration_type - optional - the type of iterated objects. 
				If provided, a pointer to the iteration_type type must should be implicitly castable to a pointer to TYPE,
				otherwise the iteration is empty. 
				Only objects of a type implicitly castable to iteration_type are iterated. The 
				other objects are skipped. */

		// constructor( collection )
		template <class COLLECTION_TYPE> 
			Iterator( const COLLECTION_TYPE & collection );

		// constructor( collection, iteration_type )
		template <class COLLECTION_TYPE> 
			Iterator( const COLLECTION_TYPE & collection, const reflective::Type & iteration_type );

		// constructor( collection_type, collection )
		Iterator( const reflective::Type & collection_type, void * collection );

		// constructor( collection_type, collection, iteration_type )
		Iterator( const reflective::Type & collection_type, void * collection, const reflective::Type & iteration_type );

		// constructor( collection, i_key_value )
		template <class COLLECTION_TYPE> 
			Iterator( const COLLECTION_TYPE & collection, 
				const void * i_key_value );

		// constructor( collection, iteration_type, i_key_value )
		template <class COLLECTION_TYPE> 
			Iterator( const COLLECTION_TYPE & collection, const reflective::Type & iteration_type,
				const void * i_key_value );

		// constructor( collection_type, collection, i_key_value )
		Iterator( const reflective::Type & collection_type, void * collection,
			const void * i_key_value );

		// constructor( collection_type, collection, iteration_type, i_key_value )
		Iterator( const reflective::Type & collection_type, void * collection, const reflective::Type & iteration_type,
			const void * i_key_value );

		// destructor
		~Iterator();

		// access to object
		const QualifiedType & qualified_type() const;
		void * object() const;

		// move_next
		bool move_next();
		bool operator ++ ();
		void operator ++ ( int );

		// is_not_over
		bool is_not_over() const;
		operator bool() const;

		// insert
		template <class TYPE> bool insert();
		template <class TYPE> bool insert( const TYPE & source_object );
		bool insert( const void * source_object, const Type & object_type );
		bool insert( const void * source_object, const Type & object_type, const void * key_value );

		// multi_insert
		template <class TYPE> bool multi_insert( size_t object_count );
		template <class TYPE> bool multi_insert( size_t object_count, const TYPE & source_object );
		bool multi_insert( size_t object_count, const void * source_object, const Type & object_type );

		// remove - multi_remove
		bool remove();
		bool multi_remove( size_t object_count );

	private: // internal services		
		void _assign_collection( const reflective::Type & collection_type, void * collection );
		void _assign_collection( const reflective::Type & collection_type, void * collection, const void * i_key_value );

		Iterator & operator = ( const Iterator & ); // not supported

	private: // data members				
		reflective::IIterator::Group _group;
		const reflective::Type * _iteration_type;
		reflective::IIterator * _abstract_iterator;
		size_t _index_in_group;
	};

} // namespace reflective
