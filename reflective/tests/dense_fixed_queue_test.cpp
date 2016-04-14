
#include "..\density\dense_fixed_queue.h"
#include "..\unit_testing\testing_utils.h"
#include <deque>
#include <iostream>
#include "..\reflective_settings.h"

namespace reflective
{
	namespace details
	{
		namespace DenseFixedQueueTest
		{
			void test1(size_t i_size)
			{
				NoLeakScope no_leak_scope;

				DenseFixedQueue<int, TestAllocator<int> > queue(i_size);
				std::deque<int> vector;

				for (int progr = 0; progr < 1000; progr++)
				{
					if ( (progr % 4) == 0 )
					{
						std::cout << "* Push until full" << std::endl;
						int i = 0;
						while (queue.try_push(i))
						{
							vector.push_back(i);
							i++;
						}
					}
					else if ((progr % 3) == 0)
					{
						std::cout << "* Push " << progr << " times" << std::endl;
						int i = 0;
						while (i < progr && queue.try_push(i))
						{
							vector.push_back(i);
							i++;
						}
					}
					else if ((progr % 2) == 0)
					{
						std::cout << "* Pop " << progr << " times" << std::endl;
						int i = 0;
						while (!queue.empty() && i < progr)
						{
							queue.consume([&vector](DenseFixedQueue<int>::ElementType, int val) {
								REFLECTIVE_TEST_ASSERT(vector.size() > 0 && val == vector.front());
								vector.pop_front();
							});
							i++;
						}
					}
					else
					{
						std::cout << "* Pop until empty" << std::endl;
						int i = 0;
						while (!queue.empty())
						{
							queue.consume([&vector](DenseFixedQueue<int>::ElementType, int val) {
								REFLECTIVE_TEST_ASSERT(vector.size() > 0 && val == vector.front());
								vector.pop_front();
							});
							i++;
						}
						REFLECTIVE_TEST_ASSERT(vector.empty());
					}

					// check for equality queue and vector
					auto mem_capacity = queue.mem_capacity();
					auto mem_size = queue.mem_size();
					std::cout << "used: " << mem_size << ", els: " << vector.size() << ", free: " << mem_capacity - mem_size << std::endl;
					REFLECTIVE_TEST_ASSERT(queue.empty() == vector.empty());
					if (!vector.empty())
					{
						REFLECTIVE_TEST_ASSERT(queue.front() == vector.front());
					}
					auto it1 = queue.cbegin();
					auto it2 = vector.cbegin();
					int u = 0;
					for (;;)
					{
						bool end1 = it1 == queue.cend();
						bool end2 = it2 == vector.cend();
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
			}
		}
	}

	void dense_fixed_queue_test()
	{
		details::DenseFixedQueueTest::test1(1024 * 16 + 1);
		details::DenseFixedQueueTest::test1(1024 * 16);
		details::DenseFixedQueueTest::test1(16);
		details::DenseFixedQueueTest::test1(1);
		details::DenseFixedQueueTest::test1(0);
	}

}