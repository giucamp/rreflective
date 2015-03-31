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
		if( m_remaining_length == 0 )
			return 0;

		const char character = *m_curr_char;

		m_remaining_length--;
		m_curr_char++;

		return character;
	}

	// FromStringBuffer::accept
	bool FromStringBuffer::accept( char character, AcceptOptions options )
	{
		const char * curr_char = m_curr_char;
		const char * const end_of_chars = m_curr_char + m_remaining_length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( curr_char < end_of_chars && isspace( *curr_char ) )
			{
				curr_char++;
			}
		}

		// try to accept the string
		bool result = false;
		const size_t remaining_length = end_of_chars - curr_char;
		if( remaining_length >= 1 )
		{
			if( options & eIgnoreCase )
			{
				// case insensitive
				result = tolower( *m_curr_char ) == tolower( character );
			}
			else
			{
				// case sensitive
				result = *m_curr_char == character;
			}
		}

		// advance in the curr_char
		if( result )
		{
			curr_char++;

			REFLECTIVE_ASSERT( curr_char <= end_of_chars );

			m_remaining_length = end_of_chars - curr_char;
			m_curr_char = curr_char;
		}

		return result;
	}
	
	// FromStringBuffer::accept
	bool FromStringBuffer::accept( const char * string, size_t string_length, AcceptOptions options )
	{
		const char * curr_char = m_curr_char;
		const char * const end_of_chars = m_curr_char + m_remaining_length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( curr_char < end_of_chars && isspace( *curr_char ) )
			{
				curr_char++;
			}
		}

		// try to accept the string
		bool result = false;
		const size_t remaining_length = end_of_chars - curr_char;
		if( remaining_length <= string_length )
		{
			if( options & eIgnoreCase )
			{
				// case insensitive
				result = _strnicmp( m_curr_char, string, string_length ) == 0;
			}
			else
			{
				// case sensitive
				result = strncmp( m_curr_char, string, string_length ) == 0;
			}
		}

		// advance in the curr_char
		if( result )
		{
			curr_char += string_length;

			REFLECTIVE_ASSERT( curr_char <= end_of_chars );

			m_remaining_length = end_of_chars - curr_char;
			m_curr_char = curr_char;
		}

		return result;
	}

	// FromStringBuffer::accept_indexed
	int FromStringBuffer::accept_indexed( const char * const * string_table, size_t table_length, AcceptOptions options )
	{
		const char * curr_char = m_curr_char;
		const char * const end_of_chars = m_curr_char + m_remaining_length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( curr_char < end_of_chars && isspace( *curr_char ) )
			{
				curr_char++;
			}
		}

		// try to accept the string
		int result = -1;
		const size_t remaining_length = end_of_chars - curr_char;
		if( options & eIgnoreCase )
		{
			// case insensitive
			for( size_t index = 0; index < table_length; index++ )
			{
				if( !_strnicmp( m_curr_char, string_table[index], remaining_length ) )
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
				if( !strncmp( m_curr_char, string_table[index], remaining_length ) )
				{
					result = static_cast<int>( index );
					break;
				}
			}
		}

		// advance in the curr_char
		if( result >= 0 )
		{
			curr_char += strlen( string_table[result ] );

			REFLECTIVE_ASSERT( curr_char <= end_of_chars );

			m_remaining_length = end_of_chars - curr_char;
			m_curr_char = curr_char;
		}

		return result;
	}

	// FromStringBuffer::accept_word
	bool FromStringBuffer::accept_word( StaticConstString * out_result, AcceptOptions options )
	{
		REFLECTIVE_ASSERT( out_result != nullptr );

		const char * curr_char = m_curr_char;
		size_t remaining_length = m_remaining_length;

		// skip spaces
		if( options & eIgnoreLeadingSpaces )
		{
			while( remaining_length > 0 && isspace( *curr_char ) )
			{
				curr_char++;
				remaining_length--;
			}
		}

		const char * const start_of_word = curr_char;
		if( remaining_length == 0 )
			return false; // end of text

		if( isalpha( *curr_char ) == 0 )
			return false; // non alphabetic heading char

		// consume alpha-num characters
		do {			
			curr_char++;
			remaining_length--;
		} while( remaining_length != 0 && isalnum( *curr_char ) );
		
		// write result
		const size_t word_length = curr_char - start_of_word;
		*out_result = StaticConstString( start_of_word, word_length );

		// advance in the curr_char
		m_remaining_length = remaining_length;
		m_curr_char = curr_char;

		return true;
	}

	// FromStringBuffer::accept_whitespaces
	bool FromStringBuffer::accept_whitespaces()
	{
		const char * curr_char = m_curr_char;
		size_t remaining_length = m_remaining_length;

		bool result = false;

		while( remaining_length > 0 && isspace( *curr_char ) )
		{
			curr_char++;
			remaining_length--;
			result = true;
		}

		m_remaining_length = remaining_length;
		m_curr_char = curr_char;

		return result;
	}

	// FromStringBuffer::accept_from_end
	bool FromStringBuffer::accept_from_end( const char * i_chars, size_t i_length, AcceptOptions i_options )
	{
		size_t length = m_remaining_length;
		if( i_options & eIgnoreLeadingSpaces )
		{
			while( length > 0 && isspace( m_curr_char[ length - 1 ] ) )
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
				result = tolower( m_curr_char[length - 1] ) == tolower( i_chars[length_to_check - 1] );
			}
			else
			{
				// case sensitive
				result = m_curr_char[length - 1] == i_chars[length_to_check - 1];
			}
			if( !result )
				break;

			length--;
			length_to_check--;
		}

		if( i_options & eIgnoreLeadingSpaces )
		{
			while( length > 0 && isspace( m_curr_char[ length - 1 ] ) )
			{
				length--;
			}
		}

		if( result )
		{
			m_remaining_length = length;
		}

		return result;
	}

} // namespace reflective

