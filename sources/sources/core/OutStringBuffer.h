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
	/** This class implements an output text stream to an user-provided char buffer. OutStringBuffer does not participate to 
		the ownership of the buffer. The buffer must be valid when any non-const function is called on OutStringBuffer.
		The destination buffer can be specified to the constructor of OutStringBuffer, by specifying a char* pointing to
		the beginning of the buffer and a size_t with the total length, or by specifying a fixed-size char array:
		\code{.cpp}
		using namespace reflective;
		using namespace std;

		char dest[128];
		OutStringBuffer out(dest);
		out << "This is an int: " << 40 + 2;
		out << " and this is a string: " << string("str");

		std::cout << dest;
		\endcode		
		If a buffer is provided, OutStringBuffer store a null-terminating character at the end of the string. In no cases 
		OutStringBuffer will write outsize the destination buffer. If the buffer is not big enough to store all the text that
		is written, the text is truncated (the user may check this with the member function is_truncated). In this case the user may
		decide to allocate the required space, and rewrite the buffer from scratch.

		\code{.cpp}
		using namespace reflective;
		using namespace std;

		vector<char> buffer(10);

		OutStringBuffer out(buffer.data(), buffer.size());
		out << "This string is too long, and this is a number " << 40 + 2;

		if (out.is_truncated())
		{
		buffer.resize(out.needed_buffer_length());
		out = OutStringBuffer(buffer.data(), buffer.size());
		out << "This string is too long, and this is a number " << 40 + 2;

		REFLECTIVE_ASSERT(!out.is_truncated(), "");
		REFLECTIVE_ASSERT(out.is_full(), "");
		}

		std::cout << string(buffer.data(), buffer.size() - 1);
		\endcode

		The destination buffer can be set only while constructing an OutStringBuffer
		
		
	*/
	class OutStringBuffer
	{
	public:
		

				// construction \ destruction
		
		/** Constructs an OutStringBuffer with no destination buffer assigned. All the write methods
		 can be legitimately called, but OutStringBuffer will not write any buffer. Anyway the user can
		 use needed_buffer_length() to abtain the number of chars that would have be written so far. */
		OutStringBuffer();

		/** Constructs an OutStringBuffer specifying a destination buffer. */
		OutStringBuffer(char * i_dest_buffer, size_t i_buffer_size);
		
		template < size_t BUFFER_SIZE >
			OutStringBuffer( char (&i_dest_buffer)[BUFFER_SIZE] )
				: OutStringBuffer(i_dest_buffer, BUFFER_SIZE)
					{ }
			
		/** Writes a character to the buffer */
		void write_char(char i_char);
		
		/** Writes a string on the buffer. The string is not required to be null-terminated, but 
				i_string[i_string_length] can be a null char. */
		void write_nstr(const char * i_string, const size_t i_string_length);

		void write_cstr(const char * i_null_terminated_string)			{ write_nstr(i_null_terminated_string, strlen(i_null_terminated_string)); }
		
		template <size_t ARRAY_SIZE>
			void write_literal(const char(&i_array)[ARRAY_SIZE])		{ write_nstr(i_array, ARRAY_SIZE - 1); }

		template <typename TYPE>
			void write_any(const TYPE & i_object)
		{
			AnyToString<TYPE, has_to_string<TYPE>::value>::to_string(*this, i_object);
		}


		OutStringBuffer & operator << (char i_char) { write_char(i_char); return *this; }

		//OutStringBuffer & operator << (const char * i_null_terminated_string) { write_cstr(i_null_terminated_string); return *this; }

		template <size_t ARRAY_SIZE>
			OutStringBuffer & operator << (const char(&i_array)[ARRAY_SIZE]) { write_literal(i_array); return *this; }

		template <typename TYPE>
			OutStringBuffer & operator << (const TYPE & i_object)
		{
			write_any(i_object); return *this;
		}			

		template <typename TYPE>
			OutStringBuffer & operator << (TYPE && i_object)
		{
			write_any(std::forward<TYPE>(i_object)); return *this;
		}

		size_t needed_char_count() const							{ return m_written_chars; }

		size_t needed_buffer_length() const							{ return m_written_chars + 1; }

		size_t remaining_buffer_length() const						{ return m_end_of_buffer - m_next_char; }

		bool is_full() const										{ return m_end_of_buffer == m_next_char; }

		bool is_truncated() const									{ return m_written_chars + 1 > m_buffer_size; }		

		char * next_char() const									{ return m_next_char; }

		void manual_advance( size_t i_required_length, size_t i_actual_written_length );

	private:

		void flush();

		template <typename TYPE, bool HAS_TOSTRING_METHOD> struct AnyToString;
		template <typename TYPE> struct AnyToString<TYPE,true>
			{ static void to_string(OutStringBuffer & i_dest, const TYPE & i_object) { i_object.to_string(i_dest); } };

		template <typename TYPE> struct AnyToString<TYPE, false>
			{ static void to_string(OutStringBuffer & i_dest, const TYPE & i_object) { ::reflective::to_string(i_dest, i_object); } };

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

