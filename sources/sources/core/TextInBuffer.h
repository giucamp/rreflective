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
	class TextInBuffer
	{
	public:

		TextInBuffer(const char * i_buffer, size_t i_length);

		template <size_t ARRAY_SIZE>
			TextInBuffer(char (&i_string)[ARRAY_SIZE])
				: TextInBuffer(i_string, ARRAY_SIZE)
					{ }

		TextInBuffer(const char * i_null_terminated_string)
			: TextInBuffer(i_null_terminated_string, strlen(i_null_terminated_string))
				{ }


			// accept

		bool accept(char i_character);

		bool accept(const char * i_null_terminated_string);

		bool accept(const char * i_string, size_t i_string_length);

		template <size_t ARRAY_SIZE>
			bool accept(char(&i_string)[ARRAY_SIZE])
		{
			return accept(i_string, i_string_length - 1);
		}

			//

		const char * nect_char() const				{ return m_next_char; }

		size_t remaining_buffer_length() const		{ return m_end_of_buffer - m_next_char; }

		void manual_advance(size_t i_read_length);

	private:
		const char * m_next_char;
		const char * m_end_of_buffer;
		#ifdef _DEBUG
			const char * m_dbg_buffer; /**< pointer to the beginning of the buffer (which can be nullptr). The stream does not need
									this, so it is provided only in debug.*/
		#endif
	};
}
