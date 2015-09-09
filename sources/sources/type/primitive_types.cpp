
namespace reflective
{
	namespace details
	{
		// sint_to_string - converts a signed int to a string
		template <unsigned RADIX, typename INT_TYPE>
			inline void sint_to_string(TextOutBuffer & i_dest, INT_TYPE i_value)
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

			// using a temporary variable to avoid the warning "conditional expression is constant"
			unsigned radix = RADIX;
			if (radix == 16)
			{
				i_dest << "0x";
			}

			if (is_negative)
			{
				i_dest << '-';
			}

			for (int digit_index = used_buffer - 1; digit_index >= 0; digit_index--)
			{
				i_dest << buffer[digit_index];
			}
		}

		// uint_to_string - converts an unsigned int to a string
		template <unsigned RADIX, typename INT_TYPE>
			inline void uint_to_string(TextOutBuffer & i_dest, INT_TYPE i_value)
		{
			static_assert(!std::numeric_limits<INT_TYPE>::is_signed, "uint_to_string is for unsigned ints");

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
			
			// using a temporary variable to avoid the warning "conditional expression is constant"
			unsigned radix = RADIX;
			if (radix == 16)
			{
				i_dest << "0x";
			}

			for (int digit_index = used_buffer - 1; digit_index >= 0; digit_index--)
			{
				i_dest << buffer[digit_index];
			}
		}

		// sint_from_string - converts a string to an unsigned
		template <typename INT_TYPE>
			inline bool sint_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_buffer, INT_TYPE & o_value)
		{
			static_assert(std::numeric_limits<INT_TYPE>::is_signed, "sint_from_string is for signed ints");

			i_source.accept_whitespaces();

			const bool negative = i_source.accept("-");

			i_source.accept_whitespaces();

			const bool hex = i_source.accept("0x") || i_source.accept("0X");
			
			const char * curr_digit = i_source.next_char();
			const char * const end_of_buffer = i_source.end_of_buffer();
			INT_TYPE result = 0;
			if (hex)
			{
				while (curr_digit < end_of_buffer)
				{
					if (*curr_digit >= '0' && *curr_digit <= '9')
					{
						result *= 16;
						result += *curr_digit - '0';
					}
					else if (*curr_digit >= 'A' && *curr_digit <= 'F')
					{
						result *= 16;
						result += *curr_digit - 'A';
					}
					else if (*curr_digit >= 'a' && *curr_digit <= 'f')
					{
						result *= 16;
						result += *curr_digit - 'a';
					}
					else
					{
						break;
					}
					curr_digit++;
				}
			}
			else
			{
				while (curr_digit < end_of_buffer)
				{
					if (*curr_digit >= '0' && *curr_digit <= '9')
					{
						result *= 10;
						result += *curr_digit - '0';
					}
					else
					{
						break;
					}
					curr_digit++;
				}
			}

			const size_t accepted_digits = curr_digit - i_source.next_char();
			if (accepted_digits == 0)
			{
				i_error_buffer << "missing digits";
			}

			i_source.manual_advance(accepted_digits);

			if (negative)
			{
				result = -result;
			}

			o_value = result;
			return accepted_digits > 0;
		}

		// uint_from_string - converts a string to an unsigned
		template <typename INT_TYPE>
			inline bool uint_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_buffer, INT_TYPE & o_value)
		{
			static_assert(!std::numeric_limits<INT_TYPE>::is_signed, "uint_from_string is for unsigned ints");

			i_source.accept_whitespaces();			

			const bool hex = i_source.accept("0x") || i_source.accept("0X");
			
			const char * curr_digit = i_source.next_char();
			const char * const end_of_buffer = i_source.end_of_buffer();
			INT_TYPE result = 0;
			if (hex)
			{
				while (curr_digit < end_of_buffer)
				{
					if (*curr_digit >= '0' && *curr_digit <= '9')
					{
						result *= 16;
						result += *curr_digit - '0';
					}
					else if (*curr_digit >= 'A' && *curr_digit <= 'F')
					{
						result *= 16;
						result += *curr_digit - 'A';
					}
					else if (*curr_digit >= 'a' && *curr_digit <= 'f')
					{
						result *= 16;
						result += *curr_digit - 'a';
					}
					else
					{
						break;
					}
					curr_digit++;
				}
			}
			else
			{
				while (curr_digit < end_of_buffer)
				{
					if (*curr_digit >= '0' && *curr_digit <= '9')
					{
						result *= 10;
						result += *curr_digit - '0';
					}
					else
					{
						break;
					}
					curr_digit++;
				}
			}

			const size_t accepted_digits = curr_digit - i_source.next_char();
			if (accepted_digits == 0)
			{
				i_error_buffer << "missing digits";
			}

			i_source.manual_advance(accepted_digits);

			o_value = result;
			return accepted_digits > 0;
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

		// signed integers assign_from_string

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, int8_t & o_dest)
	{
		return details::sint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, int16_t & o_dest)
	{
		return details::sint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, int32_t & o_dest)
	{
		return details::sint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, int64_t & o_dest)
	{
		return details::sint_from_string(i_source, i_error_dest, o_dest);
	}


			// unsigned integers assign_from_string

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, uint8_t & o_dest)
	{
		return details::uint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, uint16_t & o_dest)
	{
		return details::uint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, uint32_t & o_dest)
	{
		return details::uint_from_string(i_source, i_error_dest, o_dest);
	}

	bool assign_from_string(TextInBuffer & i_source, TextOutBuffer & i_error_dest, uint64_t & o_dest)
	{
		return details::uint_from_string(i_source, i_error_dest, o_dest);
	}
}
