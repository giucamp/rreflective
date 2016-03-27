
namespace reflective
{
	#if REFLECTIVE_ENABLE_TESTING
		
		template <typename CHAR, typename CHAR_TRAITS >
			void unit_test(BasicStringView<CHAR, CHAR_TRAITS>**, CorrectnessTestContext & i_context)
		{
			using namespace std;
			using View = BasicStringView<CHAR, CHAR_TRAITS>;
			using String = basic_string<CHAR, CHAR_TRAITS>;

			auto make_string = [](const char * i_source) {
				return String(i_source, i_source + strlen(i_source)); };
			
			const String str1 = make_string("abc");
			const View view1(str1);

			REFLECTIVE_TEST_ASSERT(view1.size() == 3);

			
			vector<String> random_strings;
			const size_t string_count = 500;
			random_strings.resize(string_count);
			for(size_t i = 0; i < string_count; i++)
			{
				random_strings[i] = i_context.random_string<CHAR, CHAR_TRAITS>(10);
			}
			for(size_t i = 0; i < string_count; i++)
			{
				const auto & str = random_strings[i];
				const auto view = View(str);

				// test comparison
				for (size_t j = 0; j < string_count; j++)
				{
					REFLECTIVE_TEST_ASSERT((str < random_strings[j]) == (view < View(random_strings[j])));
					REFLECTIVE_TEST_ASSERT((str <= random_strings[j]) == (view <= View(random_strings[j])));
					REFLECTIVE_TEST_ASSERT((str > random_strings[j]) == (view > View(random_strings[j])));
					REFLECTIVE_TEST_ASSERT((str >= random_strings[j]) == (view >= View(random_strings[j])));
					REFLECTIVE_TEST_ASSERT((str == random_strings[j]) == (view == View(random_strings[j])));
					REFLECTIVE_TEST_ASSERT((str != random_strings[j]) == (view != View(random_strings[j])));
				}

				// test substr
				{
					const size_t from = i_context.random_int<size_t>(0, str.length() + 1);
					const size_t count = i_context.random_int<size_t>(0, 1000);
					const auto sub_str = str.substr(from, count);
					REFLECTIVE_TEST_ASSERT(View(sub_str) == view.substr(from, count));
				}

				// test find, starts_with, ends_with
				{
					const auto target_string = i_context.random_string<CHAR>(5);
					const auto target_char = i_context.random_char<CHAR>();
					const auto from = i_context.random_int<size_t>(0, str.length() + 4);
					auto r1 = str.find(target_char, from);
					auto r2 = view.find(target_char, from);
					REFLECTIVE_TEST_ASSERT( (r1 == String::npos) ? (r2 == View::npos) : (r1 == r2) );
					r1 = str.find(target_string, from);
					r2 = view.find(target_string, from);
					REFLECTIVE_TEST_ASSERT((r1 == String::npos) ? (r2 == View::npos) : (r1 == r2));
					REFLECTIVE_TEST_ASSERT( (str.find(target_string) == 0) == view.starts_with(target_string));
					REFLECTIVE_TEST_ASSERT( (str.find(target_char) == 0) == view.starts_with(target_char));

					auto rev_str = str;
					auto rev_target = target_string;
					std::reverse(rev_str.begin(), rev_str.end());
					std::reverse(rev_target.begin(), rev_target.end());
					REFLECTIVE_TEST_ASSERT((rev_str.find(rev_target) == 0) == view.ends_with(target_string));
					REFLECTIVE_TEST_ASSERT((rev_str.find(target_char) == 0) == view.ends_with(target_char));
				}
			}
		}

	#endif
}

