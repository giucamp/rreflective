
namespace reflective
{
	#if REFLECTIVE_ENABLE_TESTING
		
		template <typename CHAR, typename CHAR_TRAITS >
			void unit_test(BasicStringView<CHAR, CHAR_TRAITS>**, CorrectnessTestContext & /*i_context*/)
		{
			using View = BasicStringView<CHAR, CHAR_TRAITS>;
			using String = std::basic_string<CHAR, CHAR_TRAITS>;

			const String str1 = "abc";
			const View view1(str1);

			REFLECTIVE_TEST_ASSERT(view1.size() == 3);
		}

	#endif
}

