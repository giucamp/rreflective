
namespace reflective
{
	OutStringBuffer::OutStringBuffer(char * i_buffer, size_t i_buffer_size)
	{
		REFLECTIVE_ASSERT( i_buffer_size > 0, "The size of the buffer must be > 0");

		m_next_char = i_buffer;
		m_buffer_size = i_buffer_size;
		m_end_of_buffer = i_buffer + (i_buffer_size - 1);
		*m_next_char = 0;
		 
		m_written_chars = 0;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	void OutStringBuffer::write_char(char i_char)
	{
		REFLECTIVE_ASSERT(i_char != 0, "Can't write the null character");
		m_written_chars++;

		if (m_next_char < m_end_of_buffer)
		{
			*m_next_char = i_char;
			m_next_char++;
			*m_next_char = 0;
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

		*m_next_char = 0;
	}

	void OutStringBuffer::manual_advance(size_t i_required_length, size_t i_actual_written_length)
	{
		REFLECTIVE_ASSERT(i_actual_written_length <= remaining_buffer_length(), "OVERFOWING THE BUFFER!!!" );
		m_written_chars += i_required_length;
		m_next_char += i_actual_written_length;
		*m_next_char = 0;
	}
}

