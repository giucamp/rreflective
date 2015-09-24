
namespace reflective
{
	inline const Type * QualifiedTypePtr::primary_type() const
	{ 
		if (m_indirection_levels == 0)
			return m_leftmost_type;
		else if (m_leftmost_type != nullptr)
			return &get_type<void*>();
		else
			return nullptr;
	}

	template <typename TYPE>
		inline QualifiedTypePtr get_qualified_type()
	{
		static_assert(StaticQualification<TYPE>::s_indirection_levels < QualifiedTypePtr::s_max_indirection_levels,
			"Maximum indirection level exceeded");

		return QualifiedTypePtr( &get_type<TYPE>(), 
			StaticQualification<TYPE>::s_indirection_levels,
			StaticQualification<TYPE>::s_constness_word,
			StaticQualification<TYPE>::s_volatileness_word );			
	}

	template <typename OUT_STREAM>
		inline void reflective::QualifiedTypePtr::to_string(OUT_STREAM & i_dest) const
	{
		if (!is_empty())
		{
			i_dest << leftmost_type()->name();

			uintptr_t level = 0;
			const uintptr_t ind_levels = indirection_levels();
			do {

				if (is_const(level))
				{
					i_dest << " const";
				}

				if (is_volatile(level))
				{
					i_dest << " volatile";
				}

				if (level < ind_levels)
				{
					i_dest << " *";
				}

				level++;

			} while (level <= ind_levels);
		}
	}
}
