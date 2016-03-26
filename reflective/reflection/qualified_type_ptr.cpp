#include "qualified_type_ptr.h"

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

	#if REFLECTIVE_ENABLE_TESTING

	namespace details
	{
		template <size_t CV_COUNT>
			void QualifiedTypePtr_unit_test_cvs(const QualifiedTypePtr & i_q_type, const CV_Flags(&i_cvs)[CV_COUNT])
		{
			REFLECTIVE_TEST_ASSERT(i_q_type.indirection_levels() + 1 == CV_COUNT);
			for (size_t i = 0; i < CV_COUNT; i++)
			{
				REFLECTIVE_TEST_ASSERT(i_q_type.cv_flags(i) == i_cvs[i]);
			}
		}

		template <typename TYPE>
			void QualifiedTypePtr_unit_test_type()
		{
			REFLECTIVE_TEST_ASSERT(get_type<TYPE&>() == get_type<TYPE*const>());

			// test <TYPE>
			{ const auto q_type_ptr = get_type<TYPE>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::None };
			QualifiedTypePtr_unit_test_cvs(q_type_ptr, cvs); 
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

			// test <volatile TYPE>
			{ const auto q_type_ptr = get_type<volatile TYPE>();
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<TYPE>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			const CV_Flags cvs[] = { CV_Flags::Volatile };
			QualifiedTypePtr_unit_test_cvs(q_type_ptr, cvs); 
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

			// test <const TYPE &>
			{ const auto q_type_ptr = get_type<const TYPE &>();
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
			QualifiedTypePtr_unit_test_cvs(q_type_ptr, cvs); 
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

			// test <TYPE *const*volatile**>
			{ const auto q_type_ptr = get_type<TYPE*const*volatile**&>();
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
			QualifiedTypePtr_unit_test_cvs(q_type_ptr, cvs);
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }
		}

		QualifiedTypePtr qualified_type_from_string(const char * i_string)
		{
			std::istringstream std_stream(i_string);
			InTxtStreamAdapt<std::istream> stream(std_stream);
			return stream.read<QualifiedTypePtr>();
		}

	} // namespace details

	void QualifiedTypePtr::unit_test()
	{
		using namespace details;

		// test empty
		{ QualifiedTypePtr q_type_ptr;
		REFLECTIVE_TEST_ASSERT(q_type_ptr.is_empty());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == nullptr);
		REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == nullptr);
		REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 0);
		REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); }

		// test types
		QualifiedTypePtr_unit_test_type<float>();
		QualifiedTypePtr_unit_test_type<std::vector<int>>();

		REFLECTIVE_TEST_ASSERT(get_type<const void *>().is_const(1));
		REFLECTIVE_TEST_ASSERT(get_type<void *const>().is_const(0));
		
		{
			QualifiedTypePtr q_type_ptr(get_naked_type<float>(), { CV_Flags::Const | CV_Flags::Volatile, CV_Flags::None, CV_Flags::Volatile });
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<float>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 2);			
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(0) == (CV_Flags::Const | CV_Flags::Volatile) );
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(1) == CV_Flags::None);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(2) == CV_Flags::Volatile);
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr());
			REFLECTIVE_TEST_ASSERT(q_type_ptr == get_type<float volatile * * volatile const >());
		}
		{
			REFLECTIVE_TEST_ASSERT(QualifiedTypePtr(get_naked_type<void>(), {} ) == QualifiedTypePtr(get_naked_type<void>(), { CV_Flags::None }));
		}

		{
			QualifiedTypePtr q_type_ptr(get_naked_type<void>(), { CV_Flags::Const | CV_Flags::Volatile, CV_Flags::None, CV_Flags::Volatile });
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(0));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
			REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.is_volatile(2));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 2);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(0) == (CV_Flags::Const | CV_Flags::Volatile));
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(1) == CV_Flags::None);
			REFLECTIVE_TEST_ASSERT(q_type_ptr.cv_flags(2) == CV_Flags::Volatile);
			REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
			REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr());
			REFLECTIVE_TEST_ASSERT(q_type_ptr == get_type<void volatile * * volatile const >());
		}

		// test <void *const*volatile**>
		{ const auto q_type_ptr = get_type<void*const*volatile**&>();
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
		REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr); 
		REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

		// test <const void *>
		{ const auto q_type_ptr = get_type<const void *>();
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
		REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(1));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
		REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
		REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

		// test <void * const>
		{ const auto q_type_ptr = get_type<void * const>();
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
		REFLECTIVE_TEST_ASSERT(q_type_ptr.is_const(0));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
		REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
		REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

		// test <void *>
		{ const auto q_type_ptr = get_type<void *>();
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_empty());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.final_type() == &get_naked_type<void>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.primary_type() == &get_naked_type<void*>());
		REFLECTIVE_TEST_ASSERT(q_type_ptr.indirection_levels() == 1);
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(0));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_const(1));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(0));
		REFLECTIVE_TEST_ASSERT(!q_type_ptr.is_volatile(1));
		REFLECTIVE_TEST_ASSERT(q_type_ptr == q_type_ptr);
		REFLECTIVE_TEST_ASSERT(q_type_ptr != QualifiedTypePtr()); }

		// test cv qualifiers for float volatile*const volatile*const*
		{ const auto q_type_ptr = get_type<float volatile*const volatile*const*>();
		const CV_Flags cvs[] = { CV_Flags::None, CV_Flags::Const, CV_Flags::Const | CV_Flags::Volatile, CV_Flags::Volatile };
		QualifiedTypePtr_unit_test_cvs(q_type_ptr, cvs); }

		// test <const int *> and <const * int> (they are the same type)
		REFLECTIVE_TEST_ASSERT(get_type<const int *>() == get_type<int const *>());

		/* QualifiedTypePtr is documented (as implementation note) to be big as two pointers. This static_assert is here
			to verify the correctness of this note, but if it would ever fail on some compiler, it may safely be removed
			(together with the implementation note), as no other assumptions on the size of QualifiedTypePtr are present
			in the library. */
		static_assert(sizeof(QualifiedTypePtr) == sizeof(void*) * 2, "QualifiedTypePtr is not big as two pointers, as documented.");


		#define CHECK_TYPE(TYPE) \
			REFLECTIVE_TEST_ASSERT(get_type<TYPE>() == details::qualified_type_from_string(#TYPE))

		CHECK_TYPE(float);
		CHECK_TYPE(const float);
		CHECK_TYPE(volatile float const);
		CHECK_TYPE(const float ***);
		CHECK_TYPE(volatile float *const volatile*const*);
		CHECK_TYPE(float *const volatile***);
		CHECK_TYPE(float *volatile**);
		CHECK_TYPE(float&);
		CHECK_TYPE(const float&);
		CHECK_TYPE(volatile float const&);
		CHECK_TYPE(const float ***&);
		CHECK_TYPE(volatile float *const volatile*const*&&);
		CHECK_TYPE(float *const volatile***&);
		CHECK_TYPE(float *volatile**&&);
		CHECK_TYPE(float&&);
		CHECK_TYPE(const float**);
		CHECK_TYPE(volatile float **const);
		CHECK_TYPE(const float ***const);
		CHECK_TYPE(volatile const float *const volatile*const*);
		CHECK_TYPE(float *const volatile***);
		CHECK_TYPE(float *volatile**);
		CHECK_TYPE(float**&&);
		CHECK_TYPE(const float****&);
		CHECK_TYPE(volatile float *const&);
		CHECK_TYPE(const float *&&);
		CHECK_TYPE(volatile float *const volatile*const*&&);
		CHECK_TYPE(float *const volatile***&);
		CHECK_TYPE(float *volatile**&&);

		CHECK_TYPE(void);
		CHECK_TYPE(const void);
		CHECK_TYPE(volatile void const);
		CHECK_TYPE(const void ***);
		CHECK_TYPE(volatile void *const volatile*const*);
		CHECK_TYPE(void *const volatile***);
		CHECK_TYPE(void *volatile**);
		//CHECK_TYPE(void&);
		//CHECK_TYPE(const void&);
		//CHECK_TYPE(volatile void const&);
		CHECK_TYPE(const void ***&);
		CHECK_TYPE(volatile void *const volatile*const*&&);
		CHECK_TYPE(void *const volatile***&);
		CHECK_TYPE(void *volatile**&&);
		//CHECK_TYPE(void&&);
		CHECK_TYPE(const void**);
		CHECK_TYPE(volatile void **const);
		CHECK_TYPE(const void ***const);
		CHECK_TYPE(volatile const void *const volatile*const*);
		CHECK_TYPE(void *const volatile***);
		CHECK_TYPE(void *volatile**);
		CHECK_TYPE(void**&&);
		CHECK_TYPE(const void****&);
		CHECK_TYPE(volatile void *const&);
		CHECK_TYPE(const void *&&);
		CHECK_TYPE(volatile void *const volatile*const*&&);
		CHECK_TYPE(void *const volatile***&);
		CHECK_TYPE(void *volatile**&&);

		#undef CHECK_TYPE
	}
	#endif
}