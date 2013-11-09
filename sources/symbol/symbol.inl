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
	// Symbol::constructor( name )
	inline Symbol::Symbol( const SymbolName & name )
		: _name( name ), _type_id( eTypeSymbol )
	{
		#if REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES && REFLECTIVE_ENABLE_ASSERT
			dbg_check_sumbol_name( name.string() );
		#endif
	}

	// Symbol::name
	inline const SymbolName & Symbol::name() const
	{
		return _name;
	}

	// comparaison
	inline bool Symbol::operator == ( const Symbol & op ) const
	{
		#if REFLECTIVE_IS_DEBUG
			const bool ref_eq = this == &op;
			const bool name_eq = _name == op._name;
			REFLECTIVE_ASSERT( ref_eq == name_eq );
			REFLECTIVE_UNUSED( name_eq );
			return ref_eq;
		#else
			return this == &op;
		#endif
	}

	// Symbol::operator !=
	inline bool Symbol::operator != ( const Symbol & op ) const
	{
		#if REFLECTIVE_IS_DEBUG
			const bool ref_eq = this == &op;
			const bool name_eq = _name == op._name;
			REFLECTIVE_ASSERT( ref_eq == name_eq );
			return !ref_eq;
		#else
			return this != &op;
		#endif
	}

	// Symbol::set_type_id
	inline void Symbol::set_type_id( int type_id )
	{
		_type_id = type_id;		
	}

	// Symbol::set_custom_type
	inline void Symbol::set_custom_type( int id, const Type * type )
	{
		symbol_types[ id ] = type;
	}
}
