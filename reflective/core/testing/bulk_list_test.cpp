
namespace reflective
{
#if REFLECTIVE_ENABLE_TESTING

	namespace details
	{
		namespace BulkListTest
		{
			class TestAllocatorBase
			{
			public:

				class NoLeakScope
				{
				public:
					NoLeakScope()
					{
						REFLECTIVE_TEST_ASSERT(t_map.size() == 0);
					}

					~NoLeakScope()
					{
						REFLECTIVE_TEST_ASSERT(t_map.size() == 0);
					}

					NoLeakScope(const NoLeakScope &) = delete;
					NoLeakScope & operator = (const NoLeakScope &) = delete;
				};

				void * alloc(size_t i_size)
				{
					void * block = operator new (i_size);
					AllocationEntry entry;
					entry.m_size = i_size;
					auto res = t_map.insert(std::make_pair(block, entry));
					REFLECTIVE_INTERNAL_ASSERT(res.second);
					return block;
				}

				void free(void * i_block)
				{
					auto it = t_map.find(i_block);
					REFLECTIVE_INTERNAL_ASSERT(it != t_map.end());
					t_map.erase(it);
					operator delete (i_block);
				}

			private:
				struct AllocationEntry
				{
					size_t m_size;
				};
				static thread_local std::unordered_map<void*, AllocationEntry> t_map;
			};

			thread_local std::unordered_map<void*, TestAllocatorBase::AllocationEntry> TestAllocatorBase::t_map;

			template <class TYPE> class TestAllocator : private TestAllocatorBase
			{
			public:
				typedef TYPE value_type;

				TestAllocator() {}

				template <class OTHER_TYPE> TestAllocator(const TestAllocator<OTHER_TYPE>& /*i_other*/) { }

				TYPE * allocate(std::size_t i_count)
				{
					return static_cast<TYPE *>( TestAllocatorBase::alloc(i_count * sizeof(TYPE)) );
				}

				void deallocate(TYPE * i_block, std::size_t /*i_count*/)
				{
					TestAllocatorBase::free(i_block);
				}

				template <typename OTHER_TYPE>
				bool operator == (const TestAllocator<OTHER_TYPE> &) const
				{
					return true;
				}

				template <typename OTHER_TYPE>
				bool operator != (const TestAllocator<OTHER_TYPE> &) const
				{
					return false;
				}				
			};

			using TestString = std::basic_string<char, std::char_traits<char>, TestAllocator<char> >;
			using TestBulkListString = BulkList< TestString, TestAllocator<TestString> >;

			void bulk_list_test_insert(TestBulkListString i_list, size_t i_at, size_t i_count)
			{
				using namespace BulkListTest;

				std::vector<TestString> vec(i_list.begin(), i_list.end());

				auto const res1 = i_list.insert(std::next(i_list.cbegin(), i_at), i_count, TestString("42"));
				auto const res2 = vec.insert(std::next(vec.cbegin(), i_at), i_count, TestString("42"));
				std::vector<TestString> vec_1(i_list.begin(), i_list.end());
				REFLECTIVE_TEST_ASSERT(vec == vec_1);

				auto const dist1 = std::distance(i_list.begin(), res1);
				auto const dist2 = std::distance(vec.begin(), res2);
				REFLECTIVE_TEST_ASSERT(dist1 == dist2);
			}

			#ifdef _MSC_VER
				#pragma warning(push)
				#pragma warning(disable: 4324) // structure was padded due to alignment specifier
			#endif

			struct alignas(1) StructB_1 { char m_member = 42; };
			struct alignas(2) StructB_2 { char m_member = 42; };
			struct alignas(4) StructB_4 { int m_member = 42; };
			struct alignas(8) StructB_8 { int m_member = 42; };
			struct alignas(16) StructB_16 { int m_member = 42; };
			struct alignas(32) StructB_32 { int m_member = 42; };
			struct alignas(64) StructB_64 { int m_member = 42; };
			struct alignas(128) StructB_128 { int m_member = 42; };
			struct alignas(256) StructB_256 { int m_member = 42; };

			template <size_t VALUE, typename BASE> struct AlignHelper {
				static const size_t value = VALUE > std::alignment_of<BASE>::value ? VALUE : std::alignment_of<BASE>::value;
			};

			template <typename BASE> struct alignas(AlignHelper<1, BASE>::value) StructA_1 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<2, BASE>::value) StructA_2 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<4, BASE>::value) StructA_4 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<8, BASE>::value) StructA_8 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<16, BASE>::value) StructA_16 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<32, BASE>::value) StructA_32 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<64, BASE>::value) StructA_64 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<128, BASE>::value) StructA_128 : BASE { };
			template <typename BASE> struct alignas(AlignHelper<256, BASE>::value) StructA_256 : BASE { };

			#ifdef _MSC_VER
				#pragma warning(pop)
			#endif

			void test1()
			{
				TestAllocatorBase::NoLeakScope leak_detector;

				const auto list = TestBulkListString::make();
				static_assert(sizeof(list) == sizeof(void*) * 2, "If the allocator is stateless BulkList is documented to be big as two pointers");
				REFLECTIVE_TEST_ASSERT(list.begin() == list.end());
				REFLECTIVE_TEST_ASSERT(list.size() == 0);
				REFLECTIVE_TEST_ASSERT(list == TestBulkListString());

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

			void test2()
			{
				TestAllocatorBase::NoLeakScope leak_detector;

				const auto list = TestBulkListString::make(TestString("1"), TestString("2"), TestString("3"));
				REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 0) == "1");
				REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 1) == "2");
				REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 2) == "3");
				REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 2) != list.end());
				REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 3) == list.end());
				REFLECTIVE_TEST_ASSERT(list.size() == 3);
				REFLECTIVE_TEST_ASSERT(list != TestBulkListString());

				for (size_t i = 0; i <= list.size(); i++)
				{
					for (size_t j = 0; j < 4; j++)
					{
						bulk_list_test_insert(list, i, j);
					}
				}

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

				// test erase
				for (size_t i = 0; i <= list.size(); i++)
				{
					for (size_t j = i; j <= list.size(); j++)
					{
						auto list_5 = list;
						vector<TestString> vec(list_5.begin(), list_5.end());
						auto vec_res = vec.erase(std::next(vec.cbegin(), i), std::next(vec.cbegin(), j));
						auto lst_res = list_5.erase(std::next(list_5.cbegin(), i), std::next(list_5.cbegin(), j) );
						vector<TestString> vec1(list_5.begin(), list_5.end());
						REFLECTIVE_TEST_ASSERT(vec == vec1);
					}
				}
			}

			template <typename BASE_CLASS>
				void typed_alignment_test()
			{
				using List = BulkList< BASE_CLASS, TestAllocator<BASE_CLASS> >;
				
				std::vector<List> lists = {
					List::make(),
					List::make(StructA_16<BASE_CLASS>()),
					List::make(StructA_16<BASE_CLASS>(), StructA_32<BASE_CLASS>()),
					List::make(
						StructA_16<BASE_CLASS>(),
						StructA_8<BASE_CLASS>(),
						StructA_256<BASE_CLASS>(),
						StructA_64<BASE_CLASS>(),
						StructA_4<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_2<BASE_CLASS>(),
						StructA_32<BASE_CLASS>()),
					List::make(
						StructA_16<BASE_CLASS>(),
						StructA_8<BASE_CLASS>(),
						StructA_256<BASE_CLASS>(),
						StructA_8<BASE_CLASS>(),
						StructA_64<BASE_CLASS>(),
						StructA_4<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_2<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_64<BASE_CLASS>(),
						StructA_4<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_1<BASE_CLASS>(),
						StructA_2<BASE_CLASS>(),
						StructA_16<BASE_CLASS>(),
						StructA_32<BASE_CLASS>()) };

				lists.insert(lists.begin() + lists.size()/2, 20, List::make(StructA_256<BASE_CLASS>(), StructA_32<BASE_CLASS>()) );

				for (const auto & list : lists)
				{
					for (const auto & element : list)
					{
						REFLECTIVE_TEST_ASSERT(element.m_member == 42);
					}
				}
			}

			void test3()
			{
				TestAllocatorBase::NoLeakScope leak_detector;

				typed_alignment_test<StructB_1>();
				typed_alignment_test<StructB_2>();
				typed_alignment_test<StructB_4>();
				typed_alignment_test<StructB_8>();
				typed_alignment_test<StructB_16>();
				typed_alignment_test<StructB_32>();
				typed_alignment_test<StructB_64>();
				typed_alignment_test<StructB_128>();
				typed_alignment_test<StructB_256>();
			}
		}
	}

	void bulk_list_test(CorrectnessTestContext & /*i_context*/)
	{
		details::BulkListTest::test1();
		details::BulkListTest::test2();
		details::BulkListTest::test3();
	}

#endif
}