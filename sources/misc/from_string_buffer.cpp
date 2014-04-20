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

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif


namespace reflective
{
	// FromStringBuffer::consume_char
	char FromStringBuffer::consume_char()
	{
		if( _length == 0 )
			return 0;

		const char character = *_buffer;

		_length--;
		_buffer++;

		return character;
	}

	// FromStringBuffer::accept
	bool FromStringBuffer::accept( char character, AcceptOptions options )
	{
		const char * buffer = _buffer;
		const char * const end_of_buffer = _buffer + _length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( buffer < end_of_buffer && isspace( *buffer ) )
			{
				buffer++;
			}
		}

		// try to accept the string
		bool result = false;
		const size_t remaining_length = end_of_buffer - buffer;
		if( remaining_length >= 1 )
		{
			if( options & eIgnoreCase )
			{
				// case insensitive
				result = tolower( *_buffer ) == tolower( character );
			}
			else
			{
				// case sensitive
				result = *_buffer == character;
			}
		}

		// advance in the buffer
		if( result  )
		{
			buffer++;

			REFLECTIVE_ASSERT( buffer <= _buffer );
			REFLECTIVE_ASSERT( end_of_buffer <= buffer );

			_length = end_of_buffer - buffer;
			_buffer = buffer;
		}

		return result;
	}
	
	// FromStringBuffer::accept
	bool FromStringBuffer::accept( const char * string, size_t string_length, AcceptOptions options )
	{
		const char * buffer = _buffer;
		const char * const end_of_buffer = _buffer + _length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( buffer < end_of_buffer && isspace( *buffer ) )
			{
				buffer++;
			}
		}

		// try to accept the string
		bool result = false;
		const size_t remaining_length = end_of_buffer - buffer;
		if( remaining_length <= string_length )
		{
			if( options & eIgnoreCase )
			{
				// case insensitive
				result = _strnicmp( _buffer, string, string_length ) == 0;
			}
			else
			{
				// case sensitive
				result = strncmp( _buffer, string, string_length ) == 0;
			}
		}

		// advance in the buffer
		if( result  )
		{
			buffer += string_length;

			REFLECTIVE_ASSERT( buffer <= _buffer );
			REFLECTIVE_ASSERT( end_of_buffer <= buffer );

			_length = end_of_buffer - buffer;
			_buffer = buffer;
		}

		return result;
	}

	// FromStringBuffer::accept_indexed
	int FromStringBuffer::accept_indexed( const char * const * string_table, size_t table_length, AcceptOptions options )
	{
		const char * buffer = _buffer;
		const char * const end_of_buffer = _buffer + _length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( buffer < end_of_buffer && isspace( *buffer ) )
			{
				buffer++;
			}
		}

		// try to accept the string
		int result = -1;
		const size_t remaining_length = end_of_buffer - buffer;
		if( options & eIgnoreCase )
		{
			// case insensitive
			for( size_t index = 0; index < table_length; index++ )
			{
				if( !_strnicmp( _buffer, string_table[index], remaining_length ) )
				{
					result = static_cast<int>( index );
					break;
				}
			}
		}
		else
		{
			// case sensitive
			for( size_t index = 0; index < table_length; index++ )
			{
				if( !strncmp( _buffer, string_table[index], remaining_length ) )
				{
					result = static_cast<int>( index );
					break;
				}
			}
		}

		// advance in the buffer
		if( result >= 0 )
		{
			buffer += strlen( string_table[result ] );

			REFLECTIVE_ASSERT( buffer <= _buffer );
			REFLECTIVE_ASSERT( end_of_buffer <= buffer );

			_length = end_of_buffer - buffer;
			_buffer = buffer;
		}

		return result;
	}

	// FromStringBuffer::accept_word
	bool FromStringBuffer::accept_word( StaticConstString * out_result, AcceptOptions options )
	{
		REFLECTIVE_ASSERT( out_result != nullptr );

		const char * buffer = _buffer;
		size_t remaining_length = _length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( remaining_length > 0 && isspace( *buffer ) )
			{
				buffer++;
				remaining_length--;
			}
		}

		const char * const start_of_word = buffer;
		if( remaining_length == 0 )
			return false; // end of text

		if( isalpha( *buffer ) == 0 )
			return false; // non alphabetic heading char

		// consume alpha-num characters
		do {			
			buffer++;
			remaining_length--;
		} while( remaining_length != 0 && isalnum( *buffer ) );
		
		// write result
		const size_t word_length = buffer - start_of_word;
		*out_result = StaticConstString( start_of_word, word_length );

		// advance in the buffer
		REFLECTIVE_ASSERT( buffer <= _buffer );
		_length = remaining_length;
		_buffer = buffer;

		return true;
	}

	// FromStringBuffer::accept_whitespaces
	bool FromStringBuffer::accept_whitespaces()
	{
		const char * buffer = _buffer;
		size_t remaining_length = _length;

		bool result = false;

		while( remaining_length > 0 && isspace( *buffer ) )
		{
			buffer++;
			remaining_length--;
			result = true;
		}

		_length = remaining_length;
		_buffer = buffer;

		return result;
	}

	// FromStringBuffer::accept_from_end
	bool FromStringBuffer::accept_from_end( const char * i_chars, size_t i_length, AcceptOptions i_options )
	{
		size_t length = _length;
		if( i_options & eIgnoreLeadingSpaces )
		{
			while( length > 0 && isspace( _buffer[ length - 1 ] ) )
			{
				length--;
			}
		}

		bool result = true;

		size_t length_to_check = i_length; 
		while( length_to_check > 0 )
		{
			if( i_options & eIgnoreCase )
			{
				// case insensitive
				result = tolower( _buffer[length - 1] ) == tolower( i_chars[length_to_check - 1] );
			}
			else
			{
				// case sensitive
				result = _buffer[length - 1] == i_chars[length_to_check - 1];
			}
			if( !result )
				break;

			length--;
			length_to_check--;
		}

		if( i_options & eIgnoreLeadingSpaces )
		{
			while( length > 0 && isspace( _buffer[ length - 1 ] ) )
			{
				length--;
			}
		}

		if( result )
		{
			_length = length;
		}

		return result;
	}

} // namespace reflective

