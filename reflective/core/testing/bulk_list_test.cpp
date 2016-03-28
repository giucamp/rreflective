
namespace reflective
{
#if REFLECTIVE_ENABLE_TESTING

	void bulk_list_test(CorrectnessTestContext & /*i_context*/)
	{
		/*{
			auto list = make_bulk_list<int>();
			REFLECTIVE_TEST_ASSERT(list.begin() == list.end());
			REFLECTIVE_TEST_ASSERT(list.size() == 0);
		}*/

		{
			auto list1 = make_bulk_list<int>(1, 2 + 2, 3);
			REFLECTIVE_TEST_ASSERT(*std::next(list1.begin(), 0) == 1);
			REFLECTIVE_TEST_ASSERT(*std::next(list1.begin(), 1) == 2 + 2);
			REFLECTIVE_TEST_ASSERT(*std::next(list1.begin(), 2) == 3);
			REFLECTIVE_TEST_ASSERT(std::next(list1.begin(), 2) != list1.end());
			REFLECTIVE_TEST_ASSERT(std::next(list1.begin(), 3) == list1.end());
			REFLECTIVE_TEST_ASSERT(list1.size() == 3);
		}
	}

#endif
}