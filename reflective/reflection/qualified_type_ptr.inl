
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

		// partial specialization for const volatile types
		template <typename TYPE> class StaticQualification<const volatile TYPE>
		{
		public:
			using UnderlyingType = typename  StaticQualification<TYPE>::UnderlyingType;
			static const size_t s_indirection_levels = StaticQualification<TYPE>::s_indirection_levels;
			static const size_t s_constness_word = StaticQualification<TYPE>::s_constness_word | 1;
			static const size_t s_volatileness_word = StaticQualification<TYPE>::s_volatileness_word | 1;
			static const bool s_is_lvalue_reference = StaticQualification<TYPE>::s_is_lvalue_reference;
			static const bool s_is_rvalue_reference = StaticQualification<TYPE>::s_is_rvalue_reference;
		};
	}

	template <typename TYPE>
		inline QualifiedTypePtr get_type()
	{
		static_assert(details::StaticQualification<TYPE>::s_indirection_levels <= QualifiedTypePtr::s_max_indirection_levels,
			"Maximum indirection level exceeded");

		return QualifiedTypePtr(&get_naked_type<typename details::StaticQualification<TYPE>::UnderlyingType>(),
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
			REFLECTIVE_INTERNAL_ASSERT(m_final_type != nullptr);
			return &get_naked_type<void*>();
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
		OUT_STREAM & operator << (OUT_STREAM & i_dest, const QualifiedTypePtr & i_qt)
	{
		if (!i_qt.is_empty())
		{
			i_dest << i_qt.final_type()->name();

			uintptr_t level = 0;
			const uintptr_t ind_levels = i_qt.indirection_levels();
			do {

				if (i_qt.is_const(level))
				{
					i_dest << " const";
				}

				if (i_qt.is_volatile(level))
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
		return i_dest;
	}

	template <typename UNDERLYING_STREAM>
		InTxtStreamAdapt<UNDERLYING_STREAM, char> & operator >> (InTxtStreamAdapt<UNDERLYING_STREAM, char> & i_source, QualifiedTypePtr & o_dest_qualified_type)
	{
		size_t constness_word = 0, volatileness_word = 0;
		const Type * final_type = nullptr;
		size_t indirection_levels = 0; // this variable is not the index of the current i.l., but the number of i.l.'s so far
		
		for (;;)
		{
			i_source.accept_whitespaces();

			if (i_source.accept_literal("const")) // accept "const"
			{
				constness_word |= 1;
			}
			else if (i_source.accept_literal("volatile")) // accept "volatile"
			{
				volatileness_word |= 1;
			}
			else if (i_source.accept_char('*'))
			{
				constness_word <<= 1;
				volatileness_word <<= 1;
				indirection_levels++;
				if (indirection_levels > QualifiedTypePtr::s_max_indirection_levels)
				{
					break;
				}
			}
			else if (i_source.accept_char('&') || i_source.accept_literal("&&"))
			{
				constness_word <<= 1;
				constness_word |= 1;
				volatileness_word <<= 1;
				indirection_levels++;
				break;
			}
			else if (indirection_levels == 0 && final_type == nullptr) // only in the last indirection level (that is before any *, & or &&
			{
				// accept the final type
				std::string type_name;
				parse_type_name(i_source, type_name);
				final_type = GlobalRegistry::instance().find_type_by_full_name(type_name);
				if (final_type == nullptr)
				{
					i_source.error_stream() << "Unknwon type: '" << type_name << "'" << std::endl;
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (indirection_levels > QualifiedTypePtr::s_max_indirection_levels)
		{
			i_source.error_stream() << "Exceeded the maximum number of indirection levels (" << QualifiedTypePtr::s_max_indirection_levels << ")" << std::endl;
		}
		else if (final_type == nullptr)
		{
			i_source.error_stream() << "Missing final type" << std::endl;
		}
		else
		{
			o_dest_qualified_type = QualifiedTypePtr(final_type, indirection_levels, constness_word, volatileness_word);
		}

		return i_source;
	}
}
