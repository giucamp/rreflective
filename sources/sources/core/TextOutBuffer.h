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
	/** This class implements an output text stream. The buffer to be written is provided by the user of the class.  */
	class TextOutBuffer
	{
	public:

		TextOutBuffer();

		TextOutBuffer(char * i_buffer, size_t i_buffer_size);
		
		template <size_t BUFFER_SIZE>
			TextOutBuffer(char(&i_buffer)[BUFFER_SIZE])
				: TextOutBuffer(i_buffer, BUFFER_SIZE)
			{ }

		void write(char i_char);
		
		void write(const char * i_string, const size_t i_string_length);

		void write(const char * i_null_terminated_string)			{ write(i_null_terminated_string, strlen(i_null_terminated_string)); }
		
		size_t  needed_length() const								{ return m_needed_length; }

		TextOutBuffer & operator << (const char * i_text)			{ write(i_text, strlen(i_text));  return *this; }

		template <size_t ARRAY_SIZE>
			TextOutBuffer & operator << (const char(&literal_tring)[ARRAY_SIZE])			{ write(i_text, ARRAY_SIZE - 1);  return *this; }

	private:
		char * m_next_char_to_write;
		char * m_end_of_buffer; /**< first char out of the buffer*/
		size_t m_needed_length; /**< chars written to the stream, interdependently from the actual buffer length */
		#ifdef _DEBUG
			char * m_dbg_buffer; /**< pointer to the beginning of the buffer (which can be nullptr). The stream does not need
									 this, so it is provided only in debug.*/
		#endif
	};

} // namespace reflective

