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
	// construction
	inline StaticConstString::StaticConstString()
		: _string( "" ), _length( 0 )
	{
	}

	// StaticConstString::constructor( const char * )
	inline StaticConstString::StaticConstString( const char * source )
		: _string( source ), _length( strlen( source ) )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < _length; index++ )
			{
				REFLECTIVE_ASSERT( _string[ index ] != 0 );
			}
		#endif
	}

	// StaticConstString::constructor( const char *, size_t )
	inline StaticConstString::StaticConstString( const char * source, size_t length )
		: _string( source ), _length( length )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			for( size_t index = 0; index < _length; index++ )
			{
				REFLECTIVE_ASSERT( _string[ index ] != 0 );
			}
		#endif
	}

	// StaticConstString::constructor( const char [] )
	template < size_t LENGTH > 
		inline StaticConstString::StaticConstString( const char (&source)[ LENGTH ] )
			: _string( source ), _length( LENGTH - 1 )
	{
		REFLECTIVE_COMPILETIME_ASSERT( LENGTH > 0 );
	}


	// StaticConstString::operator ==
	inline bool StaticConstString::operator == ( const StaticConstString & operand ) const
	{
		return operand._length == _length &&
			strncmp( _string, operand._string, _length ) == 0;
	}

	// StaticConstString::operator !=
	inline bool StaticConstString::operator != ( const StaticConstString & operand ) const
	{
		return operand._length != _length ||
			strncmp( _string, operand._string, _length ) != 0;
	}

	// StaticConstString::start_of_chars
	inline const char * StaticConstString::start_of_chars() const
	{
		return _string;
	}

	// StaticConstString::end_of_chars
	inline const char * StaticConstString::end_of_chars() const
	{
		return _string + _length;
	}

	// StaticConstString::length
	inline size_t StaticConstString::length() const
	{
		return _length;
	}

	// StaticConstString::is_empty
	inline bool StaticConstString::is_empty() const
	{
		return _length == 0;
	}
}
