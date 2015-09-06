
namespace reflective
{
	TextOutBuffer::TextOutBuffer()
	{
		m_next_char = nullptr;
		m_end_of_buffer = nullptr;
		m_written_chars = 0;
		m_buffer_size = 0;
		#ifdef _DEBUG
			m_dbg_buffer = nullptr;
		#endif
	}

	TextOutBuffer::TextOutBuffer(char * i_buffer, size_t i_buffer_size)
	{
		m_next_char = i_buffer;
		m_buffer_size = i_buffer_size;
		if (i_buffer_size > 0)
		{
			m_end_of_buffer = i_buffer + (i_buffer_size - 1);
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

	void TextOutBuffer::write(char i_char)
	{
		m_written_chars++;

		char * new_pos = m_next_char + 1;
		if (new_pos <= m_end_of_buffer)
		{
			*m_next_char = i_char;
			*new_pos = 0;
			m_next_char = new_pos;
		}
	}

	void TextOutBuffer::write(const char * i_string, const size_t i_string_length)
	{
		m_written_chars += i_string_length;

		const size_t remaining_length = m_end_of_buffer - m_next_char;
		const size_t length_to_write = std::min(remaining_length, i_string_length);
		
		memcpy(m_next_char, i_string, length_to_write * sizeof(char));
		char * new_pos = m_next_char + length_to_write;
		if( m_buffer_size > 0 )
		{
			// if the buffer is big at least on char, then the last character is reserved for the terminating null 
			*new_pos = 0;
		}		
		m_next_char = new_pos;
	}
}

