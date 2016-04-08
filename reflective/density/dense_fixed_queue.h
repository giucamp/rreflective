
#pragma once
#include "element_type.h"

namespace reflective
{
	template <typename ELEMENT, typename ALLOCATOR, typename TYPE_WRAPPER >
		class DenseFixedQueue : private ALLOCATOR
	{
		
	public:

		using TypeWrapper = TYPE_WRAPPER;

		using allocator_type = ALLOCATOR;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		DenseFixedQueue(size_t i_buffer_byte_capacity);

		template <typename ELEMENT_COMPLETE_TYPE>
			void push_back(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			void * end = m_end;
			for (;;)
			{
				TypeWrapper * type = static_cast<TypeWrapper *>(end);
				
				address_upper_align(type + 1);
			}
		}

	private:
		void * m_begin;
		void * m_end;
		void * m_buffer_start;
		void * m_buffer_end;
	};
}