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
	class StringOutputStream;
	class Class;

	// StaticConstString
	class StaticConstString
	{
	public:

		// construction
		StaticConstString();
		StaticConstString( const char * source );
		StaticConstString( const char * source, size_t length );		
		template < size_t LENGTH > StaticConstString( const char (&source)[ LENGTH ] );

		// comparaison
		bool operator == ( const StaticConstString & operand ) const;
		bool operator != ( const StaticConstString & operand ) const;

		// access
		const char * start_of_chars() const;
		const char * end_of_chars() const;
		size_t length() const;
		bool is_empty() const;

		// to_string
		static void to_string( StringOutputStream & dest_buffer, const Type & type, 
			const void * object );	

	private:
		const char * _string;
		size_t _length;
	};

}

namespace reflective_externals
{
	// init_type with StaticConstString
	reflective::Class * init_type(
		reflective::StaticConstString * null_pointer_1,
		reflective::StaticConstString * null_pointer_2 );
}



