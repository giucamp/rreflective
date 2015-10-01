#include "qualified_type_ptr.h"

namespace reflective
{
	QualifiedTypePtr::QualifiedTypePtr(const Type * i_final_type, size_t i_indirection_levels, size_t i_constness_word, size_t i_volatileness_word)
		: m_final_type(i_final_type), m_indirection_levels(i_indirection_levels), m_constness_word(i_constness_word), m_volatileness_word(i_volatileness_word)
	{
		REFLECTIVE_ASSERT( i_indirection_levels <= s_max_indirection_levels, "The indirection levels exceed s_max_indirection_levels" );
		REFLECTIVE_ASSERT( (i_constness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The constness word is not valid" );
		REFLECTIVE_ASSERT( (i_volatileness_word & ~((2 << i_indirection_levels) - 1)) == 0, "The volatileness word is not valid" );
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
		return false;
	}

	#if REFLECTIVE_ENABLE_TESTING
		void QualifiedTypePtr::unit_test()
		{
			// test empty
			{ QualifiedTypePtr q_type_ptr;
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == nullptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == nullptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0); 
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <float>
			{ const auto q_type_ptr = get_qualified_type<float>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <volatile float>
			{ const auto q_type_ptr = get_qualified_type<volatile float>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <const float &>
			{ const auto q_type_ptr = get_qualified_type<const float &>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

			// test <float *const*volatile**>
			{ const auto q_type_ptr = get_qualified_type<float*const*volatile**&>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_type<void*>());
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
		}
	#endif
}