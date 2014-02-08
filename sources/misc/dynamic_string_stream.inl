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
	// DynamicStringStream::constructor()
	inline DynamicStringStream::DynamicStringStream()
	{
	}

	// DynamicStringStream::clear
	inline void DynamicStringStream::clear()
	{
		m_stream.clear();
	}

	// DynamicStringStream::constructor( i_initial_buffer_length )
	inline DynamicStringStream::DynamicStringStream( size_t i_initial_buffer_length )
	{
		reserve( i_initial_buffer_length );
	}
		
	// DynamicStringStream::string_length
	inline size_t DynamicStringStream::string_length() const
	{
		return m_stream.actual_length();
	}

	// DynamicStringStream::buffer_length
	inline size_t DynamicStringStream::buffer_length() const
	{
		return m_stream.buffer_length();
	}

	// DynamicStringStream::chars (const)
	inline const char * DynamicStringStream::chars() const
	{
		return m_stream.buffer();
	}
	
	// DynamicStringStream::destructor
	inline DynamicStringStream::~DynamicStringStream()
	{
		char * buffer = m_stream.buffer();
		if( buffer != nullptr )
			reflective_externals::mem_free( buffer );
	}

} // namespace reflective
