
namespace reflective
{
	QualifiedTypePtr::QualifiedTypePtr(const Type * i_final_type, size_t i_indirection_levels, size_t i_constness_word, size_t i_volatileness_word)
		: m_final_type(i_final_type), m_indirection_levels(i_indirection_levels), m_constness_word(i_constness_word), m_volatileness_word(i_volatileness_word)
	{
		REFLECTIVE_INTERNAL_ASSERT( i_indirection_levels <= s_max_indirection_levels);
		REFLECTIVE_INTERNAL_ASSERT( (i_constness_word & ~((2 << i_indirection_levels) - 1)) == 0 );
		REFLECTIVE_INTERNAL_ASSERT( (i_volatileness_word & ~((2 << i_indirection_levels) - 1)) == 0);
	}

	QualifiedTypePtr::QualifiedTypePtr(const Type & i_final_type, ArrayView<const CV_Flags> i_cv_flags)
		: m_final_type(&i_final_type)
	{
		REFLECTIVE_ASSERT(i_cv_flags.size() <= s_max_indirection_levels, "The number of indirection levels exceeds s_max_indirection_levels");
		
		uintptr_t constness_word = 0, volatileness_word = 0;

		const size_t size = i_cv_flags.size();
		for (size_t index = 0; index < size; index++)
		{
			CV_Flags flags = i_cv_flags[index];
			if ((flags & CV_Flags::Const) != CV_Flags::None)
			{
				constness_word |= uintptr_t(1) << index;
			}

			if ((flags & CV_Flags::Volatile) != CV_Flags::None)
			{
				volatileness_word |= uintptr_t(1) << index;
			}
		}
		m_constness_word = constness_word;
		m_volatileness_word = volatileness_word;
		m_indirection_levels = size > 0 ? size - 1 : 0;

		REFLECTIVE_INTERNAL_ASSERT(m_constness_word == constness_word && m_volatileness_word == volatileness_word );
	}

	bool QualifiedTypePtr::operator == (const QualifiedTypePtr & i_source) const
	{
		return m_final_type == i_source.m_final_type &&
			m_indirection_levels == i_source.m_indirection_levels &&
			m_constness_word == i_source.m_constness_word &&
			m_volatileness_word == i_source.m_volatileness_word;
	}
}