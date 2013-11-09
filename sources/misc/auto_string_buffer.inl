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
	// AutoStringBuffer::constructor()
	inline AutoStringBuffer::AutoStringBuffer()
		: _chars( null ), _buffer_length( 0 ), _string_length( 0 )
	{
	}

	// AutoStringBuffer::constructor( initial_buffer_size )
	inline AutoStringBuffer::AutoStringBuffer( 
			size_t initial_buffer_size )
		: _buffer_length( initial_buffer_size ), _string_length( 0 )
	{
		_chars = static_cast<char*>( reflective_externals::mem_alloc( alignment_of( char ), initial_buffer_size * sizeof( char ) ) );
		if( _chars == null )
			_buffer_length = 0;
	}

	// AutoStringBuffer::object_to_string( object )
	template <class OBJECT_TYPE>
		inline bool AutoStringBuffer::object_to_string( const OBJECT_TYPE & object )
	{
		// get the actual type
		const Type & type = safe_type_of( object );

		return object_to_string( &object, type );
	}
	
	// AutoStringBuffer::string_length
	inline size_t AutoStringBuffer::string_length() const
	{
		return _string_length;
	}

	// AutoStringBuffer::buffer_length
	inline size_t AutoStringBuffer::buffer_length() const
	{
		return _buffer_length;
	}

	// AutoStringBuffer::chars (const)
	inline const char * AutoStringBuffer::chars() const
	{
		return _chars;
	}

	// AutoStringBuffer::chars
	inline char * AutoStringBuffer::chars()
	{
		return _chars;
	}

	// AutoStringBuffer::destructor
	inline AutoStringBuffer::~AutoStringBuffer()
	{
		if( _chars != null )
			reflective_externals::mem_free( _chars );
	}

} // namespace reflective
