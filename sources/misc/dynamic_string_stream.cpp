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
	// DynamicStringStream::append_object( object, type )
	bool DynamicStringStream::append_object( const void * i_object, const Type & i_type )
	{
		if( !i_type.check_capabilities( Type::eHasToString ) )
		{
			*this << '{' << i_type.name() << " doesn't have to_string} ";
			return false;
		}
		else
		{
			const size_t prev_length = m_stream.needed_length();
			REFLECTIVE_ASSERT( prev_length > 0 );

			i_type.to_string( m_stream, i_object );
			if( !m_stream.is_truncated() )
				return true;

			const size_t needed_length = m_stream.needed_length();

			m_stream.clear( prev_length - 1 );

			reserve( needed_length * 2 + 32 );

			i_type.to_string( m_stream, i_object );

			REFLECTIVE_ASSERT( !m_stream.is_truncated() );

			return true;
		}		
	}

	// DynamicStringStream::append_string
	void DynamicStringStream::append_string( const char * i_string, size_t i_length )
	{
		const size_t prev_length = m_stream.needed_length();
		REFLECTIVE_ASSERT( prev_length > 0 );

		m_stream.append( i_string, i_length );
		if( !m_stream.is_truncated() )
			return;

		reserve( m_stream.needed_length() * 2 + 32 );

		m_stream.clear( prev_length - 1 );

		m_stream.append( i_string, i_length );

		REFLECTIVE_ASSERT( !m_stream.is_truncated() );		
	}

	// DynamicStringStream::append_char
	void DynamicStringStream::append_char( char i_char )
	{
		const size_t prev_length = m_stream.needed_length();
		REFLECTIVE_ASSERT( prev_length > 0 );

		m_stream.append( i_char );
		if( !m_stream.is_truncated() )
			return;

		reserve( m_stream.needed_length() * 2 + 32 );

		m_stream.clear( prev_length - 1 );

		m_stream.append( i_char );

		REFLECTIVE_ASSERT( !m_stream.is_truncated() );		
	}

	// DynamicStringStream::reserve
	void DynamicStringStream::reserve( size_t i_buffer_length )
	{
		REFLECTIVE_ASSERT( i_buffer_length > 0 ); // the buffer should at least contain the terminating null

		if( i_buffer_length <= m_stream.buffer_length() )
			return;

		const size_t prev_used_length = m_stream.actual_length();
		
		char * new_buffer = static_cast<char*>( reflective_externals::mem_alloc( alignment_of( char ), i_buffer_length * sizeof( char ) ) );
		char * prev_buffer = m_stream.buffer();
		if( prev_buffer != nullptr )
		{
			memcpy( new_buffer, prev_buffer, prev_used_length * sizeof( char ) );
			reflective_externals::mem_free( prev_buffer );
		}
		else
			*new_buffer = 0;

		m_stream.set_string_buffer( new_buffer, i_buffer_length, prev_used_length );
	}

} // namespace reflective


