
#include "..\dense_list.h"
#include "..\testing_utils.h"
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>

namespace reflective
{
	namespace
	{
		class TestObjectBase
		{
		public:
			virtual ~TestObjectBase(){}

			void non_virtual()
			{
				a = 0;
			}

			volatile int a, b, c, d;
		};

		class TestObjectDerived : public TestObjectBase
		{		
		public:
			volatile int a, b, c, d;
		};

		class TestContainerVector
		{
		public:

			TestContainerVector()
			{
				m_vector.reserve(24*2);
				for (size_t i = 0; i < m_vector.size(); i++)
				{
					m_vector.push_back(std::make_unique<TestObjectDerived>());
				}
			}

			std::vector<std::unique_ptr<TestObjectBase>> m_vector;
		};

		class TestContainerList
		{
		public:

			using List = DenseList < TestObjectBase, std::allocator<TestObjectBase>, UnmovableTypeInfo<TestObjectBase> >;

			TestContainerList()
			{
				m_list = List::make(
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(),
					TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived(), TestObjectDerived());
			}

			List m_list;
		};		


		void memory_stress()
		{
			const size_t size = 10000;
			volatile int * ints = new int[size];
			for (size_t i = 0; i < size; i++)
			{
				ints[i] = 40;
			}
			for (size_t i = 0; i < size; i++)
			{
				ints[i] += 4;
			}
			delete[] ints;
		}
	}

	void dense_list_benchmark()
	{
		using namespace std;
		using namespace std::chrono;

		const size_t count = 1000000;
		vector<TestContainerList> lists(count);
		vector<TestContainerVector> vectors(count);

		memory_stress();
		{
			cout << "list..." << endl;
			const auto time_before = high_resolution_clock::now();
			for (auto & container : lists)
			{
				auto end_it = container.m_list.end();
				for (auto it = container.m_list.begin(); it != end_it; it++)
				{
					it->non_virtual();
				}
			}
			auto spent = duration_cast<nanoseconds>(high_resolution_clock::now() - time_before);
			cout << spent.count() << endl;
		}

		memory_stress();
		{
			cout << "vectors..." << endl;
			const auto time_before = chrono::high_resolution_clock::now();
			for (auto & container : vectors)
			{
				auto end_it = container.m_vector.end();
				for (auto it = container.m_vector.begin(); it != end_it; it++)
				{
					(*it)->non_virtual();
				}
			}
			auto spent = duration_cast<nanoseconds>(high_resolution_clock::now() - time_before);
			cout << spent.count() << endl;
		}
	}
}
