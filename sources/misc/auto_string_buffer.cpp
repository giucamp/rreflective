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
	// AutoStringBuffer::object_to_string( object, type )
	bool AutoStringBuffer::object_to_string( const void * object, const Type & type )
	{
		if( !type.check_capabilities( Type::eHasToStringDumper ) )
			return false;

		// compute the length
		ToStringBuffer buffer( _chars, _buffer_length );
		type.to_string( buffer, object );
		_string_length = buffer.needed_length() + 1;
		if( _string_length > _buffer_length )
		{
			if( !realloc( _string_length ) )
				return false;

			buffer.init( _chars, _buffer_length );
			type.to_string( buffer, object );

			REFLECTIVE_ASSERT( !buffer.is_truncated() );			
		}

		_chars[ buffer.needed_length() ] = 0;
		return true;
	}

	// AutoStringBuffer::realloc
	bool AutoStringBuffer::realloc( size_t new_buffer_size )
	{
		if( _chars != null )
			reflective_externals::mem_free( _chars );
		_chars = static_cast<char*>( reflective_externals::mem_alloc( alignment_of( char ), new_buffer_size * sizeof( char ) ) );
		if( _chars == null )
		{
			_buffer_length = 0;
			return false;
		}

		_buffer_length = new_buffer_size;
		return true;
	}

} // namespace reflective


