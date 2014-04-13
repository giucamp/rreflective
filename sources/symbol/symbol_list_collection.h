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
			: public reflective::ICollectionHandler
	{
	public:
		_CollectionHandler();

		// query_item_count
		bool query_item_count( const void * collection_object, size_t * out_count ) const;

		// query_item_base_type
		const Type * query_item_base_type( const void * collection_object, 
			bool * out_allow_derived_types ) const;

		// create_iterator
		reflective::IIterator * create_iterator( void * i_collection_object, const void * i_key_value ) const;

		// resize
		bool resize( void * collection_object, size_t new_size ) const
		{
			REFLECTIVE_UNUSED_2( collection_object, new_size );
			return false;
		}

		// get_indices_info
		bool get_key_type( const void * i_collection_object, uint32_t * o_flags, QualifiedType * o_index_type ) const
		{
			REFLECTIVE_UNUSED( i_collection_object );
			*o_flags = ePositionalIndex;
			*o_index_type = safe_get_qualified_type<SymbolName>();
			return true;
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
				: public reflective::IIterator
	{
	public:
		_Iterator( const SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > & symbol_set, size_t offset_index );

		void first_group( ConstGroup & out_group );
		void next_group( ConstGroup & out_group );

		void first_group( Group & out_group )
		{
			REFLECTIVE_UNUSED( out_group );
			REFLECTIVE_ASSERT( false ); // unsupported
		}

		void next_group( Group & out_group )
		{
			REFLECTIVE_UNUSED( out_group );
			REFLECTIVE_ASSERT( false ); // unsupported
		}

		bool insert( 
			size_t group_offset_index, const Type & items_type, 
			const void * source_object, size_t item_count,
			const void * i_key_value, Group & out_curr_group )
		{
			REFLECTIVE_UNUSED_4( group_offset_index, items_type, source_object, item_count );
			REFLECTIVE_UNUSED_2( out_curr_group, i_key_value );
			return false; // unsupported
		}

		bool remove( size_t offset_index, size_t item_count,
			Group & out_curr_group )
		{
			REFLECTIVE_UNUSED_2( offset_index, item_count );
			REFLECTIVE_UNUSED( out_curr_group );
			return false; // unsupported
		}

	private:
		_Iterator & operator = ( const _Iterator & ); // not supported

	private: // data members
		const SymbolList<SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER > & _symbol_set;
		size_t _index;
	};

} // namespace reflective
