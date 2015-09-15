
namespace reflective
{
	InStringBuffer::InStringBuffer(const char * i_buffer, size_t i_buffer_size)
	{
		m_next_char = i_buffer;
		m_end_of_buffer = i_buffer + i_buffer_size;
		REFLECTIVE_ASSERT(memchr(i_buffer, 0, i_buffer_size) == nullptr, "The string buffer contains a null character");

		#ifdef _DEBUG
			m_dbg_buffer = i_buffer;
		#endif
	}

	bool InStringBuffer::accept_char(char i_character)
	{
		REFLECTIVE_ASSERT(i_character != 0, "Can't accept the null character")
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

	bool InStringBuffer::accept_char_case_ins(char i_character)
	{
		REFLECTIVE_ASSERT(i_character != 0, "Can't accept the null character")
		if (m_next_char < m_end_of_buffer)
		{
			if (tolower(*m_next_char) == tolower(i_character))
			{
				m_next_char++;
				return true;
			}
		}
		return false;
	}

	bool InStringBuffer::accept_cstr(const char * i_string, size_t i_string_length)
	{
		REFLECTIVE_ASSERT(memchr(i_string, 0, i_string_length) == nullptr, "The input string contains a null character");

		const size_t remaining_length = m_end_of_buffer - m_next_char;
		if (remaining_length >= i_string_length && memcmp(m_next_char, i_string, i_string_length) == 0)
		{
			m_next_char += i_string_length;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool InStringBuffer::accept_cstr_case_ins(const char * i_string, size_t i_string_length)
	{
		REFLECTIVE_ASSERT(memchr(i_string, 0, i_string_length) == nullptr, "The input string contains a null character");

		const size_t remaining_length = m_end_of_buffer - m_next_char;
		if (remaining_length >= i_string_length && _memicmp(m_next_char, i_string, i_string_length) == 0)
		{
			m_next_char += i_string_length;
			return true;
		}
		else
		{
			return false;
		}
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

