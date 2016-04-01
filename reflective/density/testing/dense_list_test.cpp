
#include "..\testing_utils.h"

namespace reflective
{
#if REFLECTIVE_ENABLE_TESTING

	namespace density
	{
		namespace details
		{
			namespace DenseListTest
			{
				using TestString = std::basic_string<char, std::char_traits<char>, TestAllocator<char> >;
				using TestDenseListString = DenseList< TestString, TestAllocator<TestString> >;

				void dense_list_test_insert(TestDenseListString i_list, size_t i_at, size_t i_count)
				{
					using namespace DenseListTest;

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

				#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below

					struct StructB_1{ char m_member = 42; std::aligned_storage<1, 1> m_aligned; };
					struct StructB_2 { char m_member = 42; std::aligned_storage<1, 2> m_aligned; };
					struct StructB_4 { int m_member = 42; std::aligned_storage<1, 4> m_aligned; };
					struct StructB_8 { int m_member = 42; std::aligned_storage<1, 8> m_aligned; };
					struct StructB_16 { int m_member = 42; std::aligned_storage<1, 16> m_aligned; };
					struct StructB_32 { int m_member = 42; std::aligned_storage<1, 32> m_aligned; };
					struct StructB_64 { int m_member = 42; std::aligned_storage<1, 64> m_aligned; };
					struct StructB_128 { int m_member = 42; std::aligned_storage<1, 128> m_aligned; };
					struct StructB_256 { int m_member = 42; std::aligned_storage<1, 256> m_aligned; };

					template <typename BASE> struct StructA_1 : BASE { std::aligned_storage<1, 1 > m_aligned; };
					template <typename BASE> struct StructA_2 : BASE { std::aligned_storage<1, 2 > m_aligned; };
					template <typename BASE> struct StructA_4 : BASE { std::aligned_storage<1, 4 > m_aligned; };
					template <typename BASE> struct StructA_8 : BASE { std::aligned_storage<1, 8 > m_aligned; };
					template <typename BASE> struct StructA_16 : BASE { std::aligned_storage<1, 16 > m_aligned; };
					template <typename BASE> struct StructA_32 : BASE { std::aligned_storage<1, 32 > m_aligned; };
					template <typename BASE> struct StructA_64 : BASE { std::aligned_storage<1, 64 > m_aligned; };
					template <typename BASE> struct StructA_128 : BASE { std::aligned_storage<1, 128 > m_aligned; };
					template <typename BASE> struct StructA_256 : BASE { std::aligned_storage<1, 256 > m_aligned; };

				#else

					struct alignas(1) StructB_1{ char m_member = 42; };
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

				#endif

				#ifdef _MSC_VER
					#pragma warning(pop)
				#endif

				void test1()
				{
					TestAllocatorBase::NoLeakScope leak_detector;

					const auto list = TestDenseListString::make();
					static_assert(sizeof(list) == sizeof(void*) * 2, "If the allocator is stateless DenseList is documented to be big as two pointers");
					REFLECTIVE_TEST_ASSERT(list.begin() == list.end());
					REFLECTIVE_TEST_ASSERT(list.size() == 0);
					REFLECTIVE_TEST_ASSERT(list == TestDenseListString());

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

					const auto list = TestDenseListString::make(TestString("1"), TestString("2"), TestString("3"));
					REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 0) == "1");
					REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 1) == "2");
					REFLECTIVE_TEST_ASSERT(*std::next(list.begin(), 2) == "3");
					REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 2) != list.end());
					REFLECTIVE_TEST_ASSERT(std::next(list.begin(), 3) == list.end());
					REFLECTIVE_TEST_ASSERT(list.size() == 3);
					REFLECTIVE_TEST_ASSERT(list != TestDenseListString());

					for (size_t i = 0; i <= list.size(); i++)
					{
						for (size_t j = 0; j < 4; j++)
						{
							dense_list_test_insert(list, i, j);
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
							const auto vec_res = vec.erase(std::next(vec.begin(), i), std::next(vec.begin(), j));
							const auto lst_res = list_5.erase(std::next(list_5.begin(), i), std::next(list_5.begin(), j));
							vector<TestString> vec1(list_5.begin(), list_5.end());
							REFLECTIVE_TEST_ASSERT(vec == vec1);

							const auto lst_dist = std::distance(list_5.begin(), lst_res);
							const auto vec_dist = std::distance(vec.begin(), vec_res);
							REFLECTIVE_TEST_ASSERT(lst_dist == vec_dist);
						}
					}
				}

				template <typename BASE_CLASS>
					void typed_alignment_test()
				{
					using List = DenseList< BASE_CLASS, TestAllocator<BASE_CLASS> >;
				
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

				class Moveable
				{
					public:
						Moveable(int){}
						Moveable(Moveable &&) {}
						Moveable & operator = (Moveable &&) {}
						Moveable(const Moveable &) = delete;
						Moveable & operator = (const Moveable &) = delete;
				};

				void test4()
				{
					#if !defined(_MSC_VER) || _MSC_VER >= 1900 // disable for Visual Studio 2013 and below
						TestAllocatorBase::NoLeakScope leak_detector;
						using List = DenseList< Moveable, TestAllocator<Moveable> >;
						List::make(Moveable(1), Moveable(2));
					#endif
				}
			}
		}
	} // namespace density

	void dense_list_test(CorrectnessTestContext & /*i_context*/)
	{
		density::details::DenseListTest::test1();
		density::details::DenseListTest::test2();
		density::details::DenseListTest::test3();
		density::details::DenseListTest::test4();
	}

#endif
}