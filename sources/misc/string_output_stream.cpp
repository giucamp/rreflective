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
	// StringOutputStream::append
	size_t StringOutputStream::append( const char * i_start_of_token, size_t i_token_length )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < i_token_length; index++ )
			{
				REFLECTIVE_ASSERT( i_start_of_token[index] != 0 );
			}
		#endif

		if( m_needed_length + 1 >= m_buffer_length )
		{
			m_needed_length += i_token_length;
			return 0;
		}

		if( m_buffer_length == 0 )
		{
			return 0;
		}

		// length_to_copy
		const size_t max_length_to_copy = m_buffer_length - m_needed_length;
		size_t length_to_copy = i_token_length;
		if( length_to_copy >= max_length_to_copy )
			length_to_copy = max_length_to_copy;

		// copy
		char * dest_start = m_buffer + m_needed_length;
		memcpy( dest_start, i_start_of_token, length_to_copy * sizeof( char ) );

		m_needed_length += i_token_length;
		if( m_needed_length < m_buffer_length )
			m_buffer[m_needed_length] = 0;
		else if( m_buffer_length > 0 )
			m_buffer[m_buffer_length - 1] = 0;

		#if REFLECTIVE_ENABLE_ASSERT
			size_t buff_actual_length = actual_length();
			for( size_t index = 0; index < buff_actual_length; index++ )
			{
				REFLECTIVE_ASSERT( m_buffer[index] != 0 );
			}
		#endif

		return length_to_copy;
	}


	// StringOutputStream::append
	bool StringOutputStream::append( char char_token )
	{
		REFLECTIVE_ASSERT( char_token != 0 );

		if( m_needed_length + 1 >= m_buffer_length )
		{
			m_needed_length++;
			return false;
		}

		m_buffer[ m_needed_length ] = char_token;
		m_needed_length++;
		if( m_needed_length < m_buffer_length )
			m_buffer[m_needed_length] = 0;
		else if( m_buffer_length > 0 )
			m_buffer[m_buffer_length - 1] = 0;

		#if REFLECTIVE_ENABLE_ASSERT
			size_t buff_actual_length = actual_length();
			for( size_t index = 0; index < buff_actual_length; index++ )
			{
				REFLECTIVE_ASSERT( m_buffer[index] != 0 );
			}
		#endif
	
		return true;
	}

} // namespace reflective

