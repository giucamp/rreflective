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

	// StringOutputStream::default constructor
	inline StringOutputStream::StringOutputStream()
		: m_buffer( nullptr ), m_needed_length( 0 ), m_buffer_length( 0 )
	{
	}

	// StringOutputStream::constructor
	inline StringOutputStream::StringOutputStream( char * buffer, size_t max_length )
		: m_buffer( buffer ), m_needed_length( 0 ), m_buffer_length( max_length )
	{

	}

	// StringOutputStream::append
	inline size_t StringOutputStream::append( const char * null_terminated_token )
	{
		const size_t length = strlen( null_terminated_token );
		return append( null_terminated_token, length );
	}

	// StringOutputStream::append_literal
	template <size_t ARRAY_SIZE>
		inline size_t StringOutputStream::append_literal( const char (&literal_tring)[ ARRAY_SIZE ] )
	{
		return append( literal_tring, ARRAY_SIZE - 1 );
	}


	// StringOutputStream::start_of_chars
	inline char * StringOutputStream::buffer()
	{
		return m_buffer;
	}

	// StringOutputStream::start_of_chars
	inline const char * StringOutputStream::buffer() const
	{
		return m_buffer;
	}

	// StringOutputStream::actual_length
	inline size_t StringOutputStream::actual_length() const
	{
		const size_t buff_len = m_buffer_length > 0 ? m_buffer_length - 1 : 0;
		if( m_needed_length <= buff_len )
			return m_needed_length;
		return buff_len;
	}

	// StringOutputStream::is_truncated
	inline bool StringOutputStream::is_truncated() const
	{
		return m_needed_length + 1 > m_buffer_length;
	}

	// StringOutputStream::needed_length
	inline size_t StringOutputStream::needed_length() const
	{
		return m_needed_length + 1;
	}

	// StringOutputStream::buffer_length
	inline size_t StringOutputStream::buffer_length() const
	{
		return m_buffer_length;
	}

	// StringOutputStream::set_string_buffer
	inline void StringOutputStream::set_string_buffer( char * buffer, size_t max_length )
	{
		m_buffer = buffer;
		m_needed_length = 0;
		m_buffer_length = max_length;
	}

	// StringOutputStream::unset_string_buffer
	inline void StringOutputStream::unset_string_buffer()
	{
		m_buffer = nullptr;
		m_needed_length = 0;
		m_buffer_length = 0;
	}
		
	// StringOutputStream::clear
	inline void StringOutputStream::clear()
	{
		m_needed_length = 0;
	}

	// StringOutputStream::clear
	inline void StringOutputStream::clear( size_t i_length_to_preserve )
	{
		m_needed_length = i_length_to_preserve;
	}

	// StringOutputStream::operator << ( const TYPE & i_object )
	template <typename TYPE>  
		inline StringOutputStream & StringOutputStream::operator << ( const TYPE & i_object )
	{ 
		const Type & type = type_of( i_object );
		if( !type->check_capabilities( Type::eHasToString ) )
		{
			*this << type.name() << " does not have to_string";
		}
		else
		{
			type.to_string( *this, i_object );
		}

		return *this; 
	}

} // namespace reflective

