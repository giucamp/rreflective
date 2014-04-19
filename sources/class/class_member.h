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
	// ClassMember
	class ClassMember : public Symbol
	{
	public:

		enum Attributes
		{
			NO_ATTRIBUTES		= 0,
			PROTECTED			= 1 << 0,
			PRIVATE				= 1 << 1,			
			BACK_REFERENCE		= 1 << 2,
			READONLY			= 1 << 3,
			IDENTIFIER			= 1 << 4,
			TRANSIENT			= 1 << 5,
			ALL_ATTRIBUTES		= (1 << 6) - 1
		};		

		Attributes attributes() const;
		bool is_public() const;
		bool is_private() const;
		bool is_protected() const;
		bool is_back_reference() const;

		const char * visibility_as_string() const;

		class Comparer
		{
		public:
			enum { Sort = 1 };
			bool less_than_or_equal( const ClassMember & first, const ClassMember & second );
		};

		virtual ~ClassMember() {}

	protected:

		ClassMember( SymbolName name, Attributes attributes );			

	private: // data members
		Attributes _attributes;
	};

	// count_public_members
	template < class CLASS_MEMBER >
		size_t count_public_members( 
			const SymbolList< CLASS_MEMBER, false, ClassMember::Comparer > & member_list );

	ClassMember::Attributes operator | ( ClassMember::Attributes op1, ClassMember::Attributes op2 );
}

namespace reflective_externals
{
	// reflection of reflective::ClassMember::Attributes
	void init_type( reflective::Type * volatile * o_result,
		reflective::ClassMember::Attributes * null_pointer_1,
		reflective::ClassMember::Attributes * null_pointer_2 );

	// reflection of reflective::ClassMember
	void init_type( reflective::Type * volatile * o_result,
		reflective::ClassMember * null_pointer_1,
		reflective::ClassMember * null_pointer_2 );
}
