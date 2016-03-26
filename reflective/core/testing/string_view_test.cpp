
namespace reflective
{
	template <typename CHAR, typename CHAR_TRAITS >
		void setup_type(TypeSetupContext<BasicStringView<CHAR, CHAR_TRAITS>> & i_context)
	{

	}

	namespace testing
	{
		template <typename CHAR, typename CHAR_TRAITS >
			void string_view_test()
		{
			using View = BasicStringView<CHAR, CHAR_TRAITS>;
			using String = std::basic_string<CHAR, CHAR_TRAITS>;

			const String str1 = "abc";
			const View view1(str1);

			REFLECTIVE_TEST_ASSERT(view1.size() == 3);
		}
	}
}

