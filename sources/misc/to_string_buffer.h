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
	// ToStringBuffer
	class ToStringBuffer
	{
	public:

		// constructor
		ToStringBuffer();
		template <size_t MAX_LENGTH> ToStringBuffer( char (&buffer)[MAX_LENGTH ] );
		ToStringBuffer( char * buffer, size_t max_length );

		void init( char * buffer, size_t max_length );
		void uninit();
		bool is_initialized() const;

		size_t get_bookmark() const;
		void set_bookmark( size_t bookmark );

		void clear();

		// append
		size_t append( const char * null_terminated_token );
		size_t append( const char * start_of_token, size_t token_length ); /* if
			the buffer is not big enough, the string is truncated. */
		template <size_t ARRAY_SIZE>
			size_t append_literal( const char (&chars)[ ARRAY_SIZE ] );
		bool append( char char_token );

		// start_of_chars \ actual_length		
		const char * start_of_chars() const; /* start of the string in the buffer*/
		char * start_of_chars(); /* start of the string in the buffer*/
		size_t actual_length() const; /* actual length of the string in the buffer,
			which can be truncated if the buffer is not big enough. */

		// is_truncated
		bool is_truncated() const;
		
		// needed_length \ max_length
		size_t needed_length() const; /* needed length for the buffer, that is the sum 
			of the length of all the tokens added with the method append */
		size_t max_length() const; // lenght of the buffer 

	private: // data members
		char * _buffer; // the actual length of the chars is min( _needed_length, _max_length )
		size_t _needed_length;
		size_t _max_length;
	};

} // namespace reflective

