
#pragma once
#include "density_common.h"

namespace reflective
{
	class TestAllocatorBase
	{
	public:

		class NoLeakScope
		{
		public:
			NoLeakScope()
			{
				REFLECTIVE_TEST_ASSERT(GetMap().size() == 0);
			}

			~NoLeakScope()
			{
				REFLECTIVE_TEST_ASSERT(GetMap().size() == 0);
			}

			NoLeakScope(const NoLeakScope &) = delete;
			NoLeakScope & operator = (const NoLeakScope &) = delete;
		};

		void * alloc(size_t i_size)
		{
			void * block = operator new (i_size);
			AllocationEntry entry;
			entry.m_size = i_size;
			auto res = GetMap().insert(std::make_pair(block, entry));
			REFLECTIVE_INTERNAL_ASSERT(res.second);
			return block;
		}

		void free(void * i_block)
		{
			auto & map = GetMap();
			auto it = map.find(i_block);
			REFLECTIVE_INTERNAL_ASSERT(it != map.end());
			map.erase(it);
			operator delete (i_block);
		}

	private:

		struct AllocationEntry
		{
			size_t m_size;
		};

		static std::unordered_map<void*, AllocationEntry> & GetMap()
		{
			#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
				if (t_map == nullptr)
				{
					t_map = new std::unordered_map<void*, AllocationEntry>;
				}
				return *t_map;
			#else
				return t_map;
			#endif
		}

	private:

		#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
			static _declspec(thread) std::unordered_map<void*, AllocationEntry> * t_map;
		#else
			static thread_local std::unordered_map<void*, AllocationEntry> t_map;
		#endif
	};

	#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
		_declspec(thread) std::unordered_map<void*, TestAllocatorBase::AllocationEntry> * TestAllocatorBase::t_map;
	#else
		thread_local std::unordered_map<void*, TestAllocatorBase::AllocationEntry> TestAllocatorBase::t_map;
	#endif

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

	class ExceptionStressTestObject
	{
		std::vector<int, TestAllocator<int> > m_vector;
	};

} //namespace reflective