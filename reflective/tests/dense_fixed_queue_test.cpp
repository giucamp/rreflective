
#include "..\density\dense_fixed_queue.h"
#include "..\unit_testing\testing_utils.h"
#include <vector>
#include <list>
#include <iostream>
#include "..\reflective_settings.h"

namespace reflective
{
	namespace details
	{
		namespace DenseFixedQueueTest
		{
			void test1()
			{
				DenseFixedQueue<int> queue(1024 * 16);
				queue.try_push_back(1);
				queue.try_push_back(2);
				queue.try_push_back(3);
				queue.try_push_back(4);
				for (auto & i : queue)
				{
					std::cout << i << std::endl;
				}

				std::cout << "---" << std::endl;
				while (!queue.empty())
				{
					queue.consume_front([](DenseFixedQueue<int>::ElementType, int i) {
						std::cout << i << std::endl;
					});
				}
			}
		}
	}

	void dense_fixed_queue_test()
	{
		details::DenseFixedQueueTest::test1();
	}

}