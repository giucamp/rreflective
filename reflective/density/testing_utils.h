
#pragma once
#include "density_common.h"
#include <unordered_map>
#include <functional>

namespace reflective
{
	class TestAllocatorBase
	{
	public:

		TestAllocatorBase();
		~TestAllocatorBase();

		class NoLeakScope
		{
		public:
			NoLeakScope();
			~NoLeakScope();
			NoLeakScope(const NoLeakScope &) = delete;
			NoLeakScope & operator = (const NoLeakScope &) = delete;
		};

	protected:

		void * alloc(size_t i_size);

		void free(void * i_block);

	private:
		struct AllocationEntry
		{
			size_t m_size;
		};
		struct Levels
		{
			std::unordered_map<void*, AllocationEntry> m_allocations;
		};
		struct ThreadData
		{
			std::vector<Levels> m_levels;

			void push_level();

			void pop_level();
		};
		static ThreadData & GetThreadData();

	private:
		#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
			static _declspec(thread) ThreadData * st_thread_data;
		#else
			static thread_local ThreadData st_thread_data;
		#endif
	};

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

		#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
					
			template<class Other> struct rebind { typedef TestAllocator<Other> other; };

			void construct(TYPE * i_pointer)
			{	
				new (i_pointer) TYPE();
			}

			void construct(TYPE * i_pointer, const TYPE & i_source)
			{
				new (i_pointer) TYPE(i_source);
			}

			template<class OTHER_TYPE, class... ARG_TYPES>
				void construct(OTHER_TYPE * i_pointer, ARG_TYPES &&... i_args)
			{
				new (i_pointer)OTHER_TYPE(std::forward<ARG_TYPES>(i_args)...);
			}

			void destroy(TYPE * i_pointer)
			{
				i_pointer->~TYPE();
				(void)i_pointer; // avoid warning C4100: 'i_pointer' : unreferenced formal parameter
			}

		#endif
	};
	
	namespace except_stress
	{
		void run_test(std::function<void()> i_test);

		void check_point();

		class AllocatingTester
		{
		public:

			AllocatingTester();

			bool operator == (const AllocatingTester & i_other) const
				{ return m_rand_value == i_other.m_rand_value; }
			
			bool operator != (const AllocatingTester & i_other) const
				{ return m_rand_value != i_other.m_rand_value; }

		private:
			std::vector<int, TestAllocator<int> > m_vector;
			int m_rand_value;
		};

		class Copy_MoveExcept final : private AllocatingTester
		{
		public:

			Copy_MoveExcept() { check_point(); }

			// copy
			Copy_MoveExcept(const Copy_MoveExcept & i_source)
				: AllocatingTester((check_point(), i_source))
			{

			}
			Copy_MoveExcept & operator = (const Copy_MoveExcept & i_source)
			{
				check_point();
				AllocatingTester::operator = (i_source);
				check_point();
				return *this;
			}

			// move (except)
			Copy_MoveExcept(Copy_MoveExcept && i_source)
				: AllocatingTester((check_point(), std::move(i_source)))
			{
				check_point();
			}
			Copy_MoveExcept & operator = (Copy_MoveExcept && i_source)
			{
				check_point();
				AllocatingTester::operator = (std::move(i_source));
				check_point();
				return *this;
			}

			bool operator == (const Copy_MoveExcept & i_other) const
				{ return AllocatingTester::operator == (i_other); }

			bool operator != (const Copy_MoveExcept & i_other) const
				{ return AllocatingTester::operator != (i_other); }
		};

		class NoCopy_MoveExcept final : private AllocatingTester
		{
		public:

			NoCopy_MoveExcept() { check_point(); }
			
			// copy
			NoCopy_MoveExcept(const NoCopy_MoveExcept & i_source) = delete;
			NoCopy_MoveExcept & operator = (const NoCopy_MoveExcept & i_source) = delete;

			// move (except)
			NoCopy_MoveExcept(NoCopy_MoveExcept && i_source)
				: AllocatingTester((check_point(), std::move(i_source)) )
					{ check_point(); }
			NoCopy_MoveExcept & operator = (NoCopy_MoveExcept && i_source)
			{
				check_point();
				AllocatingTester::operator = (std::move(i_source));
				check_point();
				return *this;
			}

			bool operator == (const NoCopy_MoveExcept & i_other) const
				{ return AllocatingTester::operator == (i_other); }

			bool operator != (const NoCopy_MoveExcept & i_other) const
				{ return AllocatingTester::operator != (i_other); }
		};

	} // namespace except_stress

} // namespace reflective
