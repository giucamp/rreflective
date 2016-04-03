
#include "..\testing_utils.h"
#include <random>

namespace reflective
{
	#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
		_declspec(thread) TestAllocatorBase::ThreadData * TestAllocatorBase::st_thread_data;
	#else
		thread_local TestAllocatorBase::ThreadData TestAllocatorBase::st_thread_data;
	#endif

	TestAllocatorBase::NoLeakScope::NoLeakScope()
	{
		GetThreadData().push_level();
	}

	TestAllocatorBase::NoLeakScope::~NoLeakScope()
	{
		GetThreadData().pop_level();
	}

	TestAllocatorBase::ThreadData & TestAllocatorBase::GetThreadData()
	{
		#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
			if (st_thread_data == nullptr)
			{
				st_thread_data = new ThreadData;
			}
			return *st_thread_data;
		#else
			return st_thread_data;
		#endif
	}

	TestAllocatorBase::TestAllocatorBase()
	{
	}

	TestAllocatorBase::~TestAllocatorBase()
	{
	}

	void TestAllocatorBase::ThreadData::push_level()
	{
		m_levels.emplace_back();
	}

	void TestAllocatorBase::ThreadData::pop_level()
	{
		REFLECTIVE_INTERNAL_ASSERT(m_levels.size() > 0);
		REFLECTIVE_INTERNAL_ASSERT(m_levels.back().m_allocations.size() == 0);
		m_levels.pop_back();
	}

	void * TestAllocatorBase::alloc(size_t i_size)
	{
		void * block = operator new (i_size);

		auto & thread_data = GetThreadData();
		if (thread_data.m_levels.size() > 0)
		{
			AllocationEntry entry;
			entry.m_size = i_size;

			auto & allocations = thread_data.m_levels.back().m_allocations;
			auto res = allocations.insert(std::make_pair(block, entry));
			REFLECTIVE_INTERNAL_ASSERT(res.second);
		}

		return block;
	}

	void TestAllocatorBase::free(void * i_block)
	{
		auto & thread_data = GetThreadData();
		if (thread_data.m_levels.size() > 0)
		{
			auto & allocations = thread_data.m_levels.back().m_allocations;
			auto it = allocations.find(i_block);
			REFLECTIVE_INTERNAL_ASSERT(it != allocations.end());
			allocations.erase(it);
		}

		operator delete (i_block);
	}

	namespace except_stress
	{
		namespace
		{
			struct StaticData
			{
				int64_t m_current_counter;
				int64_t m_except_at;
				StaticData() : m_current_counter(0), m_except_at(-1) {}
			};

			class TestException
			{
			};

			thread_local StaticData  * st_static_data;
		}

		void check_point()
		{
			auto const static_data = st_static_data;
			if (static_data != nullptr)
			{
				if (static_data->m_current_counter == static_data->m_except_at)
				{
					throw TestException();
				}
				static_data->m_current_counter++;
			}
		}

		std::random_device g_random_device;
		std::mt19937 g_rand(g_random_device());

		AllocatingTester::AllocatingTester()
			: m_rand_value(std::uniform_int_distribution<int>(100000)(g_rand)),
			  m_vector(10, -1)
		{
		}

		void run_test(std::function<void()> i_test)
		{
			REFLECTIVE_RUNTIME_CHECK(st_static_data == nullptr, __func__ " does no support recursion");

			i_test();

			StaticData static_data;
			st_static_data = &static_data;
			try
			{
				int64_t curr_iteration = 0;
				bool exception_occurred;
				do {
					exception_occurred = false;

					TestAllocatorBase::NoLeakScope no_leak_scope;
					try
					{
						static_data.m_current_counter = 0;
						static_data.m_except_at = curr_iteration;
						i_test();
					}
					catch (TestException)
					{
						exception_occurred = true;
					}
					curr_iteration++;

				} while (exception_occurred);
			}
			catch (...)
			{
				st_static_data = nullptr; // unknown exception, reset st_static_data and retrhow
				throw;
			}
			st_static_data = nullptr;
		}

	} // namespace except_stress

} // namespace reflective
