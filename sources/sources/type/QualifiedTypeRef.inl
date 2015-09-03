
namespace reflective
{
	inline const Type & QualifiedTypeRef::front_type() const
	{ 
		if (m_indirection_levels == 0)
			return m_underlying_type;
		else
			return get_type<void*>();
	}

	template <typename TYPE>
		inline QualifiedTypeRef get_qualified_type()
	{
		static_assert(StaticQualification<TYPE>::s_indirection_levels < s_max_indirection_levels);

		return QualifiedTypeRef(get_type<TYPE>(), 
			StaticQualification<TYPE>::s_indirection_levels,
			StaticQualification<TYPE>::s_constness_word,
			StaticQualification<TYPE>::s_volatileness_word );			
	}
}
