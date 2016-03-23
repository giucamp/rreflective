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
	template <typename UNDERLYING_STREAM, typename CHAR = UNDERLYING_STREAM::char_type, typename CHAR_TRAITS = std::char_traits<CHAR> >
		class OutTxtStreamAdapt;
	template <typename UNDERLYING_STREAM, typename CHAR = UNDERLYING_STREAM::char_type, typename CHAR_TRAITS = std::char_traits<CHAR> >
		class InTxtStreamAdapt;

	namespace details
	{
		namespace sfinae
		{
			// insiperd by the soulution of iammilind in the question http://stackoverflow.com/questions/5839357/detect-operator-support-with-decltype-sfinae
			// but using is_same<NoSupport,decltype()> instead of sizeof.

			struct NoSupport {};
			template<typename TYPE> TYPE & dummy_get_any();

			// dummy streaming operators, not implemented - we use SFINAE to check if the result is NoSupport
			template <typename TYPE, typename CHAR, typename CHAR_TRAITS>
			NoSupport operator << (std::basic_ostream<CHAR, CHAR_TRAITS> &, const TYPE &);
			template <typename TYPE, typename CHAR, typename CHAR_TRAITS>
			NoSupport operator >> (std::basic_istream<CHAR, CHAR_TRAITS> &, TYPE &);

			template <typename TYPE, typename CHAR, typename CHAR_TRAITS> struct HasStdStreamingOperators
			{
				static const bool has_out = !std::is_same< NoSupport, decltype(dummy_get_any<std::basic_ostream<CHAR, CHAR_TRAITS>>() << dummy_get_any<TYPE>()) >::value;
				static const bool has_in = !std::is_same< NoSupport, decltype(dummy_get_any<std::basic_istream<CHAR, CHAR_TRAITS>>() >> dummy_get_any<TYPE>()) >::value;
			};
		}
	}

	template <typename CHAR, typename CHAR_TRAITS>
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
	};

	template <typename CHAR, typename CHAR_TRAITS>
	class InTxtStreamAdapt< std::basic_istream<CHAR, CHAR_TRAITS>, CHAR, CHAR_TRAITS > final
	{
	public:

		InTxtStreamAdapt(std::basic_istream<CHAR, CHAR_TRAITS> & i_underlying_stream)
			: m_underlying_stream(i_underlying_stream) { }

		template <typename TYPE>
		typename std::enable_if< details::sfinae::HasStdStreamingOperators<TYPE, CHAR, CHAR_TRAITS>::has_in, InTxtStreamAdapt & >::type operator >> (TYPE && i_value)
		{
			m_underlying_stream >> std::forward<TYPE>(i_value);
			return *this;
		}

		InTxtStreamAdapt & operator << (const char * i_null_terminated_string)
		{
			m_underlying_stream << i_null_terminated_string;
			return *this;
		}

		InTxtStreamAdapt(const InTxtStreamAdapt &) = delete;
		InTxtStreamAdapt(InTxtStreamAdapt &&) = delete;
		InTxtStreamAdapt & operator = (const InTxtStreamAdapt &) = delete;
		InTxtStreamAdapt & operator = (InTxtStreamAdapt &&) = delete;

		void apend_error(const char * i_string)
		{
			m_underlying_stream.setstate(std::ios_base::failbit);
			m_pending_error += i_string;
		}

		void apend_error(const std::basic_string<CHAR, CHAR_TRAITS> & i_string)
		{
			m_underlying_stream.setstate(std::ios_base::failbit);
			m_pending_error += i_string;
		}

	private:
		std::basic_istream<CHAR, CHAR_TRAITS> & m_underlying_stream;
		std::basic_string<CHAR, CHAR_TRAITS> m_pending_error;
	};
}