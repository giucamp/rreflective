
namespace reflective
{
	void to_string(TextOutBuffer & i_dest, int8_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length );
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, int16_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, int32_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, int64_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, uint8_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, uint16_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, uint32_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}

	void to_string(TextOutBuffer & i_dest, uint64_t i_value)
	{
		size_t written_length = 0;
		_snprintf_s(i_dest.next_char(), i_dest.remaining_buffer_length(), _TRUNCATE, "%d%n", i_value, &written_length);
		i_dest.manual_advance(written_length);
	}



	/*bool assign_from_string(TextInBuffer & i_source, int8_t & o_dest, TextOutBuffer & i_error)
	{
		sscanf()
		i_source.nect_char()
	}*/

	bool assign_from_string(TextInBuffer & i_source, int16_t & o_dest, TextOutBuffer & i_error);
	bool assign_from_string(TextInBuffer & i_source, int32_t & o_dest, TextOutBuffer & i_error);
	bool assign_from_string(TextInBuffer & i_source, int64_t & o_dest, TextOutBuffer & i_error);

	bool assign_from_string(TextInBuffer & i_source, uint8_t & o_dest, TextOutBuffer & i_error);
	bool assign_from_string(TextInBuffer & i_source, uint16_t & o_dest, TextOutBuffer & i_error);
	bool assign_from_string(TextInBuffer & i_source, uint32_t & o_dest, TextOutBuffer & i_error);
	bool assign_from_string(TextInBuffer & i_source, uint64_t & o_dest, TextOutBuffer & i_error);
}
