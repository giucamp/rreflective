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
	// DefaultCompaper
	class DefaultCompaper
	{
	public:
		enum { Sort = 0 };
		template < class ANY>
		bool less_than_or_equal( const ANY & first, const ANY & second )
			{ return &first < &second; }
	};

	// SymbolList< SYMBOL, ALLOW_DUPLICATES, SYMBOL_COMPARER >
	template < class SYMBOL, bool ALLOW_DUPLICATES, class SYMBOL_COMPARER = DefaultCompaper >
		class SymbolList
	{
	public:

		typedef SYMBOL Symbol;
		static const bool allow_duplicates;

		// constructor			// destructor
		SymbolList();			~SymbolList();

		// item access
		const SYMBOL & operator [] ( size_t index ) const;
		size_t count() const;

		// find
		const SYMBOL * find( SymbolName name ) const;
		const SYMBOL * find( SymbolNameHash name_hash ) const;

		// assign
		void assign( const SYMBOL * * items, size_t count );
		template < size_t ITEM_COUNT > void assign( const SYMBOL * (&items)[ ITEM_COUNT ] );

		static CollectionHandler & access_collection_handler();

	protected:
		class _CollectionHandler;
		class _Iterator;

	private: // internal services
		#if REFLECTIVE_CHECK_FOR_DUPLICATE_SYMBOLS
			size_t _check_for_duplicates( const SYMBOL * * result, size_t max_result_count ) const;
		#endif
		#if REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS
			void _check_sorting() const;
		#endif
		#if REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS
			void _sort();
		#endif
		static _CollectionHandler _collection_handler;

	private: // data members
		const SYMBOL * * _items;
		size_t _count;
	};

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::SymbolList<SYMBOL>
	template < class SYMBOL, bool ALLOW_DUPLICATES,
			class SYMBOL_COMPARER >
		reflective::Class * init_type(
			reflective::SymbolList<SYMBOL,ALLOW_DUPLICATES,SYMBOL_COMPARER > * null_pointer_1,
			reflective::SymbolList<SYMBOL,ALLOW_DUPLICATES,SYMBOL_COMPARER > * null_pointer_2 );

} // namespace reflective_externals
