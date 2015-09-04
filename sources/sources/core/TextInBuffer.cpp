
namespace reflective
{
	TextInBuffer::TextInBuffer(const char * i_buffer, size_t i_buffer_size)
	{
		m_next_char_to_read = i_buffer;
		m_end_of_buffer = i_buffer + i_buffer_size;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	bool TextInBuffer::accept(char i_character)
	{
		if (m_next_char_to_read < m_end_of_buffer)
		{
			return *m_next_char_to_read == i_character;
		}
		return false;
	}

	bool TextInBuffer::accept(const char * i_null_terminated_string)
	{		
		const char * curr_buffer_char = m_next_char_to_read;
		for (const char * curr_source_char = i_null_terminated_string; *curr_source_char != 0; curr_source_char++)
		{

		}

		return true;
	}

	bool TextInBuffer::accept(const char * i_string, size_t i_string_length)
	{

	}
}

