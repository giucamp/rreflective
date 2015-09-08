
namespace reflective
{
	namespace details
	{
		template <unsigned RADIX, typename INT_TYPE>
			void sint_to_string(TextOutBuffer & i_dest, INT_TYPE i_value)
		{
			static_assert(std::numeric_limits<INT_TYPE>::is_signed, "sint_to_string is for signed ints");

			const bool is_negative = i_value < 0;

			const int buffer_size = std::numeric_limits<INT_TYPE>::digits10 + 1;
			char buffer[buffer_size];
			int used_buffer = 0;
			INT_TYPE remaining_value = is_negative ? -i_value : i_value;
			do {

				const char digit = static_cast<char>(remaining_value % RADIX);
				remaining_value /= RADIX;

				buffer[used_buffer] = '0' + digit;
				used_buffer++;

				REFLECTIVE_ASSERT(used_buffer < buffer_size || remaining_value == 0, "buffer too small?");
								
			} while (remaining_value > 0 && used_buffer < buffer_size);

			if (is_negative)
			{
				i_dest << '-';
			}

			for (int digit_index = used_buffer - 1; digit_index >= 0; digit_index--)
			{
				i_dest << buffer[digit_index];
			}
		}

		template <unsigned RADIX, typename INT_TYPE>
			void uint_to_string(TextOutBuffer & i_dest, INT_TYPE i_value)
		{
			static_assert(!std::numeric_limits<INT_TYPE>::is_signed, "sint_to_string is for unsigned ints");

			const int buffer_size = std::numeric_limits<INT_TYPE>::digits10 + 1;
			char buffer[buffer_size];
			int used_buffer = 0;
			INT_TYPE remaining_value = i_value;
			do {

				const char digit = static_cast<char>(remaining_value % RADIX);
				remaining_value /= RADIX;

				buffer[used_buffer] = '0' + digit;
				used_buffer++;

				REFLECTIVE_ASSERT(used_buffer < buffer_size || remaining_value == 0, "buffer too small?");
								
			} while (remaining_value > 0 && used_buffer < buffer_size);
			
			for (int digit_index = used_buffer - 1; digit_index >= 0; digit_index--)
			{
				i_dest << buffer[digit_index];
			}
		}
	}

				// signed integers to_string

	// to_string(TextOutBuffer, int8_t)
	void to_string(TextOutBuffer & i_dest, int8_t i_value)
	{
		details::sint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, int16_t)
	void to_string(TextOutBuffer & i_dest, int16_t i_value)
	{
		details::sint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, int32_t)
	void to_string(TextOutBuffer & i_dest, int32_t i_value)
	{
		details::sint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, int64_t)
	void to_string(TextOutBuffer & i_dest, int64_t i_value)
	{
		details::sint_to_string<10>(i_dest, i_value);
	}


			// unsigned integers to_string
	
	// to_string(TextOutBuffer, uint8_t)
	void to_string(TextOutBuffer & i_dest, uint8_t i_value)
	{
		details::uint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, uint16_t)
	void to_string(TextOutBuffer & i_dest, uint16_t i_value)
	{
		details::uint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, uint32_t)
	void to_string(TextOutBuffer & i_dest, uint32_t i_value)
	{
		details::uint_to_string<10>(i_dest, i_value);
	}

	// to_string(TextOutBuffer, uint64_t)
	void to_string(TextOutBuffer & i_dest, uint64_t i_value)
	{
		details::uint_to_string<10>(i_dest, i_value);
	}
}
