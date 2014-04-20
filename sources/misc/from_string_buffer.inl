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

	// FromStringBuffer::constructor
	inline FromStringBuffer::FromStringBuffer( const char * chars, size_t length )
		: _buffer( chars ), _length( length )
	{
	}

	// FromStringBuffer::copy constructor
	inline FromStringBuffer::FromStringBuffer( const FromStringBuffer & source )
		: _buffer( source._buffer ), _length( source._length )
	{
	}

	// FromStringBuffer::assignment
	inline FromStringBuffer & FromStringBuffer::operator = ( const FromStringBuffer & source )
	{
		_buffer = source._buffer;
		_length = source._length;
		return *this;
	}

	// FromStringBuffer::set
	inline void FromStringBuffer::set( const char * chars, size_t length )
	{
		_buffer = chars;
		_length = length;
	}

	// FromStringBuffer::chars
	inline const char * FromStringBuffer::chars() const
	{
		return _buffer;
	}

	// FromStringBuffer::remaining_length
	inline size_t FromStringBuffer::remaining_length() const
	{
		return _length;
	}

	// FromStringBuffer::accept
	inline bool FromStringBuffer::accept( const char * null_terminated_string, AcceptOptions options )
	{
		const size_t string_length = strlen( null_terminated_string );
		return accept( null_terminated_string, string_length, options );
	}

	// FromStringBuffer::accept_literal
	template <size_t LENGTH>
		inline bool FromStringBuffer::accept_literal( const char (&string_literal)[ LENGTH ], AcceptOptions options )
	{
		return accept( string_literal, LENGTH - 1, options );
	}

	// FromStringBuffer::accept_literal_from_end
	template <size_t LENGTH>
		inline bool FromStringBuffer::accept_literal_from_end( const char (&string_literal)[ LENGTH ], AcceptOptions options )
	{
		return accept_from_end( string_literal, LENGTH - 1, options );
	}

	// FromStringBuffer::accept_indexed
	template <size_t TABLE_LENGTH>
		inline int FromStringBuffer::accept_indexed( const char * (&string_table)[ TABLE_LENGTH ], AcceptOptions options )
	{
		return accept_indexed( string_table, TABLE_LENGTH, options );
	}
	
} // namespace reflective

