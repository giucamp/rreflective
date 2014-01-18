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
	// SymbolList::allow_duplicates
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			const bool SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::allow_duplicates = ALLOW_DUPLICATES;

	// SymbolList::constructor
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			inline SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::SymbolList()
				: _items( nullptr ), _count( 0 )
	{
	}

	// SymbolList::destructor
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			inline SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::~SymbolList()
	{
	}

	#ifndef REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS
		#error REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS should be defined
	#endif
	#ifndef REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS
		#error REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS should be defined
	#endif

	// SymbolList::assign( items, count )
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::assign(
				const SYMBOL * * items, size_t count )
	{
		REFLECTIVE_ASSERT( _items == nullptr ); // cannot reassign

		void * allocation = reflective_externals::mem_lifo_alloc(
			alignment_of( SYMBOL * ), count * sizeof( SYMBOL * ) );

		const SYMBOL * * allocated_items = static_cast< const SYMBOL * * >( allocation );

		objects_copy<const SYMBOL *>( allocated_items, allocated_items + count, items );

		_items = allocated_items;
		_count = count;

		#if REFLECTIVE_CHECK_FOR_DUPLICATE_SYMBOLS
			if( !ALLOW_DUPLICATES )
			{
				/* _check_for_duplicates is called even if ALLOW_DUPLICATES is true, so that SymbolName
					can detect hash collision. */
				const size_t max_duplicate_count = 16;
				const SYMBOL * duplicates[ max_duplicate_count ];
				const size_t duplicate_count = _check_for_duplicates( duplicates, max_duplicate_count );				
				REFLECTIVE_ASSERT( duplicate_count == 0 );
				REFLECTIVE_UNUSED( duplicate_count );
			}
		#endif

		#if REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS
			if( SYMBOL_COMPARER::Sort )
			{
				_check_sorting();
			}
		#endif

		#if REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS
			const bool sort = SYMBOL_COMPARER::Sort != 0;
			if( sort )
			{
				_sort();
				#if REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS && REFLECTIVE_IS_DEBUG
					_check_sorting();
				#endif
			}
		#endif
	}

	// SymbolList::assign( items[] )
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			template < size_t ITEM_COUNT >
				inline void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::assign(
					const SYMBOL * (&items)[ ITEM_COUNT ] )
	{
		assign( items, ITEM_COUNT );
	}

	// SymbolList[ index ]
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			inline const SYMBOL & SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::operator [] ( size_t index ) const
	{
		REFLECTIVE_ASSERT( index < _count );
		return *_items[ index ];
	}

	// SymbolList::count
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			inline size_t SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::count() const
	{
		return _count;
	}

	// SymbolList::find
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			const SYMBOL * SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::find( SymbolName name ) const
	{
		size_t index = 0;
		if( _count ) do {
			if( _items[index]->name() == name )
				return _items[index];
		} while( ++index < _count );
		return nullptr;
	}

	// SymbolList::find
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			const SYMBOL * SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::find( SymbolNameHash name_hash ) const
	{
		size_t index = 0;
		if( _count ) do {
			if( _items[index]->name().uint_hash() == name_hash )
				return _items[index];
		} while( ++index < _count );
		return nullptr;
	}


	// SymbolList::_check_for_duplicates
	#if REFLECTIVE_CHECK_FOR_DUPLICATE_SYMBOLS
		template < class SYMBOL, bool ALLOW_DUPLICATES,
			class SYMBOL_COMPARER >
				size_t SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_check_for_duplicates(
					const SYMBOL * * result, size_t max_result_count ) const
		{
			size_t result_count = 0;
			size_t outer_index = 0;
			if( _count ) do {

				size_t inner_index = outer_index + 1;
				if( inner_index < _count ) do {

					if( _items[inner_index]->name() == _items[outer_index]->name() )
					{
						if( result_count < max_result_count )
							result[ result_count++ ] = _items[inner_index];
						if( result_count < max_result_count )
							result[ result_count++ ] = _items[outer_index];
					}

				} while( ++inner_index < _count );
			} while( ++outer_index < _count );
			return result_count;
		}

	#endif // #if REFLECTIVE_CHECK_FOR_DUPLICATE_SYMBOLS

	// SymbolList::_check_sorting
	#if REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS
		template < class SYMBOL, bool ALLOW_DUPLICATES,
			class SYMBOL_COMPARER >
				void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_check_sorting() const
		{
			SYMBOL_COMPARER compaper;
			size_t index = 1;
			if( _count > 1 ) do {
				const bool less_than_or_equal = compaper.less_than_or_equal(
					*_items[index - 1], *_items[index] );
				REFLECTIVE_ASSERT( less_than_or_equal ); /* the member of this SymbolList 
					are not correctly sorted and REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS
					is defined as true.*/
				REFLECTIVE_UNUSED( less_than_or_equal );
			} while( ++index < _count );
		}
	#endif

	// SymbolList::_sort
	#if REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS

		template < class SYMBOL, bool ALLOW_DUPLICATES,
			class SYMBOL_COMPARER >
				void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_sort()
		{
			SYMBOL_COMPARER compaper;
			size_t index = 1;
			if( _count > 1 ) do {

				// _items[ index ] must be inserted in the correct position
				// in the sorted array from 0 to index
				const SYMBOL & member = *_items[ index ];

				size_t inner_index = index - 1;

				bool done = false;

				do {
					const bool less_than_or_equal = compaper.less_than_or_equal(
						*_items[ inner_index ], member );
					if( !less_than_or_equal )
					{
						_items[ inner_index + 1 ] = _items[ inner_index ];
						if( inner_index == 0 )
							done = true;
						inner_index = inner_index - 1;
					}
					else
						done = true;
				} while( !done );

				_items[ inner_index + 1 ] = &member;

			} while( ++index < _count );
		}

	#endif // #if REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS

	// SymbolList::_collection_handler
	template < class SYMBOL, bool ALLOW_DUPLICATES, class SYMBOL_COMPARER >
		typename SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler
			SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_collection_handler;

	// SymbolList::access_collection_handler
	template < class SYMBOL, bool ALLOW_DUPLICATES, class SYMBOL_COMPARER >
		inline CollectionHandler & SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::access_collection_handler()
	{
		return _collection_handler;
	}


} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >
	template < class SYMBOL, bool ALLOW_DUPLICATES,
		class SYMBOL_COMPARER >
			reflective::Class * init_type(
				reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > * null_pointer_1,
				reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > ThisClass;

		static Class * result = nullptr;
		if( result != nullptr )
			return result;

		// dependencies
		TypeContainer< size_t >::init_type();

		// template arguments
		const TemplateParameter template_parameters[] =
		{
			TemplateParameter( "SYMBOL", safe_get_qualified_type<SYMBOL>() ),
			TemplateParameter( "ALLOW_DUPLICATES", ALLOW_DUPLICATES ),
		};

		Class * class_object = new_class<ThisClass>( "reflective", "SymbolList", template_parameters );

		// properties
		const Property * properties[] =
		{
			new_property<ThisClass>( "count", &ThisClass::count ),
		};

		// assign members
		class_object->assign_properties( properties );

		class_object->set_collection_handler( &ThisClass::access_collection_handler() );

		// assign and return type
		result = class_object;
		return result;
	}

} // namespace reflective_externals
