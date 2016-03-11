/***********************************************************************************

Copyright 2011-2012 Giuseppe Campana - giu.campana@gmail.com
All rights reserved

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY GIUSEPPE CAMPANA ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GIUSEPPE CAMPANA OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************/

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	/** Non-owning range of contiguous char - see http://en.cppreference.com/w/cpp/experimental/basic_string_view.
		This class represent a contiguous range of characters, whose storage is managed by the user. Accessing the
		content of the string view when the storage is no more valid, leads to undefined behavior.
		This implementation extends the one described in the standard with some methods useful for parsing (like 
		remove_prefix_char and remove_prefix_string).
	*/
	template <typename CHAR, typename CHAR_TRAITS >
		class BasicStringView
	{
	public:

		using traits_type = CHAR_TRAITS;
		using value_type = CHAR;
		using pointer = CHAR *;
		using const_pointer = const CHAR *;
		using reference = CHAR &;
		using const_reference = const CHAR &;
		using const_iterator = const CHAR *;
		using iterator = const CHAR *;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using size_type = std::size_t;
		using difference_type = std::ptrdiff_t;
		
		static REFLECTIVE_CONSTEXPR const size_t npos = static_cast<size_t>(-1);


					/**** ctors, assignment ****/

		/** Constructs an empty string view. For an empty view, begin() == end(), and range based loops end before the first iteration. */
		REFLECTIVE_CONSTEXPR BasicStringView() REFLECTIVE_NOEXCEPT
			: m_chars(nullptr), m_size(0)
				{ }

		/** Copy-constructs a string view. The new view will share the character buffer with the source, but each of them
			has its own begin-end markers. */
		REFLECTIVE_CONSTEXPR BasicStringView(const BasicStringView & i_source) REFLECTIVE_NOEXCEPT = default;

		/** Constructs a string view from a std::basic_string (that is a std::string, a std::wstring, etc.). If the source string gets reallocated, 
			truncated or destroyed, accessing the content of the string view will lead to undefined behavior. */
		REFLECTIVE_CONSTEXPR BasicStringView(const std::basic_string<CHAR, CHAR_TRAITS> & i_source) REFLECTIVE_NOEXCEPT
			: m_chars(i_source.data()), m_size(i_source.length())
				{ }

		/** Constructs a string view from a null-terminated string pointer */
		REFLECTIVE_CONSTEXPR BasicStringView(const CHAR * i_c_string)
			: m_chars(i_c_string), m_size(CHAR_TRAITS::length(i_c_string))
				{ }

		/** Constructs a string view from a pointer to the first char and the length. Accessing the content of the view when
			the memory pointed by i_first_char is no more valid, leads to undefined behavior. */
		REFLECTIVE_CONSTEXPR BasicStringView(const CHAR * i_first_char, size_t i_length)
			: m_chars(i_first_char), m_size(i_length)
				{ }

		/** Copy-assigns a string view. this view will share the character buffer with the source, but each of them
			has its own begin-end markers. */
		BasicStringView & operator = (const BasicStringView & i_source) REFLECTIVE_NOEXCEPT = default;
		


					/**** container-like methods ****/
		
		REFLECTIVE_CONSTEXPR const CHAR * begin() const REFLECTIVE_NOEXCEPT
		{
			return m_chars;
		}

		REFLECTIVE_CONSTEXPR const CHAR * end() const REFLECTIVE_NOEXCEPT
		{
			return m_chars + m_size;
		}

		REFLECTIVE_CONSTEXPR const CHAR * cbegin() const REFLECTIVE_NOEXCEPT
		{
			return m_chars;
		}

		REFLECTIVE_CONSTEXPR const CHAR * cend() const REFLECTIVE_NOEXCEPT
		{
			return m_chars + m_size;
		}

		REFLECTIVE_CONSTEXPR const CHAR & operator [] (size_t i_index) const REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(i_index < m_size, "invalid index for reflective::BasicStringView::operator []" );
			return m_chars[i_index];
		}

		REFLECTIVE_CONSTEXPR const CHAR & at(size_t i_index) const
		{
			if (i_index >= m_size)
			{
				throw std::out_of_range("index out of range to reflective::BasicStringView::at");
			}
			return m_chars[i_index];
		}

		REFLECTIVE_CONSTEXPR const CHAR & front() const REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(m_size > 0, "reflective::BasicStringView::front called on empty string");
			return m_chars[0];
		}

		REFLECTIVE_CONSTEXPR const CHAR & back() const REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(m_size > 0, "reflective::BasicStringView::back called on empty string");
			return m_chars[m_size - 1];
		}

		REFLECTIVE_CONSTEXPR const CHAR * data() const
		{
			return m_chars;
		}

		REFLECTIVE_CONSTEXPR size_t size() const REFLECTIVE_NOEXCEPT
		{
			return m_size;
		}

		REFLECTIVE_CONSTEXPR size_t length() const REFLECTIVE_NOEXCEPT
		{
			return m_size; 
		}

		REFLECTIVE_CONSTEXPR size_t max_size() const REFLECTIVE_NOEXCEPT
		{
			return std::numeric_limits<size_t>::max(); 
		}

		REFLECTIVE_CONSTEXPR bool empty() const REFLECTIVE_NOEXCEPT
		{
			return m_size == 0;
		}


						/**** const methods ****/

		/** Lexicographically compares the content of two string view, using the specified char traits instead of the one of this string view.
			@return 0 if the string views have the same content, a negative value if the first is less that the second, or a positive value otherwise. */
		template <typename OTHER_CHAR_TRAITS>
			int ot_compare(BasicStringView i_other) const REFLECTIVE_NOEXCEPT
		{
			auto result = OTHER_CHAR_TRAITS::compare(m_chars, i_other.m_chars, std::min(m_size, i_other.m_size));
			if (result != 0)
			{
				return result;
			}
			else
			{
				return static_cast<int>(m_size)-static_cast<int>(i_other.m_size);
			}
		}

		/** Lexicographically compares the content of two string view.
			@return 0 if the string views have the same content, a negative value if the first is less that the second, or a positive value otherwise. */
		REFLECTIVE_CONSTEXPR int compare(BasicStringView i_other) const REFLECTIVE_NOEXCEPT
		{
			return ot_compare<CHAR_TRAITS>(i_other);
		}

		/** Returns a new string view that represent a part of this view.
			@param i_pos zero-based index of the first char to include in the new view. If it is equal to the length of this view, the result
				is an empty view. If it is greater than the length, a std::out_of_range exception is thrown
			@param i_count optional value indicating the number of char to include in the result view. Any value is legal for this parameter. The actual length 
				of the view is the minimum between i_count and length() - i_pos. By default all the chars starting from i_pos are included. */
		REFLECTIVE_CONSTEXPR BasicStringView substr(size_t i_pos, size_t i_count = npos) const
		{
			if (i_pos > m_size)
			{
				throw std::out_of_range("index out of range to reflective::BasicStringView::at");
			}

			return BasicStringView(m_chars, std::min(i_count, m_size - i_pos));
		}

		REFLECTIVE_CONSTEXPR size_t find(CHAR i_target_char, size_t i_pos = 0) const REFLECTIVE_NOEXCEPT
		{
			if (i_pos >= m_size)
			{
				return npos;
			}

			auto const res = CHAR_TRAITS::find(m_chars + i_pos, m_size - i_pos, i_target_char);
			if (res != nullptr)
			{
				return res - m_chars;
			}
			else
			{
				return npos;
			}
		}

		REFLECTIVE_CONSTEXPR size_t find(BasicStringView i_target_string, size_t i_pos = 0) const REFLECTIVE_NOEXCEPT
		{
			if (i_target_string.size() == 0)
			{
				return 0; // the empty string is present in any string, even in another empty string
			}

			if (m_size <= i_pos)
			{
				return npos; // fails for search paste the end of this, or in any case if this is empty
			}

			auto curr_start = m_chars + i_pos;
			auto remaing_length = m_size;
			auto const first_target_char = i_target_string[0];
			auto compare_result = 0; // probably the optimizer will remove this initialization
			do {
				auto const res = CHAR_TRAITS::find(curr_start, remaing_length, first_target_char);
				if (res == nullptr)
				{
					return npos;
				}

				remaing_length -= res - curr_start;
				curr_start = res;
				if (i_target_string.length() > remaing_length)
				{
					return npos;
				}

				compare_result = CHAR_TRAITS::compare(curr_start, i_target_string.data(), i_target_string.length());

			} while (compare_result != 0);

			return curr_start - m_chars;
		}

		bool starts_with(BasicStringView<CHAR, CHAR_TRAITS> i_string) const REFLECTIVE_NOEXCEPT
		{
			auto const string_len = i_string.length();
			return m_size >= string_len &&
				CHAR_TRAITS::compare(m_chars, i_string.data(), string_len) == 0;
		}

		bool ends_with(BasicStringView<CHAR, CHAR_TRAITS> i_string) const REFLECTIVE_NOEXCEPT
		{
			auto const string_len = i_string.length();
			return m_size >= string_len &&
				CHAR_TRAITS::compare(m_chars + m_size - string_len, i_string.data(), string_len) == 0;
		}

		REFLECTIVE_CONSTEXPR bool starts_with(CHAR i_char) const REFLECTIVE_NOEXCEPT
		{
			return m_size >= 1 && CHAR_TRAITS::eq(m_chars[0], i_char);
		}

		REFLECTIVE_CONSTEXPR bool ends_with(CHAR i_char) const REFLECTIVE_NOEXCEPT
		{
			return m_size > 0 && CHAR_TRAITS::eq(m_chars[m_size - 1], i_char);
		}

		template <typename OTHER_CHAR_TRAITS>
			REFLECTIVE_CONSTEXPR bool ot_starts_with(CHAR i_char) const REFLECTIVE_NOEXCEPT
		{
			return m_size >= 1 && OTHER_CHAR_TRAITS::eq(*m_chars, i_char);
		}

		bool operator == (BasicStringView i_other) const
		{
			return compare(i_other) == 0;
		}

		bool operator != (BasicStringView i_other) const
		{
			return compare(i_other) != 0;
		}

		bool operator > (const BasicStringView i_other) const
		{
			return compare(i_other) > 0;
		}

		bool operator < (const BasicStringView i_other) const
		{
			return compare(i_other) < 0;
		}

		bool operator >= (const BasicStringView i_other) const
		{
			return compare(i_other) >= 0;
		}

		bool operator <= (const BasicStringView i_other) const
		{
			return compare(i_other) <= 0;
		}


		void copy_to_cstr(CHAR * i_dest, size_t i_buffer_size, size_t i_start_index = 0)
		{
			REFLECTIVE_ASSERT(i_buffer_size > 0, "reflective::BasicStringView::copy_to_cstr called with an empty buffer");

			size_t length_to_copy = std::min(i_buffer_size - 1, m_size >= i_start_index ? m_size - i_start_index : 0);
			CHAR_TRAITS::copy(i_dest, m_chars, length_to_copy);
			i_dest[length_to_copy] = 0;
		}

		template <size_t BUFFER_SIZE>
			void copy_to_cstr(CHAR(&i_dest)[BUFFER_SIZE], size_t i_start_index = 0)
		{
			copy_to_cstr(i_dest, BUFFER_SIZE, i_start_index);
		}


			/***  ***/

		void remove_prefix(size_t i_char_count) REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(i_char_count <= m_size, "reflective::BasicStringView::remove_prefix called with invalid param");
			m_chars += i_char_count;
			m_size -= i_char_count;
		}

		bool remove_prefix_char(CHAR i_char) REFLECTIVE_NOEXCEPT
		{
			if (starts_with(i_char))
			{
				m_chars++;
				m_size--;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool remove_prefix_string(BasicStringView i_string) REFLECTIVE_NOEXCEPT
		{
			if (starts_with(i_string))
			{
				remove_prefix(i_string.length());
				return true;
			}
			else
			{
				return false;
			}
		}

		template <size_t ARRAY_SIZE>
			bool remove_prefix_literal(const CHAR(&i_array)[ARRAY_SIZE]) REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(i_array[ARRAY_SIZE - 1] == 0, "the array must be null-terminated");
			return remove_prefix_string(StringView(i_array, ARRAY_SIZE - 1));
		}

		template <size_t ARRAY_SIZE>
			bool remove_suffix_literal(const CHAR(&i_array)[ARRAY_SIZE]) REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(i_array[ARRAY_SIZE - 1] == 0, "the array must be null-terminated");
			return remove_suffix_string(StringView(i_array, ARRAY_SIZE - 1));
		}

		bool remove_prefix_writespaces() REFLECTIVE_NOEXCEPT
		{
			bool some_space_removed = false;
			while (m_size > 0 && isspace(CHAR_TRAITS::to_int_type(*m_chars)) != 0)
			{
				m_chars++;
				m_size--;
				some_space_removed = true;
			}
			return some_space_removed;
		}

		template <typename PREDICATE>
			BasicStringView remove_prefix_while(PREDICATE && i_predicate) REFLECTIVE_NOEXCEPT
		{
			const CHAR * const original_chars = m_chars;
			while (m_size > 0 && i_predicate(*m_chars) )
			{
				m_chars++;
				m_size--;
			}
			return BasicStringView(original_chars, m_chars - original_chars);
		}

		BasicStringView remove_prefix_identifier()
		{
			const CHAR * const original_chars = m_chars;
			if (m_size > 0 && isalpha(CHAR_TRAITS::to_int_type(*m_chars)))
			{
				m_chars++;
				m_size--;

				while (m_size > 0 && isalnum(CHAR_TRAITS::to_int_type(*m_chars)))
				{
					m_chars++;
					m_size--;
				}
			}
			return BasicStringView(original_chars, m_chars - original_chars);
		}

		void remove_suffix(size_t i_char_count) REFLECTIVE_NOEXCEPT
		{
			REFLECTIVE_ASSERT(i_char_count <= m_size, "reflective::BasicStringView::remove_suffix called with invalid param");
			m_size -= i_char_count;
		}

		BasicStringView remove_suffix_identifier()
		{
			const auto prev_length = m_size;
			while (m_size > 0 && isalnum(m_chars + m_size - 1))
			{
				m_size--;
			}

			const auto suffix_length = prev_length - m_size;

			REFLECTIVE_INTERNAL_ASSERT(m_size >= suffix_length);
			m_size -= suffix_length;

			return BasicStringView(m_chars + m_size, suffix_length);
		}

		bool remove_suffix_string(BasicStringView i_string) REFLECTIVE_NOEXCEPT
		{
			if (dd starts_with(i_string))
			{
				remove_prefix(i_string.length());
				return true;
			}
			else
			{
				return false;
			}
		}

		void swap(BasicStringView i_other) REFLECTIVE_NOEXCEPT
		{
			std::swap(m_chars, i_other.m_chars);
			std::swap(m_size, i_other.m_size);
		}

		template <typename PREDICATE>
			void for_each_token(CHAR i_separator, PREDICATE && i_predicate) const
		{
			auto remaining = *this;
			auto has_more = remaining.length() > 0;
			while (has_more)
			{
				auto curr_token = remaining.remove_prefix_while( [i_separator](CHAR i_char)->bool { return i_char != i_separator; } );
				has_more = remaining.length() > 0;

				if (has_more)
				{
					remaining.remove_prefix(1);
				}
				i_predicate(curr_token);
			}
		}



		template <typename TYPE>
			bool read(TYPE & o_object, OutStringBuffer i_error)
		{
			return ReadAny<TYPE, has_assign_from_string<TYPE>::value>::read(*this, i_error, o_object);
		}

		template <typename TYPE>
			bool read(TYPE & o_object)
		{
			CHAR small_buffer[sizeof(int)];
			OutStringBuffer small_error_buffer(small_buffer);
			return ReadAny<TYPE, has_assign_from_string<TYPE>::value>::read(*this, small_error_buffer, o_object);
		}

	private:

		template <typename TYPE, bool HAS_ASSIGN_FROM_STRING_METHOD> struct ReadAny;
		template <typename TYPE> struct ReadAny < TYPE, true >
		{
			static bool read(BasicStringView & i_source, OutStringBuffer & i_error_dest, TYPE & o_object)
			{
				return o_object.assign_from_string(i_source, i_error_dest);
			}
		};
		template <typename TYPE> struct ReadAny < TYPE, false >
		{
			static bool read(BasicStringView & i_source, OutStringBuffer & i_error_dest, TYPE & o_object)
			{
				return assign_from_string(i_source, i_error_dest, o_object);
			}
		};

	private:
		const CHAR * m_chars;
		size_t m_size;
	};

	inline std::ostream & operator << (std::ostream & i_dest, const StringView & i_string)
	{
		i_dest.write(i_string.data(), i_string.length());
		return i_dest;
	}
}
