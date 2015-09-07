
namespace reflective
{
				// signed integers to_string

	// to_string(TextOutBuffer, int8_t)
	void to_string(TextOutBuffer & i_dest, int8_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRId8"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, int16_t)
	void to_string(TextOutBuffer & i_dest, int16_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRId16"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, int32_t)
	void to_string(TextOutBuffer & i_dest, int32_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRId32"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, int64_t)
	void to_string(TextOutBuffer & i_dest, int64_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRId64"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}


			// unsigned integers to_string
	
	// to_string(TextOutBuffer, uint8_t)
	void to_string(TextOutBuffer & i_dest, uint8_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRIu8"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, uint16_t)
	void to_string(TextOutBuffer & i_dest, uint16_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRIu16"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, uint32_t)
	void to_string(TextOutBuffer & i_dest, uint32_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRIu32"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	// to_string(TextOutBuffer, uint64_t)
	void to_string(TextOutBuffer & i_dest, uint64_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%"PRIu64"%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}
}
