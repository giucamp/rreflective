
namespace reflective
{
	namespace details
	{
		template <typename TYPE>
		class StaticQualification;

		// generic template for non pointer, non const and non volatile types
		template <typename TYPE> class StaticQualification
		{
		public:

			using UnderlyingType = TYPE; /* it's the final type, that is the type with maximum
										 indirection level. For example the following types have all 'int' as final
										 type: 'int', 'int &', 'int * * * const * const &'.
										 C++ language remainder: a reference is like a const pointer, that is 'int &' is
										 the same as 'int * const', except that the first exposes directly its last
										 level of indirection. */

			static const size_t s_indirection_levels = 0; /* the number of indirection levels,
														  which is zero for non-pointers, 1 for pointers, 2 for pointers to pointers,
														  and so on. */

			static const size_t s_constness_word = 0; /* specifies, for each indirection
													  level,  the constness of the type.
													  - 'int', 'int**', "int&": no const is present, constness_word is zero.
													  - 'int * * const': the first indirection level is const (that is, the
													  pointer cannot be assigned after initialization), so the
													  constness_word is 1 (only bit 0 set).
													  - 'const int ********': only the last indirection level (8) is const,
													  so the 8th bit of the constness_word is 1 (constness_word=256).
													  C++ language remainder: the const keyword can be prefix or postfix to
													  the type it applies to: 'const int *' and 'int const *' are the
													  same declarations. */

			static const size_t s_volatileness_word = 0;

			static const bool s_is_lvalue_reference = false;
			static const bool s_is_rvalue_reference = false;

		private:

		};

		// partial specialization for lvalue reference types (they are threated like const pointers)
		template <typename TYPE> class StaticQualification<TYPE&>
		{
		public:
			using UnderlyingType = typename StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
			static const size_t s_constness_word = 1 | (StaticQualification<TYPE>::s_constness_word << 1);
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
			static const bool s_is_lvalue_reference = true;
			static const bool s_is_rvalue_reference = false;
		};

		// partial specialization for rvalue reference types
		template <typename TYPE> class StaticQualification<TYPE&&>
		{
		public:
			using UnderlyingType = typename StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
			static const size_t s_constness_word = 1 | (StaticQualification<TYPE>::s_constness_word << 1);
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
			static const bool s_is_lvalue_reference = false;
			static const bool s_is_rvalue_reference = true;
		};

		// partial specialization for pointer types
		template <typename TYPE> class StaticQualification<TYPE*>
		{
		public:
			using UnderlyingType = typename  StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels + 1;
			static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word << 1;
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word << 1;
			static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
			static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
		};

		// partial specialization for const types
		template <typename TYPE> class StaticQualification<const TYPE>
		{
		public:
			using UnderlyingType = typename StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels;
			static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word | 1;
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word;
			static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
			static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
		};

		// partial specialization for volatile types
		template <typename TYPE> class StaticQualification<volatile TYPE>
		{
		public:
			using UnderlyingType = typename  StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels;
			static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word;
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word | 1;
			static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
			static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
		};
	}

	template <typename TYPE>
		inline QualifiedTypePtr get_qualified_type()
	{
		static_assert(details::StaticQualification<TYPE>::s_indirection_levels <= QualifiedTypePtr::s_max_indirection_levels,
			"Maximum indirection level exceeded");

		return QualifiedTypePtr(&get_type<details::StaticQualification<TYPE>::UnderlyingType>(),
			details::StaticQualification<TYPE>::s_indirection_levels,
			details::StaticQualification<TYPE>::s_constness_word,
			details::StaticQualification<TYPE>::s_volatileness_word);
	}

	inline QualifiedTypePtr::QualifiedTypePtr()
		: m_final_type(nullptr), m_indirection_levels(0), m_constness_word(0), m_volatileness_word(0)
	{

	}

	inline const Type * QualifiedTypePtr::primary_type() const
	{ 
		if (m_indirection_levels == 0)
		{
			// the pointer is empty or it is not a pointer
			return m_final_type;
		}
		else
		{
			// the type is a pointer
			REFLECTIVE_ASSERT(m_final_type != nullptr, "QualifiedTypePtr class invariant violated");			
			return &get_type<void*>();
		}
	}

	inline bool QualifiedTypePtr::is_const(size_t i_indirection_level) const
	{
		REFLECTIVE_ASSERT(i_indirection_level <= indirection_levels(), "indirection level out of bounds" );
		return (m_constness_word & (static_cast<uintptr_t>(1) << i_indirection_level)) != 0; 
	}

	inline bool QualifiedTypePtr::is_volatile(size_t i_indirection_level) const
	{
		REFLECTIVE_ASSERT(i_indirection_level <= indirection_levels(), "indirection level out of bounds" );
		return (m_volatileness_word & (static_cast<uintptr_t>(1) << i_indirection_level)) != 0;
	}

	template <typename OUT_STREAM>
		inline void QualifiedTypePtr::to_string(OUT_STREAM & i_dest) const
	{
		if (!is_empty())
		{
			i_dest << final_type()->name();

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
