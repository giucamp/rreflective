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
	// FromStringBuffer
	class FromStringBuffer
	{
	public:

		// constructor
		FromStringBuffer( const char * chars, size_t length );
		FromStringBuffer( const FromStringBuffer & source );

		FromStringBuffer & operator = ( const FromStringBuffer & source );

		void set( const char * chars, size_t length );

		// remaining_length
		const char * chars() const;
		size_t remaining_length() const;

		enum AcceptOptions
		{
			eOptionsNone =			0,
			eIgnoreCase =			(1 << 0),
			eIgnoreLeadingSpaces =	(1 << 1),
			eOptionsAll =			(1 << 2) - 1,
		};

		char consume_char();

		// accept		
		bool accept( char character, AcceptOptions options = eOptionsNone );
		bool accept( const char * null_terminated_string, AcceptOptions options = eOptionsNone );
		bool accept( const char * chars, size_t length, AcceptOptions options = eOptionsNone );
		template <size_t LENGTH> bool accept_literal( const char (&string_literal)[ LENGTH ], AcceptOptions options = eOptionsNone );

		// accept_from_end
		bool accept_from_end( const char * chars, size_t length, AcceptOptions options = eOptionsNone );
		template <size_t LENGTH> bool accept_literal_from_end( const char (&string_literal)[ LENGTH ], AcceptOptions options = eOptionsNone );

		template <size_t TABLE_LENGTH>
			int accept_indexed( const char * (&string_table)[ TABLE_LENGTH ], AcceptOptions options = eOptionsNone );

		int accept_indexed( const char * const * string_table, size_t table_length, AcceptOptions options = eOptionsNone );

		bool accept_word( StaticConstString * out_result, AcceptOptions options = eOptionsNone );

		bool accept_whitespaces();
	
	private: // data members
		const char * m_curr_char;
		size_t m_remaining_length;
	};

	// AcceptOptions operator |
	inline FromStringBuffer::AcceptOptions operator | ( FromStringBuffer::AcceptOptions first, FromStringBuffer::AcceptOptions second )
	{
		REFLECTIVE_ASSERT( ( first & (~FromStringBuffer::eOptionsAll) ) == 0 );
		REFLECTIVE_ASSERT( ( second & (~FromStringBuffer::eOptionsAll) ) == 0 );
		const int first_int = first;
		const int second_int = second;
		return static_cast<FromStringBuffer::AcceptOptions>( first_int | second_int );
	}


} // namespace reflective

