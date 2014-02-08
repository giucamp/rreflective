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
	// DynamicStringStream
	class DynamicStringStream
	{
	public:

		DynamicStringStream();

		DynamicStringStream( size_t i_initial_buffer_length );

		~DynamicStringStream();

		bool append_object( const void * i_object, const Type & i_type );

		template <typename TYPE> bool append_object( const TYPE & i_object )
			{ return append_object( &i_object, type_of( i_object ) ); }

		void append_char( char i_char );

		void append_string( const char * i_string, size_t i_length );
		
		void clear();

		template <size_t LENGTH> DynamicStringStream & operator << ( const char (i_literal)[LENGTH] )
			{ append_string( i_literal, LENGTH - 1 ); return *this; }

		DynamicStringStream & operator << ( char i_char )
			{ append_char( i_char ); return *this; }

		DynamicStringStream & operator << ( const char * i_string )
			{ append_string( i_string, strlen(i_string) ); return *this; }
		
		template <typename TYPE> DynamicStringStream & operator << ( const TYPE & i_object )
			{ append_object( &i_object, type_of( i_object ) ); return *this;  }
		
		const char * chars() const;

		size_t string_length() const;
		
		void reserve( size_t i_buffer_length );
		
		size_t buffer_length() const;

	private:
		DynamicStringStream( const DynamicStringStream & ); // unimplemented
		DynamicStringStream & operator = ( const DynamicStringStream & );  // unimplemented

	private: // data members
		StringOutputStream m_stream;
	};

} //namespace reflective
