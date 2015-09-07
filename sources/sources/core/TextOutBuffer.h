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

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	/** This class implements an output text stream. The buffer to be written is provided by the user of the class.  
		
	*/
	class TextOutBuffer
	{
	public:
		

				// construction \ destruction
		
		/** Constructs a TextOutBuffer without assigning a buffer, that may be used to compute the size */
		TextOutBuffer();

		TextOutBuffer(char * i_dests_buffer, size_t i_buffer_size);
		
		template < size_t BUFFER_SIZE >
			TextOutBuffer( char (&i_dests_buffer)[BUFFER_SIZE] )
				: TextOutBuffer(i_buffer, BUFFER_SIZE)
					{ }

		~TextOutBuffer()	{ flush(); }




		/** Writes a character to the buffer */
		void write_char(char i_char);
		
		/** Writes a string on the buffer. The string is not required to be null-terminated, but 
				i_string[i_string_length] can be a null char. */
		void write_nstr(const char * i_string, const size_t i_string_length);

		void write_cstr(const char * i_null_terminated_string)			{ write_nstr(i_null_terminated_string, strlen(i_null_terminated_string)); }
		
		template <size_t ARRAY_SIZE>
			void write_carray(const char(&i_array)[ARRAY_SIZE])			{ write_nstr(i_array, ARRAY_SIZE - 1); }

		template <typename TYPE>
			void write_any(const TYPE  & i_object)
		{
			AnyToString<TYPE, has_to_string<TYPE>::value>::to_string(*this, i_object);
		}



		TextOutBuffer & operator << (char i_char) { write_char(i_char); return *this; }

		//TextOutBuffer & operator << (const char * i_null_terminated_string) { write_cstr(i_null_terminated_string); return *this; }

		template <size_t ARRAY_SIZE>
			TextOutBuffer & operator << (const char(&i_array)[ARRAY_SIZE]) { write_carray(i_array); return *this; }

		template <typename TYPE>
			TextOutBuffer & operator << (const TYPE & i_object)
		{
			write_any(i_object); return *this;
		}


		void flush();

		size_t needed_buffer_length() const							{ return m_written_chars + 1; }

		size_t remaining_buffer_length() const						{ return m_end_of_buffer - m_next_char; }

		bool is_full() const										{ return m_end_of_buffer == m_next_char; }

		bool is_truncated() const									{ return m_written_chars + 1 > m_buffer_size; }		

		char * next_char() const							{ return m_next_char; }

		void manual_advance( size_t i_written_length );

	private:

		template <typename TYPE, bool HAS_TOSTRING_METHOD> struct AnyToString;
		template <typename TYPE> struct AnyToString<TYPE,true>
			{ static void to_string(TextOutBuffer & i_dest, const TYPE & i_object) { i_object.to_string(i_dest); } };

		template <typename TYPE> struct AnyToString<TYPE, false>
			{ static void to_string(TextOutBuffer & i_dest, const TYPE & i_object) { ::reflective::to_string(i_dest, i_object); } };

	private:
		char * m_next_char;
		char * m_end_of_buffer; /**< first char out of the buffer*/
		size_t m_written_chars; /**< chars written to the stream, interdependently from the actual buffer length */
		size_t m_buffer_size;
		#ifdef _DEBUG
			char * m_dbg_buffer; /**< pointer to the beginning of the buffer (which can be nullptr). The stream does not need
									 this, so it is provided only in debug.*/
		#endif
	};

} // namespace reflective

