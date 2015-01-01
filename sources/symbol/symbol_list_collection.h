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
	// SymbolList<SYMBOL>::_CollectionHandler
	template < class SYMBOL, bool ALLOW_DUPLICATES,
			class SYMBOL_COMPARER >
		class SymbolList<SYMBOL,ALLOW_DUPLICATES, SYMBOL_COMPARER >::_CollectionHandler 
			: public reflective::CollectionHandler
	{
	public:

		typedef reflective::SymbolList<SYMBOL,ALLOW_DUPLICATES, SYMBOL_COMPARER > SymbolList;

		_CollectionHandler() : CollectionHandler( sizeof(_Iterator), alignment_of(_Iterator) ) {}

		Flags get_flags( const void * /*i_colection_object*/ ) const
			{ return eSupportPositionalIndex | eSupportGetCount | eAllowDerivedItems; }

		QualifiedType get_key_type( const void * /*i_collection_object*/ ) const
			{ return QualifiedType::s_empty; }

		QualifiedType get_item_type( const void * /*i_collection_object*/ ) const
			{ return safe_get_qualified_type<SYMBOL*>(); }

		size_t get_count( const void * i_collection_object ) const
		{ 
			const SymbolList & list = *static_cast<const SymbolList*>(i_collection_object);
			return list.count(); 
		}

		ObjectPointerWrapper get_at_index( const void * i_collection_object, size_t i_index ) const
		{
			const SymbolList & list = *static_cast<const SymbolList*>(i_collection_object);
			return ObjectPointerWrapper( const_cast<SYMBOL*>( &(list[i_index]) ), get_qualified_type<SYMBOL*>() ); 
		}		

		AbstractIterator * construct_iterator( void * i_collection_object, void * i_iterator_buffer ) const
		{
			const SymbolList & list = *static_cast<const SymbolList*>(i_collection_object);
			return new( i_iterator_buffer ) _Iterator( &list );
		}

		bool register_watch( Watch * watch, void * collection_object ) const
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
			return false;
		}

		void unregister_watch( Watch * watch, void * collection_object ) const
		{
			REFLECTIVE_UNUSED_2( watch, collection_object );
		}
	};

	// SymbolList<SYMBOL>::_Iterator
	template < class SYMBOL, bool ALLOW_DUPLICATES,	class SYMBOL_COMPARER >
			class SymbolList<SYMBOL,ALLOW_DUPLICATES, SYMBOL_COMPARER >::_Iterator 
				: public reflective::AbstractIterator
	{
	public:
		_Iterator( const SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > * i_symbol_set )
			: m_symbol_set( i_symbol_set ), m_index(0)
		{
			if( m_index < m_symbol_set->count() )
			{
				SYMBOL & symbol = const_cast<SYMBOL &>( (*m_symbol_set)[m_index] );
				set_item( &symbol, get_qualified_type<SYMBOL*>(), nullptr );
			}
			else
				set_item_null();
		}

		void move_next()
		{
			m_index++;
			if( m_index < m_symbol_set->count() )
			{
				SYMBOL & symbol = const_cast<SYMBOL &>( (*m_symbol_set)[m_index] );
				set_item( &symbol, get_qualified_type<SYMBOL*>(), nullptr );
			}
			else
				set_item_null();
		}
		
	private: // data members
		const SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > * m_symbol_set;
		size_t m_index;
	};

} // namespace reflective
