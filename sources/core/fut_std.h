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
	#define REFLECTIVE_CONSTEXPR

	#define REFLECTIVE_NOEXCEPT
	
	/** This header provides a temporary, not fuly not compliant implementation of some classes which are not 
		in C++14, but should be included in the C++17. To avoid confusion with the standard, the names defined
		are all camel case or inside the static class reflective::Ext, so that in the future they may become
		aliases of the standard names. */

	template <typename TYPE>
		class ArrayView
	{
	public:

		ArrayView()
			: m_objects(nullptr), m_size(0)
		{
		}

		ArrayView(TYPE * i_objects, size_t i_size)
			: m_objects(i_objects), m_size(i_size)
		{
		}

		template <size_t SIZE>
			ArrayView(TYPE(&i_objects)[SIZE])
				: m_objects(i_objects), m_size(SIZE)
		{
		}

		ArrayView(std::initializer_list< typename std::remove_const< TYPE>::type > i_initializer_list)
			: m_objects(i_initializer_list.begin()), m_size(i_initializer_list.end() - i_initializer_list.begin())
		{ }

		TYPE * data() const { return m_objects; }

		TYPE * begin() const { return m_objects; }

		TYPE * end() const { return m_objects + m_size; }

		TYPE * cbegin() const { return m_objects; }

		TYPE * cend() const { return m_objects + m_size; }

		TYPE & operator [] (size_t i_index)
		{
			assert(i_index < m_size);
			return m_objects[i_index];
		}

		const TYPE & operator [] (size_t i_index) const
		{
			assert(i_index < m_size);
			return m_objects[i_index];
		}

		size_t size() const { return m_size; }

	private:
		TYPE * m_objects;
		size_t m_size;
	};

	/** Non-owning range of contigous char - see http://en.cppreference.com/w/cpp/experimental/basic_string_view */
	template <typename CHAR, typename CHAR_TRAITS = std::char_traits<CHAR> >
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

		REFLECTIVE_CONSTEXPR BasicStringView() REFLECTIVE_NOEXCEPT
			: m_chars(nullptr), m_size(0)
				{ }

		REFLECTIVE_CONSTEXPR BasicStringView(const std::string & i_source) REFLECTIVE_NOEXCEPT
			: m_chars(i_source.data()), m_size(i_source.length())
				{ }

		REFLECTIVE_CONSTEXPR BasicStringView(const BasicStringView & i_source) = default REFLECTIVE_NOEXCEPT;

		REFLECTIVE_CONSTEXPR BasicStringView(const char * i_c_string)
			: m_chars(i_c_string), m_size(CHAR_TRAITS::length(i_c_string))
				{ }

		REFLECTIVE_CONSTEXPR BasicStringView(const char * i_c_string, size_t i_size)
			: m_chars(i_c_string), m_size(i_size)
				{ }

		BasicStringView & operator = (const BasicStringView & i_source) = default REFLECTIVE_NOEXCEPT;
		

		REFLECTIVE_CONSTEXPR const char * begin() const REFLECTIVE_NOEXCEPT { return m_chars; }

		REFLECTIVE_CONSTEXPR const char * end() const REFLECTIVE_NOEXCEPT { return m_chars + m_size; }

		REFLECTIVE_CONSTEXPR const char * cbegin() const REFLECTIVE_NOEXCEPT { return m_chars; }

		REFLECTIVE_CONSTEXPR const char * cend() const REFLECTIVE_NOEXCEPT { return m_chars + m_size; }

		REFLECTIVE_CONSTEXPR const char & operator [] (size_t i_index) const REFLECTIVE_NOEXCEPT
		{
			assert(i_index < m_size);
			return m_chars[i_index];
		}

		REFLECTIVE_CONSTEXPR const char & at(size_t i_index) const
		{
			if (i_index >= m_size)
			{
				throw std::out_of_range("index out of range to reflective::BasicStringView::at");
			}
			return m_chars[i_index];
		}

		REFLECTIVE_CONSTEXPR const char & front() const REFLECTIVE_NOEXCEPT
		{
			assert(m_size > 0);
			return m_chars[0];
		}

		REFLECTIVE_CONSTEXPR const char & back() const REFLECTIVE_NOEXCEPT
		{
			assert(m_size > 0);
			return m_chars[m_size - 1];
		}

		REFLECTIVE_CONSTEXPR const char * data() const { return m_chars; }


		REFLECTIVE_CONSTEXPR size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }

		REFLECTIVE_CONSTEXPR size_t length() const REFLECTIVE_NOEXCEPT { return m_size; }

		REFLECTIVE_CONSTEXPR size_t max_size() const REFLECTIVE_NOEXCEPT { return std::numeric_limits<size_t>::max(); }

		REFLECTIVE_CONSTEXPR bool empty() const REFLECTIVE_NOEXCEPT { return m_size == 0; }

		REFLECTIVE_CONSTEXPR void remove_prefix( size_t i_char_count )
		{
			assert(i_char_count <= m_size);
			m_chars += i_char_count;
			m_size -= i_char_count;
		}

		REFLECTIVE_CONSTEXPR void remove_suffix(size_t i_char_count) REFLECTIVE_NOEXCEPT
		{
			assert(i_char_count <= m_size);
			m_size -= i_char_count;
		}		
		
		REFLECTIVE_CONSTEXPR void swap(BasicStringView i_other) REFLECTIVE_NOEXCEPT
		{
			std::swap(m_chars, i_other.m_chars);
			std::swap(m_size, i_other.m_size);
		}

		REFLECTIVE_CONSTEXPR int compare(BasicStringView i_other) const REFLECTIVE_NOEXCEPT
		{
			auto result = CHAR_TRAITS::compare(m_chars, i_other.m_chars, std::min(m_size, i_other.m_size));
			if (result != 0)
			{
				return result;
			}
			else
			{
				return static_cast<int>(m_size) - static_cast<int>(i_other.m_size);
			}			
		}

		// REFLECTIVE_CONSTEXPR void copy(....) // not impemented

		REFLECTIVE_CONSTEXPR BasicStringView substr(size_t i_pos, size_t i_count = npos) const
		{
			if (i_pos >= m_size)
			{
				throw std::out_of_range("index out of range to reflective::BasicStringView::at");
			}

			return BasicStringView(m_chars, std::min(i_count, m_size - i_pos) );
		}

		REFLECTIVE_CONSTEXPR size_t find(char i_target_char, size_t i_pos = 0) const
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

		REFLECTIVE_CONSTEXPR size_t find(BasicStringView i_target_string, size_t i_pos = 0) const
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

		bool operator == (const BasicStringView i_other) const
		{
			return compare(i_other) == 0;
		}

		bool operator != (const BasicStringView i_other) const
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
		
	private:
		const char * m_chars;
		size_t m_size;
	};
	
	using StringView = BasicStringView<char>;
	using WStringView = BasicStringView<wchar_t>;
	using u16StringView = BasicStringView<char16_t>;
	using u32StringView = BasicStringView<char32_t>;

	inline std::ostream & operator << (std::ostream & i_dest, const StringView & i_string)
	{
		i_dest.write(i_string.data(), i_string.length());
		return i_dest;
	}

	// void_t invented by Walter E. Brown - it will be available is std from C++17 on
	template< class... >
		using VoidT = void;

	class Ext
	{
	public:
		
		template <typename CONTAINER, typename ELEMENT>
			static auto find(CONTAINER & i_container, ELEMENT && i_value) -> decltype(std::find(i_container.begin(), i_container.end(), i_value))
		{
			return std::find(i_container.begin(), i_container.end(), i_value);
		}

		template <typename CONTAINER, typename PREDICATE>
			static auto find_if(CONTAINER & i_container, PREDICATE && i_predicate) -> decltype(std::find_if(i_container.begin(), i_container.end(), i_predicate))
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate);
		}

		template <typename CONTAINER, typename ELEMENT>
			static bool contains(CONTAINER & i_container, ELEMENT && i_value)
		{
			return std::find(i_container.begin(), i_container.end(), i_value) != i_container.end();
		}

		template <typename CONTAINER, typename PREDICATE>
			static bool contains_if(CONTAINER & i_container, PREDICATE && i_predicate)
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate) != i_container.end();
		}
	};
}
