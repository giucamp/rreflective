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

		uint32_t random_uint32(uint32_t i_exclusive_upper)
		{
			return std::uniform_int_distribution<uint32_t>(0, i_exclusive_upper - 1)(m_random);
		}

		uint32_t random_uint32(uint32_t i_inclusive_lower, uint32_t i_exclusive_upper)
		{
			return std::uniform_int_distribution<uint32_t>(i_inclusive_lower, i_exclusive_upper - 1)(m_random);
		}

		char random_char()
		{
			return 'A' + static_cast<char>(random_uint32('Z' - 'A'));
		}

		std::string random_string(uint32_t i_exclusive_length_upper)
		{
			const uint32_t len = random_uint32(i_exclusive_length_upper);
			std::string result;
			result.reserve(len);
			for (uint32_t i = 0; i < len; i++)
			{
				result += random_char();
			}
			return result;
		}

		std::mt19937 random_generator() { return m_random; }

	private:
		std::mt19937 m_random;
	};

} // namespace reflective