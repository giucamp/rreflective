
namespace reflective
{
	inline const Type * QualifiedTypeRef::primary_type() const
	{ 
		if (m_indirection_levels == 0)
			return m_leftmost_type;
		else if (m_leftmost_type != nullptr)
			return &get_type<void*>();
		else
			return nullptr;
	}

	template <typename TYPE>
		inline QualifiedTypeRef get_qualified_type()
	{
		static_assert(StaticQualification<TYPE>::s_indirection_levels < QualifiedTypeRef::s_max_indirection_levels,
			"Maximum indirection level exceeded");

		return QualifiedTypeRef( &get_type<TYPE>(), 
			StaticQualification<TYPE>::s_indirection_levels,
			StaticQualification<TYPE>::s_constness_word,
			StaticQualification<TYPE>::s_volatileness_word );			
	}
}
