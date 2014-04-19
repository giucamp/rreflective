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

namespace reflective
{
	// MemberSetType::_CollectionHandler::constructor
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
			inline SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler::_CollectionHandler()
				: ICollectionHandler()
	{
	}

	// SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler::query_item_base_type
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
			const reflective::Type * SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler::query_item_base_type(
				const void * collection_object,
				bool * out_allow_derived_types ) const
	{
		REFLECTIVE_UNUSED( collection_object );

		*out_allow_derived_types = true;
		return &get_type<SYMBOL>();
	}

	// MemberSetType::_CollectionHandler::query_item_count
	template < class SYMBOL, bool ALLOW_DUPLICATES, class SYMBOL_COMPARER >
			inline bool SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler::query_item_count(
				const void * collection_object, size_t * out_count ) const
	{
		const reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > & set =
			*static_cast<const reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > *>( collection_object );
		*out_count = set.count();
		return true;
	}

	// MemberSetType::_CollectionHandler::create_iterator
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
			inline reflective::IIterator * SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler::create_iterator(
				void * collection_object, const void * i_key_value ) const
	{
		const reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > & symbol_set =
			*static_cast<const reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >*>( collection_object );

		_Iterator * iterator = REFLECTIVE_LIFO_NEW( _Iterator, symbol_set, *static_cast<const size_t*>( i_key_value ) );

		return iterator;
	}

	// MemberSetType::_Iterator::constructor
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
			inline SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator::_Iterator(
				const reflective::SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > & symbol_set, size_t offset_index )
					: _symbol_set( symbol_set )
	{
		_index = offset_index;
	}

	// MemberSetType::_Iterator::first_group
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
		void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator::first_group(
			typename SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator::ConstGroup & out_group )
	{
		const size_t count = _symbol_set.count();

		_index = 0;
		if( count > 0 )
		{
			const SYMBOL * member = &_symbol_set[ 0 ];
			out_group.curr_in_group = member;
			out_group.end_of_group = member + 1;
			out_group.qualified_type = get_qualified_type<SYMBOL>();
		}
		else
		{
			out_group.curr_in_group = nullptr;
			out_group.end_of_group = nullptr;
		}
	}

	// MemberSetType::_Iterator::next_group
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
		void SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator::next_group(
			typename SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator::ConstGroup & out_group )
	{
		const size_t count = _symbol_set.count();

		++_index;
		if( _index < count )
		{
			const SYMBOL * member = &_symbol_set[ _index ];
			out_group.curr_in_group = member;
			out_group.end_of_group = member + 1;
			out_group.qualified_type = get_qualified_type<SYMBOL>();
		}
		else
		{
			out_group.curr_in_group = nullptr;
			out_group.end_of_group = nullptr;
		}
	}
}
