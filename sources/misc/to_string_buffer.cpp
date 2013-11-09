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

namespace reflective
{
	// ToStringBuffer::append
	size_t ToStringBuffer::append( const char * start_of_token, size_t token_length )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < token_length; index++ )
			{
				REFLECTIVE_ASSERT( start_of_token[index] != 0 );
			}
		#endif

		if( _needed_length + 1 >= _max_length )
		{
			_needed_length += token_length;
			return 0;
		}

		if( _max_length == 0 )
		{
			_buffer[0] = 0;
			return 0;
		}

		// length_to_copy
		const size_t max_length_to_copy = _max_length - _needed_length;
		size_t length_to_copy = token_length;
		if( length_to_copy >= max_length_to_copy )
			length_to_copy = max_length_to_copy;

		// copy
		char * dest_start = _buffer + _needed_length;
		memcpy( dest_start, start_of_token, length_to_copy * sizeof( char ) );

		_needed_length += token_length;
		if( _needed_length < _max_length )
			_buffer[_needed_length] = 0;

		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < _needed_length; index++ )
			{
				REFLECTIVE_ASSERT( _buffer[index] != 0 );
			}
		#endif

		return length_to_copy;
	}


	// ToStringBuffer::append
	bool ToStringBuffer::append( char char_token )
	{
		REFLECTIVE_ASSERT( char_token != 0 );

		if( _needed_length >= _max_length )
		{
			_needed_length++;
			return false;
		}

		_buffer[ _needed_length ] = char_token;
		_needed_length++;
		if( _needed_length < _max_length )
			_buffer[_needed_length] = 0;

		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < _needed_length; index++ )
			{
				REFLECTIVE_ASSERT( _buffer[index] != 0 );
			}
		#endif
	
		return true;
	}

} // namespace reflective

