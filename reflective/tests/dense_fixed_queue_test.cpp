
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
				DenseFixedQueue<int> queue(i_size);
				std::deque<int> vector;

				for (int progr = 0; progr < 1000; progr++)
				{
					if (progr % 11)
					{
						int i = 0;
						while (queue.try_push(i))
						{
							vector.push_back(i);
							i++;
						}
					}

					// check for equality queue and vector
					REFLECTIVE_TEST_ASSERT(queue.empty() == vector.empty());
					if (!vector.empty())
					{
						REFLECTIVE_TEST_ASSERT(queue.front() == vector.front());
					}
					auto it1 = queue.cbegin();
					auto it2 = vector.cbegin();
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
					}
				}

				queue.try_push(1);
				queue.try_push(2);
				queue.try_push(3);
				queue.try_push(4);
				for (auto & i : queue)
				{
					std::cout << i << std::endl;
				}

				std::cout << "---" << std::endl;
				while (!queue.empty())
				{
					queue.consume([](DenseFixedQueue<int>::ElementType, int i) {
						std::cout << i << std::endl;
					});
				}
			}
		}
	}

	void dense_fixed_queue_test()
	{
		details::DenseFixedQueueTest::test1(1024 * 16);
	}

}