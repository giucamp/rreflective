
namespace reflective
{
	InStringBuffer::InStringBuffer(const char * i_buffer, size_t i_buffer_size)
	{
		m_next_char = i_buffer;
		m_end_of_buffer = i_buffer + i_buffer_size;

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	bool InStringBuffer::accept(char i_character)
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

	bool InStringBuffer::accept(const char * i_null_terminated_string)
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

	bool InStringBuffer::accept(const char * i_string, size_t i_string_length)
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

	bool InStringBuffer::accept_case_ins(char i_character)
	{
		if (m_next_char < m_end_of_buffer)
		{
			if ( tolower(*m_next_char) == tolower(i_character) )
			{
				m_next_char++;
				return true;
			}
		}
		return false;
	}

	bool InStringBuffer::accept_case_ins(const char * i_null_terminated_string)
	{

	}

	bool InStringBuffer::accept_case_ins(const char * i_string, size_t i_string_length)
	{

	}

	bool InStringBuffer::accept_range(char i_first, char i_last)
	{
		if (m_next_char < m_end_of_buffer)
		{
			if (*m_next_char >= i_first && *m_next_char <= i_last)
			{
				m_next_char++;
				return true;
			}
		}
		return false;
	}


	bool InStringBuffer::accept_whitespaces()
	{
		bool result = false;
		while (m_next_char < m_end_of_buffer && isspace(*m_next_char))
		{
			m_next_char++;
			result = true;
		}
		return result;
	}

	void InStringBuffer::manual_advance(size_t i_read_length)
	{
		REFLECTIVE_ASSERT(i_read_length <= remaining_buffer_length(), "OVERFOWING THE BUFFER!!!");
		m_next_char += i_read_length;
	}
}

