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
	class Enum;

	// Enum
	class Enum : public Type
	{
	public:	

		// Member
		class Member : public Symbol
		{
		public:
			Member( SymbolName name, int value );
			int value() const;

		private: // data members
			int _value;
		};

		// MemberComparer
		class MemberComparer
		{
		public:
			enum { Sort = 1 };
			bool less_than_or_equal( const Member & first, const Member & second )
				{ return first.value() <= second.value(); }
		};

		// MemberList
		typedef SymbolList< Member, false, MemberComparer > MemberList;

		// members
		MemberList & edit_members();
		const MemberList & members() const;

		// finds
		const Member * find_member_by_value( int value, size_t * inout_index = nullptr ) const; 
			/* if inout_index is not nullptr, the search will start from *inout_index, and if
			   the member is found, inout_index wil be filled with the index of the member + 1.
			   To get multiple members as result of the search, initialize a size_t with zero, 
			   and pass it as inout_index until nullptr is returned. */

		// constructors
		Enum( const StaticConstString & parent_namespace_path, const SymbolName & name );
		~Enum();

		// from_string
		static bool from_string( FromStringBuffer & source_buffer, const Type & type,
			void * object, StringOutputStream & error_buffer );

		// to_string
		static void to_string( StringOutputStream & dest_buffer, const Type & type, const void * object );

	private: // data members
		MemberList _members;
	};

	// new_enum( namespace_path, name, [members] )
	Enum * new_enum( const StaticConstString & parent_namespace_path, const SymbolName & name );
	template < size_t ITEM_COUNT > 
		Enum * new_enum( const StaticConstString & parent_namespace_path, const SymbolName & name, const Enum::Member * (&members)[ ITEM_COUNT ] );	

	// new_enum_member( name, value )
	Enum::Member * new_enum_member( SymbolName name, int value );
}

namespace reflective_externals
{
	// reflection of reflective::Enum
	void init_type( reflective::Type * volatile * o_result,
		reflective::Enum * null_pointer_1,
		reflective::Enum * null_pointer_2 );

	// reflection of reflective::Enum::Member
	void init_type( reflective::Type * volatile * o_result,
		reflective::Enum::Member * null_pointer_1,
		reflective::Enum::Member * null_pointer_2 );

} // namespace reflective_externals

namespace reflective_externals
{
	
	// reflection of reflective::Enum
	void init_type( reflective::Type * volatile * o_result,
		reflective::Enum * null_pointer_1,
		reflective::Enum * null_pointer_2 );

} // namespace reflective_externals
