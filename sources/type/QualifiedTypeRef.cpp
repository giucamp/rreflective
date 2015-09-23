
#include "QualifiedTypeRef.h"

namespace reflective
{
	QualifiedTypeRef::QualifiedTypeRef(const Type * i_leftmost_type, uint32_t i_indirection_levels, uint32_t i_constness_word, uint32_t i_volatileness_word)
		: m_leftmost_type(i_leftmost_type), m_indirection_levels(i_indirection_levels), m_constness_word(i_constness_word), m_volatileness_word(i_volatileness_word)
	{
		REFLECTIVE_ASSERT(i_indirection_levels <= s_max_indirection_levels, "The indirection levels exceed s_max_indirection_levels");
		REFLECTIVE_ASSERT( (i_constness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The constness word is not valid" );
		REFLECTIVE_ASSERT( (i_volatileness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The volatileness word is not valid" );
	}

	bool QualifiedTypeRef::operator == (const QualifiedTypeRef & i_source) const
	{
		return m_leftmost_type == i_source.m_leftmost_type &&
			m_indirection_levels == i_source.m_indirection_levels &&
			m_constness_word == i_source.m_constness_word &&
			m_volatileness_word == i_source.m_volatileness_word;
	}
}