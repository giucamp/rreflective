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
	/*template <typename UNDERLYING_STREAM, typename CHAR = UNDERLYING_STREAM::char_type, typename CHAR_TRAITS = std::char_traits<CHAR> >
		class OutTxtStreamAdapt;*/
	template <typename UNDERLYING_STREAM, typename CHAR = typename UNDERLYING_STREAM::char_type, typename CHAR_TRAITS = typename std::char_traits<CHAR> >
		class InTxtStreamAdapt;

	namespace details
	{
		namespace sfinae
		{
			/* Definition of HasStdStreamingOperators, that detects if the type TYPE supports streaming 
				operators (<< and >>) for  basic_ostream and basic_istream. Insiperd by the soulution of
				iammilind in the question http://stackoverflow.com/questions/5839357/detect-operator-support-with-decltype-sfinae
				but using C++11 features (is_same<NoSupport,decltype()> instead of sizeof). */

			struct NoSupport {};
			template<typename TYPE> TYPE & dummy_get_any();

			// dummy streaming operators, not implemented - we use SFINAE to check if the result is NoSupport
			template <typename TYPE, typename CHAR, typename CHAR_TRAITS>
				NoSupport operator << (std::basic_ostream<CHAR, CHAR_TRAITS> &, const TYPE &);
			template <typename TYPE, typename CHAR, typename CHAR_TRAITS>
				NoSupport operator >> (std::basic_istream<CHAR, CHAR_TRAITS> &, TYPE &);

			template <typename TYPE, typename CHAR, typename CHAR_TRAITS> struct HasStdStreamingOperators
			{
				//static const bool has_out = !std::is_same< NoSupport, decltype(dummy_get_any<std::basic_ostream<CHAR, CHAR_TRAITS>>() << dummy_get_any<TYPE>()) >::value;
				static const bool has_in = !std::is_same< NoSupport, decltype(dummy_get_any<std::basic_istream<CHAR, CHAR_TRAITS>>() >> dummy_get_any<TYPE>()) >::value;
			};
		}
	}

	/*template <typename CHAR, typename CHAR_TRAITS>
		class OutTxtStreamAdapt< std::basic_ostream<CHAR, CHAR_TRAITS>, CHAR, CHAR_TRAITS > final
	{
	public:

		OutTxtStreamAdapt(std::basic_ostream<CHAR, CHAR_TRAITS> & i_underlying_stream)
			: m_underlying_stream(i_underlying_stream) { }

		template <typename TYPE>
			typename std::enable_if< details::sfinae::HasStdStreamingOperators<TYPE, CHAR, CHAR_TRAITS>::has_out, OutTxtStreamAdapt & >::type
				operator << (TYPE && i_value)
		{
			m_underlying_stream << std::forward<TYPE>(i_value);
			return *this;
		}

		OutTxtStreamAdapt & operator << (const char * i_null_terminated_string)
		{
			m_underlying_stream << i_null_terminated_string;
			return *this;
		}

		OutTxtStreamAdapt(const OutTxtStreamAdapt &) = delete;
		OutTxtStreamAdapt(OutTxtStreamAdapt &&) = delete;
		OutTxtStreamAdapt & operator = (const OutTxtStreamAdapt &) = delete;
		OutTxtStreamAdapt & operator = (OutTxtStreamAdapt &&) = delete;

	private:
		std::basic_ostream<CHAR, CHAR_TRAITS> & m_underlying_stream;
	};*/

	template <typename CHAR, typename CHAR_TRAITS>
		class InTxtStreamAdapt< std::basic_istream<CHAR, CHAR_TRAITS>, CHAR, CHAR_TRAITS > final
	{
	public:

		InTxtStreamAdapt(std::basic_istream<CHAR, CHAR_TRAITS> & i_underlying_stream )
				: m_underlying_stream(i_underlying_stream) { }

		InTxtStreamAdapt(const InTxtStreamAdapt &) = delete;
		InTxtStreamAdapt(InTxtStreamAdapt &&) = delete;
		InTxtStreamAdapt & operator = (const InTxtStreamAdapt &) = delete;
		InTxtStreamAdapt & operator = (InTxtStreamAdapt &&) = delete;

		bool accept_char(CHAR i_char)
		{
			if (bufferize(1) && m_buffer[0] == i_char)
			{
				advance(1);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool accept_string(const BasicStringView<CHAR, CHAR_TRAITS> & i_string_view)
		{
			if (bufferize(i_string_view.size()) && 
				BasicStringView<CHAR, CHAR_TRAITS>(m_buffer.data(), i_string_view.size()) == i_string_view)
			{
				advance(i_string_view.length());
				return true;
			}
			else
			{
				return false;
			}
		}

		template <size_t ARRAY_SIZE>
			bool accept_literal(const CHAR(&i_array)[ARRAY_SIZE])
		{
			return accept_string(StringView(i_array, ARRAY_SIZE - 1));
		}

		template <typename PREDICATE>
			BasicStringView<CHAR, CHAR_TRAITS> accept_while(PREDICATE && i_predicate)
		{
			size_t result_length = 0;
			for (;;)
			{
				if (result_length >= m_buffer.size())
				{
					bufferize(10);
					if (result_length >= m_buffer.size())
					{
						break;
					}
				}
				if (!i_predicate(m_buffer[result_length]))
				{
					break;
				}
				result_length++;
			}
			m_tmp_string.assign(m_buffer.data(), m_buffer.data() + result_length);
			advance(result_length);
			return BasicStringView<CHAR, CHAR_TRAITS>(m_tmp_string.data(), m_tmp_string.size());
		}

		size_t accept_whitespaces()
		{
			return accept_while([](CHAR i_char)->bool { return isspace(CHAR_TRAITS::to_int_type(i_char)) != 0; } ).size();
		}

		BasicStringView<CHAR, CHAR_TRAITS> accept_identifier()
		{
			size_t char_index = 0;
			return accept_while([&char_index](CHAR i_char)->bool {
				const auto int_char = CHAR_TRAITS::to_int_type(i_char);
				return (char_index++ == 0) ? (isalpha(int_char) != 0) : (isalnum(int_char) != 0);
			});
		}

		template <typename TYPE>
			TYPE read()
		{
			TYPE result;
			*this >> result;
			return result;
		}

		template <typename TYPE>
			typename std::enable_if< details::sfinae::HasStdStreamingOperators<TYPE, CHAR, CHAR_TRAITS>::has_in, InTxtStreamAdapt & >::type operator >> (TYPE && i_value)
		{
			m_underlying_stream >> std::forward<TYPE>(i_value);
			return *this;
		}

		std::basic_ostream<CHAR, CHAR_TRAITS> & error_stream() { return m_error_stream; }

	private:

		bool bufferize(size_t i_required_size)
		{
			auto curr_buffer_length = m_buffer.size();
			if (curr_buffer_length >= i_required_size)
			{
				return true;
			}
			else
			{
				const size_t size_to_read = std::max(i_required_size, s_min_buffer_size);
				m_buffer.resize(size_to_read);
				m_underlying_stream.read(m_buffer.data() + curr_buffer_length, size_to_read - curr_buffer_length);
				curr_buffer_length += m_underlying_stream.gcount();
				m_buffer.resize(curr_buffer_length);
				return curr_buffer_length >= i_required_size;
			}			
		}

		void advance(size_t i_size)
		{
			m_buffer.erase(m_buffer.begin(), m_buffer.begin() + i_size);
		}
		
	private:
		vector<CHAR> m_buffer, m_tmp_string;
		std::basic_istream<CHAR, CHAR_TRAITS> & m_underlying_stream;
		std::basic_ostringstream<CHAR, CHAR_TRAITS> m_error_stream;
		static const size_t s_min_buffer_size = 512;
	};
}