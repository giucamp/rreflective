
namespace reflective
{
	TextInBuffer::TextInBuffer(const char * i_buffer, size_t i_buffer_size)
	{
		m_next_char = i_buffer;
		m_end_of_buffer = i_buffer + i_buffer_size;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	bool TextInBuffer::accept(char i_character)
	{
		if (m_next_char < m_end_of_buffer)
		{
			if (*m_next_char == i_character)
			{
				m_next_char++;
				return true;
			}			
		}
		return false;
	}

	bool TextInBuffer::accept(const char * i_null_terminated_string)
	{		
		const char * buffer = m_next_char;
		for (const char * source = i_null_terminated_string; *source != 0; source++, buffer++)
		{
			if (*buffer != *source || buffer >= m_end_of_buffer )
			{
				return false;
			}
		}
		m_next_char = buffer;
		return true;
	}

	bool TextInBuffer::accept(const char * i_string, size_t i_string_length)
	{
		const char * buffer = m_next_char;
		const char * end_of_source = i_string + i_string_length;
		for (const char * source = i_string; source < end_of_source; source++, buffer++)
		{
			if (*buffer != *source || buffer >= m_end_of_buffer)
			{
				return false;
			}
		}
		m_next_char = buffer;
		return true;
	}

	void TextInBuffer::manual_advance(size_t i_read_length)
	{
		REFLECTIVE_ASSERT(i_read_length <= remaining_buffer_length(), "OVERFOWING THE BUFFER!!!");
		m_next_char += i_read_length;
	}
}

