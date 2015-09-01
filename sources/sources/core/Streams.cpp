
#include "Streams.h"

namespace reflective
{
	TextOutStream::TextOutStream()
	{
		m_next_char_to_write = nullptr;
		m_end_of_buffer = nullptr;
		m_needed_length = 0;
		#ifdef _DEBUG
			m_dbg_buffer = nullptr;
		#endif
	}

	TextOutStream::TextOutStream(char * i_buffer, size_t i_buffer_size)
	{
		m_next_char_to_write = i_buffer;
		if (i_buffer_size > 0)
			m_end_of_buffer = i_buffer + (i_buffer_size - 1);
		else
			m_end_of_buffer = i_buffer;

		m_needed_length = 0;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	void TextOutStream::write(const char * i_string, const size_t i_string_length)
	{
		m_needed_length += i_string_length;

		char * new_pos = m_next_char_to_write + i_string_length;
		if (new_pos < m_end_of_buffer)
		{
			memcpy(m_next_char_to_write, i_string, i_string_length * sizeof(char));
			*new_pos = 0;
			m_next_char_to_write = new_pos;			
		}
	}
}

