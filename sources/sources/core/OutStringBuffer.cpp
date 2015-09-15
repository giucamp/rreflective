
namespace reflective
{
	OutStringBuffer::OutStringBuffer()
	{
		m_next_char = nullptr;
		m_end_of_buffer = nullptr;
		m_written_chars = 0;
		m_buffer_size = 0;
		#ifdef _DEBUG
			m_dbg_buffer = nullptr;
		#endif
	}

	OutStringBuffer::OutStringBuffer(char * i_buffer, size_t i_buffer_size)
	{
		m_next_char = i_buffer;
		m_buffer_size = i_buffer_size;
		if (i_buffer_size > 0)
		{
			m_end_of_buffer = i_buffer + (i_buffer_size - 1);
			*m_next_char = 0;
		}
		else
		{
			m_end_of_buffer = i_buffer;
		}
		 
		m_written_chars = 0;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	void OutStringBuffer::write_char(char i_char)
	{
		m_written_chars++;

		if (m_next_char < m_end_of_buffer)
		{
			*m_next_char = i_char;
			m_next_char++;

			flush();
		}
	}

	void OutStringBuffer::write_nstr(const char * i_string, const size_t i_string_length)
	{
		REFLECTIVE_ASSERT(memchr(i_string, 0, i_string_length) == nullptr, "The input string contains a null character");
		
		m_written_chars += i_string_length;

		const size_t remaining_length = m_end_of_buffer - m_next_char;
		const size_t length_to_write = std::min(remaining_length, i_string_length);
		
		memcpy(m_next_char, i_string, length_to_write * sizeof(char));
		m_next_char += length_to_write;

		flush();
	}

	void OutStringBuffer::manual_advance(size_t i_required_length, size_t i_actual_written_length)
	{
		REFLECTIVE_ASSERT(i_actual_written_length <= remaining_buffer_length(), "OVERFOWING THE BUFFER!!!" );
		m_written_chars += i_required_length;
		m_next_char += i_actual_written_length;

		flush();
	}

	void OutStringBuffer::flush()
	{
		if (m_buffer_size > 0)
		{
			// if the buffer is big at least on char, then the last character is reserved for the terminating null 
			*m_next_char = 0;
		}
	}
}

