
#pragma once
#include <string>
#include <iostream>
#include <cstdlib>
#include <random>
#include <functional>
#include <algorithm>
#include "..\sources\reflective.h"

class Rand
{
public:

	Rand() : m_random(std::random_device()())
	{
	}

	unsigned long long generate_ull()
	{
		return std::uniform_int_distribution<unsigned long long>()(m_random);
	}

	signed long long generate_sll()
	{
		return std::uniform_int_distribution<signed long long>()(m_random);
	}

	uint32_t generate_uint32(uint32_t i_exclusive_upper)
	{
		return std::uniform_int_distribution<uint32_t>(0, i_exclusive_upper - 1)(m_random);
	}

	uint32_t generate_uint32(uint32_t i_inclusive_lower, uint32_t i_exclusive_upper)
	{
		return std::uniform_int_distribution<uint32_t>(i_inclusive_lower, i_exclusive_upper - 1)(m_random);
	}

	char generate_char()
	{
		return 'A' + static_cast<char>(generate_uint32('Z' - 'A'));
	}

	std::string generate_string(uint32_t i_exclusive_length_upper)
	{
		const uint32_t len = generate_uint32(i_exclusive_length_upper);
		std::string result;
		result.reserve(len);
		for (uint32_t i = 0; i < len; i++)
		{
			result += generate_char();
		}
		return result;
	}

	std::mt19937 get_generator() { return m_random; }

private:
	std::mt19937 m_random;
};