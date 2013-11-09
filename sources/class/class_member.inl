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
	inline ClassMember::Attributes operator | ( ClassMember::Attributes op1, ClassMember::Attributes op2 )
	{
		const int int_op1 = static_cast<int>( op1 );
		const int int_op2 = static_cast<int>( op2 );
		return (ClassMember::Attributes)( int_op1 | int_op2 );
	}

	// ClassMember::constructor
	inline ClassMember::ClassMember( SymbolName name, Attributes attributes )
		: Symbol( name ), _attributes( attributes )
	{
	}

	// ClassMember::attributes
	inline ClassMember::Attributes ClassMember::attributes() const
	{
		return _attributes;
	}

	// ClassMember::is_public
	inline bool ClassMember::is_public() const
	{
		return ( _attributes & (PROTECTED | PRIVATE) ) == 0;
	}

	// ClassMember::is_private
	inline bool ClassMember::is_private() const
	{
		return (_attributes & PRIVATE) != 0;
	}

	// ClassMember::is_protected
	inline bool ClassMember::is_protected() const
	{
		return (_attributes & PROTECTED) != 0;
	}

	// ClassMember::is_back_reference
	inline bool ClassMember::is_back_reference() const
	{
		return (_attributes & BACK_REFERENCE) != 0;
	}


	// ClassMember::less_than_or_equal
	inline bool ClassMember::Comparer::less_than_or_equal( const ClassMember & first, const ClassMember & second )
	{
		REFLECTIVE_ASSERT( PROTECTED == 1 << 0 );
		REFLECTIVE_ASSERT( PRIVATE == 1 << 1 );
		return (first.attributes() & 2) >= (second.attributes() & 2);
	}


	// count_public_members
	template < class CLASS_MEMBER >
		size_t count_public_members( 
			const SymbolList< CLASS_MEMBER, false, ClassMember::Comparer > & member_list )
	{
		size_t matching_count = 0;

		const size_t total_count = member_list.count();
		if( total_count == 0 )
			return 0;

		size_t index = 0;
		do {

			if( member_list[ index ].is_public() )
				matching_count++;

		} while( ++index < total_count );

		return matching_count;
	}

} // namespace reflective
