#include "qualified_type_ptr.h"

namespace reflective
{
	QualifiedTypePtr::QualifiedTypePtr(const Type * i_final_type, size_t i_indirection_levels, size_t i_constness_word, size_t i_volatileness_word)
		: m_final_type(i_final_type), m_indirection_levels(i_indirection_levels), m_constness_word(i_constness_word), m_volatileness_word(i_volatileness_word)
	{
		REFLECTIVE_INTERNAL_ASSERT( i_indirection_levels <= s_max_indirection_levels, "The indirection levels exceed s_max_indirection_levels" );
		REFLECTIVE_INTERNAL_ASSERT( (i_constness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The constness word is not valid" );
		REFLECTIVE_INTERNAL_ASSERT( (i_volatileness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The volatileness word is not valid" );
	}

	bool QualifiedTypePtr::operator == (const QualifiedTypePtr & i_source) const
	{
		return m_final_type == i_source.m_final_type &&
			m_indirection_levels == i_source.m_indirection_levels &&
			m_constness_word == i_source.m_constness_word &&
			m_volatileness_word == i_source.m_volatileness_word;
	}

	bool QualifiedTypePtr::assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest)
	{
		const InStringBuffer initial_source = i_source;

		bool successful = true;
		size_t constness_word = 0, volatileness_word = 0;
		const Type * final_type = nullptr;
		size_t indirection_levels = 0; // this variable is not the index of the current i.l., but the number of i.l.
		
		for (;;)
		{
			i_source.accept_whitespaces();
			
			// accept "const"
			if (i_source.accept_literal("const"))
			{
				constness_word |= 1;
				i_source.accept_whitespaces();
			}

			// accept "volatile"
			if (i_source.accept_literal("volatile"))
			{
				volatileness_word |= 1;
				i_source.accept_whitespaces();
			}

			// only in the last indirection level (that is before any *, & or &&
			if (indirection_levels == 0)
			{
				// accept the final type
				if (i_source.accept_literal("float"))
				{
					i_source.accept_whitespaces();
					final_type = &get_naked_type<float>();
				}
			}

			if (!i_source.accept_literal("*"))
			{
				break;
			}

			constness_word <<= 1;
			volatileness_word <<= 1;
			indirection_levels++;
			if (indirection_levels > s_max_indirection_levels)
			{
				i_error_dest << "Exceeded the maximum number of indirection levels";
				successful = false;
				break;
			}
		}

		if (successful && final_type != nullptr)
		{
			// this has not modified unil now
			*this = QualifiedTypePtr(final_type, indirection_levels, constness_word, volatileness_word);
			return true;
		}
		else
		{
			// restore the source in case of failure
			i_source = initial_source;
			return false;
		}
	}

	#if REFLECTIVE_ENABLE_TESTING

		template <size_t CV_COUNT>
			void QualifiedTypePtr::unit_test_test_cvs(const QualifiedTypePtr & i_q_type, const CV_Flags(&i_cvs)[CV_COUNT])
		{
			REFLECTIVE_TEST_ASSERT(i_q_type.indirection_levels() + 1 == CV_COUNT );
			for (size_t i = 0; i < CV_COUNT; i++)
			{
				REFLECTIVE_TEST_ASSERT(i_q_type.cv_flags(i) == i_cvs[i]);
			}
		}

		template <typename TYPE>
			void QualifiedTypePtr::unit_test_type()
		{
			REFLECTIVE_TEST_ASSERT(get_qualified_type<TYPE&>() == get_qualified_type<TYPE*const>());

			// test <TYPE>
			{ const auto q_type_ptr = get_qualified_type<TYPE>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::None };
			unit_test_test_cvs(q_type_ptr, cvs); }

			// test <volatile TYPE>
			{ const auto q_type_ptr = get_qualified_type<volatile TYPE>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::Volatile };
			unit_test_test_cvs(q_type_ptr, cvs); }

			// test <const TYPE &>
			{ const auto q_type_ptr = get_qualified_type<const TYPE &>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::Const, CV_Flags::Const };
			unit_test_test_cvs(q_type_ptr, cvs); }

			// test <TYPE *const*volatile**>
			{ const auto q_type_ptr = get_qualified_type<TYPE*const*volatile**&>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 5);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(2));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(3));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(4));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(5));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(3));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(4));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(5));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::Const, CV_Flags::None, CV_Flags::None, CV_Flags::Volatile, CV_Flags::Const, CV_Flags::None };
			unit_test_test_cvs(q_type_ptr, cvs); }
		}

		void QualifiedTypePtr::unit_test()
		{
			// test empty
			{ QualifiedTypePtr q_type_ptr;
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == nullptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == nullptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0); 
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test types
			unit_test_type<float>();
			unit_test_type<std::vector<int>>();

			// this must be rejected by the compiler
			// get_qualified_type<void>();

			// test <void *const*volatile**>
			{ const auto q_type_ptr = get_qualified_type<void*const*volatile**&>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 5);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(2));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(3));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(4));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(5));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(3));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(4));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(5));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <const void *>
			{ const auto q_type_ptr = get_qualified_type<const void *>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <void * const>
			{ const auto q_type_ptr = get_qualified_type<void * const>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <void *>
			{ const auto q_type_ptr = get_qualified_type<void *>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test cv qualifiers for float volatile*const volatile*const*
			{ const auto q_type_ptr = get_qualified_type<float volatile*const volatile*const*>();
			const CV_Flags cvs[] = { CV_Flags::None, CV_Flags::Const, CV_Flags::Const | CV_Flags::Volatile, CV_Flags::Volatile };
			unit_test_test_cvs(q_type_ptr, cvs); }

			// test <const int *> and <const * int> (they are the same type)
			REFLECTIVE_TEST_ASSERT(get_qualified_type<const int *>() == get_qualified_type<int const *>());

			/* QualifiedTypePtr is documented (as implementation note) to be big as two pointers. This static_assert is here 
				to verify the correctness of this note, but if it would ever fail on some compiler, it may safely be removed
				(togheter with the implementation note), as no other assumptions on the size of QualifiedTypePtr are present
				in the library. */
			static_assert(sizeof(QualifiedTypePtr) == sizeof(void*) * 2, "QualifiedTypePtr is not big as two pointers, as documented.");

			{ 
				InStringBuffer source("const float***");
				char err_buff[16];
				OutStringBuffer err(err_buff);
				QualifiedTypePtr q_type_ptr;
				bool res = source.read( q_type_ptr );
				REFLECTIVE_TEST_ASSERT(res && q_type_ptr == get_qualified_type<const float ***>());
			}
		}
	#endif
}