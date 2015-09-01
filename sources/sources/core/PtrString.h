
#pragma once
#include "../reflective_common.h"

namespace reflective
{
	class PtrString
	{

	private:
		const char * const m_chars;

	public:

		PtrString()
			: m_chars(nullptr) {}

		PtrString(const char * i_chars)
			: m_chars(i_chars) { }

		const char * c_str() const { return m_chars; }

		const char * begin() const { return m_chars; }

		const char * end() const { return m_chars + std::strlen(m_chars) + 1; }

		const char * cbegin() const { return m_chars; }

		const char * cend() const { return m_chars + std::strlen(m_chars) + 1; }
	};
}
