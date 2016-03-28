
namespace reflective
{
#if REFLECTIVE_ENABLE_TESTING

	void bulk_list_test(CorrectnessTestContext & /*i_context*/)
	{
		{
			const auto list = make_bulk_list<int>();
			REFLECTIVE_TEST_ASSERT(list.begin() == list.end());
			REFLECTIVE_TEST_ASSERT(list.size() == 0);
			REFLECTIVE_TEST_ASSERT(list == make_bulk_list<int>() );

			// check copy constructor and copy assignment
			auto list_1 = list;
			REFLECTIVE_TEST_ASSERT(list == list_1);
			auto list_2 = list_1;
			list_2 = list_1;
			REFLECTIVE_TEST_ASSERT(list == list_2);

			// check move constructor and move assignment
			auto list_3 = std::move(list_1);
			REFLECTIVE_TEST_ASSERT(list == list_3);
			auto list_4 = list_2;
			list_4 = std::move(list_2);
			REFLECTIVE_TEST_ASSERT(list == list_4);
		}

		{
			const auto list = make_bulk_list<int>(1, 2 + 2, 3);
			REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 0) == 1);
			REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 1) == 2 + 2);
			REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 2) == 3);
			REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 2) != list.end());
			REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 3) == list.end());
			REFLECTIVE_TEST_ASSERT(list.size() == 3);
			REFLECTIVE_TEST_ASSERT(list != make_bulk_list<int>());

			// check copy constructor and copy assignment
			auto list_1 = list;
			REFLECTIVE_TEST_ASSERT(list == list_1);
			auto list_2 = list_1;
			list_2 = list_1;
			REFLECTIVE_TEST_ASSERT(list == list_2);

			// check move constructor and move assignment
			auto list_3 = std::move(list_1);
			REFLECTIVE_TEST_ASSERT(list == list_3);
			auto list_4 = list_2;
			list_4 = std::move(list_2);
			REFLECTIVE_TEST_ASSERT(list == list_4);
		}
	}

#endif
}