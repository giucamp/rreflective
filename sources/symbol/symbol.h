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
	// SymbolName
	typedef uint32_t SymbolNameHash;
	#if	REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES
		typedef NameIdentifier< SymbolNameHash, StaticConstString > SymbolName;
	#else
		typedef NameIdentifier< SymbolNameHash, void > SymbolName;
	#endif
		
	class Type;

	/** Base for most class defined in reflective. */
	class Symbol
	{	
	public:

		// name
		const SymbolName & name() const;

		// comparison
		bool operator == ( const Symbol & op ) const;
		bool operator != ( const Symbol & op ) const;

		static const Type & resolve_type( const Type & base_type, const void * object );

		static int query_custom_type_id();
		static void set_custom_type( int id, const Type * type );

		int get_type_id() const			{ return m_type_id; }

		#if REFLECTIVE_ENABLE_ASSERT
			static void dbg_check_sumbol_name( const StaticConstString & name );
		#endif

	protected:
		
		void set_type_id( int type_id );

		Symbol( const SymbolName & name ); // the name can be empty, in which case the symbol is unnamed
		
		virtual ~Symbol() {}

	private:
		Symbol & operator = ( const Symbol & );

	private: // data members
		const SymbolName m_name;
		int m_type_id;

	public:
		static const Type * symbol_types[ eMaxTypeIdCount ];
	};

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Symbol
	void init_type( reflective::Type * volatile * o_result,
		reflective::Symbol * null_pointer_1,
		reflective::Symbol * null_pointer_2 );

} // namespace reflective_externals

