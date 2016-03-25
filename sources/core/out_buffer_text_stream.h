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
	/** This class implements an output text stream to write formatted text to an user-provided character buffer. It is a lightweight objects, and
		doesn't have virtual functions.

		OutBufferTextStream does not participate to the ownership of the buffer. The buffer must be valid when any non-const
		method is called on OutBufferTextStream. The destination buffer must be specified to the constructor by passing
		a char* pointing to the beginning of the buffer and a size_t with the total length, or by specifying a 
		fixed-size char array:
		\code{.cpp}
		using namespace reflective;
		using namespace std;

		char dest[128];
		OutBufferTextStream out(dest);
		out << "This is an int: " << 40 + 2;
		out << " and this is a string: " << string("str");

		std::cout << dest << endl;
		\endcode
		If a buffer is provided, OutBufferTextStream store a null-terminating character at the end of the string after construction 
		and after any write. In no cases OutBufferTextStream will write outsize the destination buffer. If the buffer is not big enough 
		to store all the text that is written, the content is truncated. The user may check this with the member function is_truncated(). 
		In this case the user may decide to allocate the required space, and rewrite the buffer from scratch:
		\code{.cpp}
		using namespace reflective;
		using namespace std;

		vector<char> buffer(10);

		OutBufferTextStream out(buffer.data(), buffer.size());
		out << "This string is too long, and this is a number " << 40 + 2;

		if (out.is_truncated())
		{
			buffer.resize(out.needed_buffer_length());
			out = OutBufferTextStream(buffer.data(), buffer.size());
			out << "This string is too long, and this is a number " << 40 + 2;
		}

		std::cout << string(buffer.data(), buffer.size() - 1) << endl;
		\endcode
		
	*/
	template <typename CHAR, typename CHAR_TRAITS = std::char_traits<CHAR> >
		class BasicOutBufferTextStream final
	{
	public:
		
		using char_type = CHAR;
		using traits_type = CHAR_TRAITS;
		using pos_type = size_t;
		using off_type = size_t;

				// construction \ destruction
		
		/** Constructs a BasicOutBufferTextStream given a destination buffer. The size of the buffer can be 
			zero if and only if the pointer is null.
			@param i_dest_buffer pointer to the beginning of the destination buffer
			@param i_buffer_size size of the destination buffer */
		REFLECTIVE_CONSTEXPR BasicOutBufferTextStream(CHAR * i_dest_buffer, size_t i_buffer_size) REFLECTIVE_NOEXCEPT
			: m_next_char(i_dest_buffer), m_end_of_buffer(i_dest_buffer + i_buffer_size - 1), m_written_chars(0), m_buffer_size(i_buffer_size)
		{
			REFLECTIVE_ASSERT(i_buffer_size > 0, "the buffer cannot be empty");
			*m_next_char = CHAR_TRAITS::to_char_type(0); // terminates the string
		}

		/** Constructs an OutBufferTextStream given a character array. */
		template < size_t BUFFER_SIZE >
			BasicOutBufferTextStream( CHAR (&i_dest_buffer)[BUFFER_SIZE] ) REFLECTIVE_NOEXCEPT
				: BasicOutBufferTextStream(i_dest_buffer, BUFFER_SIZE)
					{  }
			
		/** Writes a character to the buffer 
			@param i_char character to write. Can't be the null character. */
		bool write_char(CHAR i_char) REFLECTIVE_NOEXCEPT
		{
			m_written_chars++;

			if (m_next_char < m_end_of_buffer)
			{
				CHAR_TRAITS::assign(*m_next_char++, i_char);
				*m_next_char = CHAR_TRAITS::to_char_type(0); // terminates the string
				return true;
			}
			else
			{
				return false;
			}
		}

		size_t write_string(const BasicStringView<CHAR, CHAR_TRAITS> & i_string)
		{
			m_written_chars += i_string.size();

			const size_t remaining_length = m_end_of_buffer - m_next_char;
			const auto length_to_write = std::min(remaining_length, i_string.size());

			CHAR_TRAITS::copy(m_next_char, i_string.data(), length_to_write);
			m_next_char += length_to_write;

			*m_next_char = CHAR_TRAITS::to_char_type(0); // terminates the string

			return length_to_write;
		}
		
		/** Writes an array of characters, presumably a string literal (like "a string").
			@param i_array array of const chars. All the characters of this array, except the last,
				must not be the null char. The last character must be the null-char. */
		template <size_t ARRAY_SIZE>
			size_t write_literal(const CHAR(&i_array)[ARRAY_SIZE])
		{
			REFLECTIVE_ASSERT(i_array[ARRAY_SIZE - 1] == 0, "the array must contain a null terminated string");
			return write_string(BasicStringView<CHAR, CHAR_TRAITS>(i_array, ARRAY_SIZE - 1));
		}


		/** Retrieves the number of characters that would have been written so far, independently of the size of the 
			buffer (and therefore of the truncation). */
		size_t needed_char_count() const							{ return m_written_chars; }

		/** Retrieves how big a buffer should be to contain all the chars written so far without incurring in truncation. */
		size_t needed_buffer_length() const							{ return (m_written_chars + 1) * sizeof(CHAR); }

		size_t remaining_buffer_length() const						{ return m_end_of_buffer - m_next_char; }

		bool is_full() const										{ return m_end_of_buffer == m_next_char; }

		bool is_truncated() const									{ return m_written_chars + 1 > m_buffer_size; }		

		CHAR * next_char() const									{ return m_next_char; }
		
		BasicOutBufferTextStream & operator << (const BasicStringView<CHAR, CHAR_TRAITS> & i_string)
		{
			write_string(i_string);
			return *this;
		}

		BasicOutBufferTextStream & operator << (CHAR i_char)
		{
			write_char(i_char);
			return *this;
		}

	private:
		CHAR * m_next_char;
		CHAR * m_end_of_buffer; /**< first char out of the buffer*/
		pos_type m_written_chars; /**< chars written to the stream, interdependently from the actual buffer length */
		pos_type m_buffer_size;
	};

	template class BasicOutBufferTextStream< char >;
	template class BasicOutBufferTextStream< wchar_t >;
	template class BasicOutBufferTextStream< char16_t >;
	template class BasicOutBufferTextStream< char32_t >;

	using OutBufferTextStream = BasicOutBufferTextStream< char >;
	using WOutBufferTextStream = BasicOutBufferTextStream< wchar_t >;
	using u16OutBufferTextStream = BasicOutBufferTextStream< char16_t >;
	using u32OutBufferTextStream = BasicOutBufferTextStream< char32_t >;

	template <typename CHAR, typename CHAR_TRAITS >
		inline BasicOutBufferTextStream<CHAR, CHAR_TRAITS> & operator << (BasicOutBufferTextStream<CHAR, CHAR_TRAITS> & i_dest, const BasicStringView<CHAR, CHAR_TRAITS> & i_string)
	{
		i_dest.write_nstr(i_string.data(), i_string.length());
		return i_dest;
	}

	template <typename TYPE>
		inline std::string to_std_string(TYPE && i_object)
	{
		std::ostringstream std_ostream;
		std_ostream << i_object;
		return std_ostream.str();
		/*const size_t fixed_size_buffer_size = 512;
		char fixed_size_buffer[fixed_size_buffer_size];
		
		char * buff = fixed_size_buffer;
		size_t buffer_size = fixed_size_buffer_size;
		
		std::unique_ptr<char[]> dynamic_buffer;
		for (;;)
		{
			OutBufferTextStream out(buff, buffer_size);
			out << i_object;
			if (!out.is_truncated())
			{
				return std::string(buff, out.needed_char_count());
			}

			REFLECTIVE_ASSERT(dynamic_buffer.get() == nullptr, "an internal error has occurred in to_std_string, which indicates a bug in the code");

			buffer_size = out.needed_buffer_length();
			dynamic_buffer = std::make_unique<char[]>(buffer_size);
			buff = dynamic_buffer.get();
		}*/
	}

} // namespace reflective

