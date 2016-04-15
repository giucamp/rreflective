
#include "..\density\dense_fixed_queue.h"
#include "..\unit_testing\testing_utils.h"
#include <deque>
#include <complex>
#include <iostream>
#include "..\reflective_settings.h"

namespace reflective
{
	namespace details
	{
		namespace DenseFixedQueueTest
		{
			template <typename TYPE, typename UINT_TO_TYPE >
				void basic_test(size_t i_size, UINT_TO_TYPE && i_uint_to_type)
			{
				NoLeakScope no_leak_scope;

				using Queue = DenseFixedQueue<TYPE, TestAllocator<TYPE> >;
				Queue queue(i_size);
				std::deque<TYPE> std_deque;

				for (unsigned progr = 0; progr < 1000; progr++)
				{
					auto element = i_uint_to_type(progr);

					/* Try a try_copy_push, try_emplace, and try_emplace. std_deque is modified 
						to reflect these changes (they must always be equals). */
					size_t pushes = 0;
					if (queue.try_copy_push(Queue::RuntimeType::make<TYPE>(), &element))
						pushes++;
					if (queue.try_emplace<TYPE>(element))
						pushes++;
					{
						auto tmp = element;
						if (queue.try_move_push(Queue::RuntimeType::make<TYPE>(), &tmp))
							pushes++;
					}
					while (pushes > 0)
					{
						std_deque.push_back(element);
						pushes--;
					}

					if ((progr % 11) == 0)
					{
						/* Assign queue from a copy of itself. queue and std_deque must preserve the equality. */
						auto copy = queue;
						REFLECTIVE_TEST_ASSERT(copy.mem_free() == queue.mem_free());
						queue = copy;
						REFLECTIVE_TEST_ASSERT(copy.mem_free() == queue.mem_free());
						
						const auto size_1 = std::distance(queue.cbegin(), queue.cend());
						const auto size_2 = std::distance(copy.cbegin(), copy.cend());
						REFLECTIVE_TEST_ASSERT(size_1 == size_2);
					}
					else if ((progr % 5) == 0)
					{
						/* fill the queue with try_emplace until there is no more place. */
						unsigned i = 0;
						while (queue.try_emplace<TYPE>(i_uint_to_type(i)))
						{
							std_deque.push_back(i_uint_to_type(i));
							i++;
						}

						/* check that the free space is not enough not insert new elements. We must take
							account of alignment padding and buffer wrapping padding */
						const auto free = queue.mem_free();
						const auto max_element_requirement = ( sizeof(TYPE) + alignof(TYPE)+
							sizeof(Queue::RuntimeType) + alignof(Queue::RuntimeType) ) * 2;
						REFLECTIVE_TEST_ASSERT(free < max_element_requirement);
					}
					else if ( (progr % 4) == 0 )
					{
						/* fill the queue with try_push until there is no more place. */
						unsigned i = 0;
						while (queue.try_push(i_uint_to_type(i)))
						{
							std_deque.push_back(i_uint_to_type(i));
							i++;
						}

						/* check that the free space is not enough not insert new elements. We must take
							account of alignment padding and buffer wrapping padding */
						const auto free = queue.mem_free();
						const auto max_element_requirement = ( sizeof(TYPE) + alignof(TYPE)+
							sizeof(Queue::RuntimeType) + alignof(Queue::RuntimeType) ) * 2;
						REFLECTIVE_TEST_ASSERT(free < max_element_requirement);
					}
					else if ((progr % 2) == 0)
					{
						/* use try_push for progr times, or until there is no more place. */
						unsigned i = 0;
						while (i < progr && queue.try_push(i_uint_to_type(i)))
						{
							std_deque.push_back(i_uint_to_type(i));
							i++;
						}
					}
					else if ((progr % 3) == 0)
					{
						/* use consume for progr times, or until the queue is empty. */
						unsigned i = 0;
						while (!queue.empty() && i < progr)
						{
							queue.consume([&std_deque](const DenseFixedQueue<TYPE>::RuntimeType &, TYPE val) {
								REFLECTIVE_TEST_ASSERT(std_deque.size() > 0 && val == std_deque.front());
								std_deque.pop_front();
							});
							i++;
						}
					}
					else
					{
						/* call consume for progr times, or until the queue is empty. */
						unsigned i = 0;
						while (!queue.empty())
						{
							queue.consume([&std_deque](const DenseFixedQueue<TYPE>::RuntimeType &, TYPE val) {
								REFLECTIVE_TEST_ASSERT(std_deque.size() > 0 && val == std_deque.front());
								std_deque.pop_front();
							});
							i++;
						}
						REFLECTIVE_TEST_ASSERT(std_deque.empty());
						REFLECTIVE_TEST_ASSERT(queue.mem_free() == queue.mem_capacity());
					}

					// check for equality queue and std_deque
					REFLECTIVE_TEST_ASSERT(queue.empty() == std_deque.empty());
					if (!std_deque.empty())
					{
						REFLECTIVE_TEST_ASSERT(queue.front() == std_deque.front());
					}
					auto it1 = queue.cbegin();
					auto it2 = std_deque.cbegin();
					int u = 0;
					for (;;)
					{
						bool end1 = it1 == queue.cend();
						bool end2 = it2 == std_deque.cend();
						REFLECTIVE_TEST_ASSERT(end1 == end2);
						if (end1)
						{
							break;
						}
						REFLECTIVE_TEST_ASSERT(*it1 == *it2);
						++it1;
						++it2;
						u++;
					}
				}

				while (!queue.empty())
				{
					queue.pop();
					std_deque.pop_back();
				}
				REFLECTIVE_TEST_ASSERT(queue.empty());
			}

			template< typename UINT_TO_TYPE>
				void typed_basic_test(UINT_TO_TYPE && i_uint_to_type)
			{
				NoLeakScope no_leak_scope;

				using Type = decltype(i_uint_to_type(0u));
				details::DenseFixedQueueTest::basic_test<Type>(1024 * 16 + 1, i_uint_to_type);
				details::DenseFixedQueueTest::basic_test<Type>(1024 * 16, i_uint_to_type);
				details::DenseFixedQueueTest::basic_test<Type>(16, i_uint_to_type);
				details::DenseFixedQueueTest::basic_test<Type>(1, i_uint_to_type);
				details::DenseFixedQueueTest::basic_test<Type>(0, i_uint_to_type);
			}



		} // namespace DenseFixedQueueTest
	} // namespace details

	void dense_fixed_queue_test()
	{
		details::DenseFixedQueueTest::typed_basic_test( [](unsigned i_input)->unsigned {
			return i_input; } );

		details::DenseFixedQueueTest::typed_basic_test( [](unsigned i_input) {
			return static_cast<char>(i_input & 0xFF); } );

		details::DenseFixedQueueTest::typed_basic_test( [](unsigned i_input) {
			return static_cast<double>(i_input); } );

		details::DenseFixedQueueTest::typed_basic_test( [](unsigned i_input) { 
			return std::complex<double>(i_input); });

		details::DenseFixedQueueTest::typed_basic_test( [](unsigned i_input) { 
			switch (i_input % 6)
			{
				default:
				case 0: return TestString("0");
				case 1: return TestString("1");
				case 2: return TestString("2");
				case 3: return TestString("3");
				case 4: return TestString("4");
				case 5: return TestString("5");
			}
		});

	}

}