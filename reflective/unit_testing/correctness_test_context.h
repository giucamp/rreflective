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

namespace reflective
{
	class CorrectnessTestContext
	{
	public:

		CorrectnessTestContext()
			: m_random(std::random_device()())
		{

		}

		template <typename INT_TYPE>
			INT_TYPE random_int(INT_TYPE i_exclusive_upper)
		{
			return std::uniform_int_distribution<INT_TYPE>(0, i_exclusive_upper - 1)(m_random);
		}

		template <typename INT_TYPE>
			INT_TYPE random_int(INT_TYPE i_inclusive_lower, INT_TYPE i_exclusive_upper)
		{
			return std::uniform_int_distribution<INT_TYPE>(i_inclusive_lower, i_exclusive_upper - 1)(m_random);
		}

		template <typename CHAR>
			CHAR random_char()
		{
			return CHAR('A') + static_cast<CHAR>(random_int<int>('Z' - 'A'));
		}

		template <typename CHAR, typename CHAR_TRAITS=std::char_traits<CHAR> >
			std::basic_string<CHAR, CHAR_TRAITS> random_string(size_t i_exclusive_length_upper)
		{
			const size_t len = random_int<size_t>(i_exclusive_length_upper);
			std::basic_string<CHAR, CHAR_TRAITS> result;
			result.reserve(len);
			for (size_t i = 0; i < len; i++)
			{
				result += random_char<CHAR>();
			}
			return result;
		}

		std::mt19937 & random_generator() { return m_random; }

	private:
		std::mt19937 m_random;
	};

} // namespace reflective