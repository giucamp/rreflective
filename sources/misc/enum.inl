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
	// new_enum_member
	inline Enum::Member * new_enum_member( SymbolName name, int value )
	{		
		Enum::Member * result = REFLECTIVE_LIFO_NEW( Enum::Member, name, value );
		return result;
	}

	// new_enum
	template < size_t ITEM_COUNT > 
		inline Enum * new_enum( const StaticConstString & parent_namespace_path, const SymbolName & name, const Enum::Member * (&members)[ ITEM_COUNT ] )
	{
		Enum * result = new_enum( parent_namespace_path, name );
		result->edit_members().assign( members );
		return result;
	}

	// Enum::edit_members
	inline Enum::MemberList & Enum::edit_members()
	{
		return _members;
	}

	// Enum::members
	inline const Enum::MemberList & Enum::members() const
	{
		return _members;
	}

	// Enum::destructor
	inline Enum::~Enum()
	{

	}

	// Enum::Member::constructor
	inline Enum::Member::Member( SymbolName name, int value )
		: Symbol( name ), _value( value )
	{
		set_type_id( eTypeEnumMember );
	}

	// Enum::Member::value
	inline int Enum::Member::value() const
	{
		return _value;
	}
}
