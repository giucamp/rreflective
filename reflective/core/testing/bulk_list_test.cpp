
namespace reflective
{
#if REFLECTIVE_ENABLE_TESTING

	namespace details
	{
		void bulk_list_test_insert(BulkList<int> i_list, size_t i_at, size_t i_count)
		{
			std::vector<int> vec(i_list.begin(), i_list.end());
			
			i_list.insert(std::next(i_list.cbegin(), i_at), i_count, 42);
			vec.insert(std::next(vec.cbegin(), i_at), i_count, 42);
			
			std::vector<int> vec_1(i_list.begin(), i_list.end());

			REFLECTIVE_TEST_ASSERT(vec == vec_1);
		}
	}

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

			details::bulk_list_test_insert(list, 1, 1 );

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
			const auto list = alloc_bulk_list<int>(std::allocator<int>(), 1, 2 + 2, 3);
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