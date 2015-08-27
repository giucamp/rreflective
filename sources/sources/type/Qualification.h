
#pragma once
#include "..\reflective_common.h"
#include "..\type\type.h"

namespace reflective
{
	class Qualification
	{
	public:

		static const size_t s_max_indirection_levels = 14;

		size_t indirection_levels() const		{ return m_indirection_levels; }

		bool is_indirection_level_const(size_t i_indirection_level) const		{ return m_indirection_levels; }

	private:
		const Type & m_underlying_type;
		const uint32_t m_indirection_levels : 4;
		const uint32_t m_constness_word : 14;
		const uint32_t m_volatilenessness_word : 14;
	};
}
