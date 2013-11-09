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

	// ToStringBuffer::default constructor
	inline ToStringBuffer::ToStringBuffer()
		: _buffer( null ), _needed_length( 0 ), _max_length( 0 )
	{
	}

	// ToStringBuffer::constructor
	template <size_t MAX_LENGTH>
		inline ToStringBuffer::ToStringBuffer( char (&buffer)[ MAX_LENGTH ] )
			: _buffer( buffer ), _needed_length( 0 ), _max_length( MAX_LENGTH )
	{
	}

	// ToStringBuffer::constructor
	inline ToStringBuffer::ToStringBuffer( char * buffer, size_t max_length )
		: _buffer( buffer ), _needed_length( 0 ), _max_length( max_length )
	{

	}

	// ToStringBuffer::append
	inline size_t ToStringBuffer::append( const char * null_terminated_token )
	{
		const size_t length = strlen( null_terminated_token );
		return append( null_terminated_token, length );
	}

	// ToStringBuffer::append_literal
	template <size_t ARRAY_SIZE>
		inline size_t ToStringBuffer::append_literal( const char (&literal_tring)[ ARRAY_SIZE ] )
	{
		return append( literal_tring, ARRAY_SIZE - 1 );
	}


	// ToStringBuffer::start_of_chars
	inline char * ToStringBuffer::start_of_chars()
	{
		return _buffer;
	}

	// ToStringBuffer::start_of_chars
	inline const char * ToStringBuffer::start_of_chars() const
	{
		return _buffer;
	}

	// ToStringBuffer::actual_length
	inline size_t ToStringBuffer::actual_length() const
	{
		if( _needed_length <= _max_length )
			return _needed_length;
		return _max_length;
	}

	// ToStringBuffer::is_truncated
	inline bool ToStringBuffer::is_truncated() const
	{
		return _needed_length > _max_length;
	}

	// ToStringBuffer::needed_length
	inline size_t ToStringBuffer::needed_length() const
	{
		return _needed_length;
	}

	// ToStringBuffer::max_length
	inline size_t ToStringBuffer::max_length() const
	{
		return _max_length;
	}

	// ToStringBuffer::init
	inline void ToStringBuffer::init( char * buffer, size_t max_length )
	{
		_buffer = buffer;
		_needed_length = 0;
		_max_length = max_length;
	}

	// ToStringBuffer::uninit
	inline void ToStringBuffer::uninit()
	{
		_buffer = null;
		_needed_length = 0;
		_max_length = 0;
	}

	// ToStringBuffer::is_initialized
	inline bool ToStringBuffer::is_initialized() const
	{
		return _buffer != null;
	}

	// ToStringBuffer::get_bookmark
	inline size_t ToStringBuffer::get_bookmark() const
	{
		return _needed_length;
	}

	// ToStringBuffer::set_bookmark
	inline void ToStringBuffer::set_bookmark( size_t bookmark )
	{
		_needed_length = bookmark;
	}

	// ToStringBuffer::clear
	inline void ToStringBuffer::clear()
	{
		_needed_length = 0;
	}

} // namespace reflective

