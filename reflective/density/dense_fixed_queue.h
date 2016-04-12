
#pragma once
#include <memory>
#include "element_type.h"

namespace reflective
{
	namespace details
	{
		template < typename ALLOCATOR, typename ELEMENT_TYPE >
			class DenseFixedQueueBase : private ALLOCATOR
		{

		public:

			struct IteratorBase
			{
				IteratorBase() {}

				IteratorBase(ELEMENT_TYPE * i_type) REFLECTIVE_NOEXCEPT // used to construct end
					: m_curr_type(i_type) { }

				IteratorBase(const DenseFixedQueueBase * i_queue, ELEMENT_TYPE * i_type, void * i_element ) REFLECTIVE_NOEXCEPT
					: m_curr_type(i_type), m_curr_element(i_element), m_queue(i_queue) { }

				void move_next() REFLECTIVE_NOEXCEPT
				{
					// advance m_curr_type
					auto const prev_type_ptr = m_curr_type;
					auto end_of_curr = address_add(m_curr_element, m_curr_type->size());
					m_curr_type = static_cast<ELEMENT_TYPE*>(address_upper_align(end_of_curr, alignof(ELEMENT_TYPE)));
					if (m_curr_type + 1 > m_queue->m_buffer_end)
					{
						m_curr_type = static_cast<ELEMENT_TYPE*>(address_upper_align(m_queue->m_buffer_start, alignof(ELEMENT_TYPE)));
						if (m_curr_type + 1 >= m_queue->m_head) // we use >=, because the condition head==tail is reserved for an empty queue
						{
							// stop
							m_curr_type = m_queue->m_tail;
						}
					}
					else
					{
						if( (prev_type_ptr >= m_queue->m_head) != (m_curr_type >= m_queue->m_head) )
						{
							// stop
							m_curr_type = m_queue->m_tail;
						}
					}

					if (m_curr_type != m_queue->m_tail)
					{
						// advance m_curr_element
						auto const prev_curr_element = m_curr_element;
						m_curr_element = address_upper_align(m_curr_type + 1, m_curr_type->alignment());
						auto end_of_element = address_add(m_curr_element, m_curr_type->size());
						if (end_of_element > m_queue->m_buffer_end)
						{
							m_curr_element = address_upper_align(m_queue->m_buffer_start, m_curr_type->alignment());
							end_of_element = address_add(m_curr_element, m_curr_type->size());
							if (end_of_element >= m_queue->m_head) // we use >=, because the condition head==tail is reserved for an empty queue
							{
								// stop
								m_curr_type = m_queue->m_tail;
							}
						}
						else
						{
							if ((prev_curr_element >= m_queue->m_head) != (m_curr_element >= m_queue->m_head))
							{
								// stop
								m_curr_type = m_queue->m_tail;
							}
						}
					}
				}

				bool operator == (const IteratorBase & i_source) REFLECTIVE_NOEXCEPT
				{
					return i_source.m_curr_type == i_source.m_curr_type;
				}

				ELEMENT_TYPE * m_curr_type;
				void * m_curr_element;
				const DenseFixedQueueBase * m_queue;
			};

			DenseFixedQueueBase(size_t i_buffer_byte_capacity)
			{
				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(
					*static_cast<ALLOCATOR*>(this));

				m_buffer_start = char_alloc.allocate(i_buffer_byte_capacity);
				m_buffer_end = address_add(m_buffer_start, i_buffer_byte_capacity);
				m_tail = m_head = static_cast<ELEMENT_TYPE *>(m_buffer_start);
			}

			DenseFixedQueueBase(DenseFixedQueueBase && i_source) REFLECTIVE_NOEXCEPT
				: m_head(i_source.m_head), m_tail(i_source.m_tail), m_buffer_start(i_source.m_buffer_start), m_buffer_end(i_source.m_buffer_end)
			{
				i_source.m_tail = i_source.m_head = nullptr;
				i_source.m_buffer_end = i_source.m_buffer_start = nullptr;
			}

			DenseFixedQueueBase & operator = (DenseFixedQueueBase && i_source) REFLECTIVE_NOEXCEPT
			{
				impl_clear();

				m_head = i_source.m_head;
				m_tail = i_source.m_tail;
				m_buffer_start = i_source.m_buffer_start;
				m_buffer_end = i_source.m_buffer_end;

				i_source.m_tail = i_source.m_head = nullptr;
				i_source.m_buffer_end = i_source.m_buffer_start = nullptr;

				return *this;
			}

			~DenseFixedQueueBase()
			{
				impl_clear();

				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(
					*static_cast<ALLOCATOR*>(this));
				
				char_alloc.deallocate(static_cast<char*>(m_buffer_start),
					address_diff(m_buffer_end, m_buffer_start) );
			}

			bool impl_empty() const REFLECTIVE_NOEXCEPT
			{
				return m_head == m_tail;
			}

			IteratorBase impl_begin() const REFLECTIVE_NOEXCEPT
			{
				if (m_head == m_tail)
				{
					return IteratorBase(m_tail);
				}
				else
				{
					auto type = static_cast<ELEMENT_TYPE*>(address_upper_align(m_head, alignof(ELEMENT_TYPE)));
					auto element = address_upper_align(type + 1, type->alignment());
					return IteratorBase(this, type, element);
				}
			}

			IteratorBase impl_end() const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase(m_tail);
			}
			
			struct CopyConstruct
			{
				const void * const m_source;

				CopyConstruct(const void * i_source)
					: m_source(i_source) { }

				void operator () (void * i_dest, const ELEMENT_TYPE & i_element_type)
				{
					i_element_type.copy_construct(i_dest, m_source);
				}
			};

			struct MoveConstruct
			{
				void * const m_source;

				MoveConstruct(void * i_source)
					: m_source(i_source) { }

				void operator () (void * i_dest, const ELEMENT_TYPE & i_element_type)
				{
					i_element_type.move_construct(i_dest, m_source);
				}
			};

			void * single_push(void * * io_tail, size_t i_size, size_t i_alignment)
			{
				auto const prev_tail = *io_tail;
				auto start_of_block = linear_alloc(io_tail, i_size, i_alignment);
				if (*io_tail > m_buffer_end)
				{
					// wrap to the start...
					*io_tail = m_buffer_start;
					start_of_block = linear_alloc(io_tail, i_size, i_alignment);
					if (*io_tail >= m_head)
					{
						// ...not enough space before the head, failed!
						start_of_block = nullptr;
						*io_tail = prev_tail;
					}
				}
				if ((prev_tail >= m_head) != (*io_tail >= m_head))
				{
					// ...crossed the head, failed!
					start_of_block = nullptr;
					*io_tail = prev_tail;
				}
				return start_of_block;
			}

			template <typename CONSTRUCTOR>
				bool impl_push(const ELEMENT_TYPE & i_source_type, CONSTRUCTOR && i_constructor)
			{
				auto tail = static_cast<void*>( m_tail );
				const auto type_block = single_push(&tail, sizeof(ELEMENT_TYPE), alignof(ELEMENT_TYPE) );
				const auto element_block = single_push(&tail, i_source_type.size(), i_source_type.alignment());
				if (element_block == nullptr || type_block == nullptr)
				{
					return false;
				}

				// commit the push
				new(type_block) ELEMENT_TYPE(i_source_type);
				i_constructor(element_block, i_source_type);
				m_tail = static_cast<ELEMENT_TYPE*>(tail);
				return true;
			}

			template <typename OPERATION>
				void impl_consume(OPERATION && i_operation)
			{
				assert(m_head != m_tail); // the queue must not be empty
				auto const element_alignment = m_head->alignment();
				auto const element_size = m_head->size();

				auto element = address_upper_align(m_head + 1, element_alignment);
				auto element_end = address_add(element, element_size);
				if (element_end > m_buffer_end)
				{
					element = address_upper_align(m_buffer_start, element_alignment);
					element_end = address_add(element, element_size);
					assert(element_end <= m_buffer_end);
				}

				i_operation(*m_head, element);

				m_head = static_cast<ELEMENT_TYPE*>(address_upper_align(element_end, std::alignment_of<ELEMENT_TYPE>::value));
				if (m_head + 1 > m_buffer_end)
				{
					m_head = static_cast<ELEMENT_TYPE*>(address_upper_align(m_buffer_start, std::alignment_of<ELEMENT_TYPE>::value));
					assert(m_head <= m_buffer_end);
				}
			}

		private:
			
			void impl_clear() REFLECTIVE_NOEXCEPT
			{
				IteratorBase it = impl_begin();
				while (it.m_curr_type != m_tail)
				{
					auto const type = it.m_curr_type;
					auto const element = it.m_curr_element;
					it.move_next();

					type->destroy(element);
					type->ELEMENT_TYPE::~ELEMENT_TYPE();
				}
				m_head = m_tail;
			}

		private:
			ELEMENT_TYPE * m_head;
			ELEMENT_TYPE * m_tail;
			void * m_buffer_start;
			void * m_buffer_end;
		}; // class DenseFixedQueueBase

	} // namespace details

	template <typename ELEMENT = void, typename ALLOCATOR = std::allocator<ELEMENT>, typename ELEMENT_TYPE = ElementType<ELEMENT> >
		class DenseFixedQueue : private details::DenseFixedQueueBase<ALLOCATOR, ELEMENT_TYPE>
	{
		using BaseClass = details::DenseFixedQueueBase<ALLOCATOR, ELEMENT_TYPE>;

	public:

		using ElementType = ELEMENT_TYPE;
		using allocator_type = ALLOCATOR;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using difference_type = ptrdiff_t;
		using size_type = size_t;
		class iterator;
		class const_iterator;

		DenseFixedQueue(size_t i_buffer_byte_capacity)
			: BaseClass(i_buffer_byte_capacity) { }

		class iterator : private IteratorBase
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = ptrdiff_t;
			using size_type = size_t;
			using value_type = ELEMENT;
			using reference = ELEMENT &;
			using const_reference = const ELEMENT &;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

			iterator(const IteratorBase & i_source) REFLECTIVE_NOEXCEPT
				: IteratorBase(i_source) {  }

			value_type & operator * () const REFLECTIVE_NOEXCEPT { return *static_cast<value_type *>(IteratorBase::m_curr_element); }
			value_type * operator -> () const REFLECTIVE_NOEXCEPT { return static_cast<value_type *>(IteratorBase::m_curr_element); }
			value_type * curr_element() const REFLECTIVE_NOEXCEPT { return static_cast<value_type *>(IteratorBase::m_curr_element); }

			iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				IteratorBase::move_next();
				return *this;
			}

			iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				const iterator copy(*this);
				IteratorBase::move_next();
				return copy;
			}

			bool operator == (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type == i_other.curr_type();
			}

			bool operator != (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type != i_other.curr_type();
			}

			bool operator == (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type == i_other.curr_type();
			}

			bool operator != (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type != i_other.curr_type();
			}

			const ELEMENT_TYPE * curr_type() const REFLECTIVE_NOEXCEPT { return IteratorBase::m_curr_type; }

		}; // class iterator

		class const_iterator final : private IteratorBase
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = ptrdiff_t;
			using size_type = size_t;
			using value_type = const ELEMENT;
			using reference = const ELEMENT &;
			using const_reference = const ELEMENT &;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

			const_iterator(const IteratorBase & i_source) REFLECTIVE_NOEXCEPT
				: IteratorBase(i_source) {  }

			const_iterator(const iterator & i_source) REFLECTIVE_NOEXCEPT
				: IteratorBase(i_source) {  }

			value_type & operator * () const REFLECTIVE_NOEXCEPT { return *static_cast<value_type *>(IteratorBase::m_curr_element); }
			value_type * operator -> () const REFLECTIVE_NOEXCEPT { return static_cast<value_type *>(IteratorBase::m_curr_element); }
			value_type * curr_element() const REFLECTIVE_NOEXCEPT { return static_cast<value_type *>(IteratorBase::m_curr_element); }

			const_iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				IteratorBase::move_next();
				return *this;
			}

			const_iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				const iterator copy(*this);
				IteratorBase::move_next();
				return copy;
			}

			bool operator == (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type == i_other.curr_type();
			}

			bool operator != (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type != i_other.curr_type();
			}

			bool operator == (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type == i_other.curr_type();
			}

			bool operator != (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return IteratorBase::m_curr_type != i_other.curr_type();
			}

			const ELEMENT_TYPE * curr_type() const REFLECTIVE_NOEXCEPT { return IteratorBase::m_curr_type; }

			friend class DenseList;

		}; // class const_iterator

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(BaseClass::impl_begin()); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(BaseClass::impl_end()); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::impl_begin()); }
		const_iterator end() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::impl_end()); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::impl_begin()); }
		const_iterator cend() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::impl_end()); }

		bool empty() const REFLECTIVE_NOEXCEPT { return BaseClass::impl_empty(); }

		void clear() REFLECTIVE_NOEXCEPT { BaseClass::impl_clear(); }

		template <typename ELEMENT_COMPLETE_TYPE>
			bool try_push(const ELEMENT_COMPLETE_TYPE & i_source)
				// REFLECTIVE_NOEXCEPT_V()
		{
			return BaseClass::impl_push(ElementType::template make<ELEMENT_COMPLETE_TYPE>(), CopyConstruct(&i_source));
		}

		template <typename OPERATION>
			void consume(OPERATION && i_operation)
		{
			BaseClass::impl_consume([&i_operation](const ELEMENT_TYPE & i_type, void * i_element) {
				i_operation(i_type, *static_cast<ELEMENT*>(i_element));
			});
		}

		void pop()
		{
			BaseClass::impl_consume_front([](const ELEMENT_TYPE &, void *) {});
		}

		const ELEMENT & front()
		{
			assert(!empty());
			const auto it = BaseClass::impl_begin();
			return *static_cast<value_type *>(it.m_curr_element);
		}

	}; // class DenseFixedQueue
}