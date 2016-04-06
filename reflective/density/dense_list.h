
#pragma once
#include <memory>
#include "dense_type_wrapper.h"
//#include <algorithm> // std::max<size_t> would be needed, but details::size_max is defined to avoid this dependency

#ifdef NDEBUG
	#define REFLECTIVE_DENSE_LIST_DEBUG		0
#else
	#define REFLECTIVE_DENSE_LIST_DEBUG		1
#endif

namespace reflective
{
	namespace details
	{
		// size_max: avoid incuding <algorithm> just to use std::max<size_t>
		REFLECTIVE_CONSTEXPR inline size_t size_max(size_t i_first, size_t i_second) REFLECTIVE_NOEXCEPT
		{
			return i_first > i_second ? i_first : i_second;
		}
	}

	/** A dense-list is a polymorphic sequence container optimized to be compact in both heap memory and inline storage.
		Elements is a DenseList are allocated respecting their alignment requirements.
		In a polymorphic container every element can have a different complete type, provided that this type is covariant to the type ELEMENT.
		All the elements of a DenseList are arranged in the same memory block of the heap. 
		Insertions\removals of a non-zero number of elements and clear() always reallocate the memory blocks and invalidate existing iterators.
		The inline storage of DenseList is the same of a pointer. An empty DenseList does not use heap memory.
		All the functions of DenseList gives at least the strong exception guarantee. */
	template <typename ELEMENT, typename ALLOCATOR, typename ELEMENT_TYPE >
		class DenseList : private ALLOCATOR
	{
		enum InternalConstructor { InternalConstructorMem };

	public:

		/** Alias for the template argument ELEMENT_TYPE */
		using ElementType = ELEMENT_TYPE;
		using allocator_type = ALLOCATOR;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		/** Creates a DenseList containing all the elements specified in the parameter list. 
			For each object of the parameter pack, an element is added to the list by copy-construction or move-construction.
				@param i_args elements to add to the list. 
				@return the new DenseList
			Example: 
				const auto list = DenseList<int>::make(1, 2, 3);
				const auto list1 = DenseList<BaseClass>::make(Derived1(), Derived2(), Derived1()); */
		template <typename... TYPES>
			inline static DenseList make(TYPES &&... i_args)
		{
			DenseList new_list;
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		/** Creates a DenseList containing all the elements specified in the parameter list. The allocator of the new DenseList is copy-constructed from the provided one.
			For each object of the parameter pack, an element is added to the list by copy-construction or move-construction.
				@param i_args elements to add to the list. 
				@return the new DenseList 
			Example:
				MyAlloc<int> my_alloc;
				MyAlloc<BaseClass> my_alloc1;
				const auto list = DenseList<int>::make_with_alloc(my_alloc, 1, 2, 3);
				const auto list1 = DenseList<BaseClass>::make_with_alloc(my_alloc1, Derived1(), Derived2(), Derived1()); */
		template <typename... TYPES>
			inline static DenseList make_with_alloc(const ALLOCATOR & i_allocator, TYPES &&... i_args)
		{
			DenseList new_list;
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		/** Constructs an empty list. This function never throws. */
		DenseList() REFLECTIVE_NOEXCEPT
			: m_buffer(nullptr)
				{ }

		/** Move-construct a list, leaving the source list empty. This function never throws. */
		DenseList(DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			move_impl(std::move(i_source));
		}

		/** Move-assigns a list, leaving the source list empty. This function never throws. */
		DenseList & operator = (DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			move_impl(std::move(i_source));
			return *this;
		}

		/** Copy-constructs a list. This function gives the strong exception guarantee. */
		DenseList(const DenseList & i_source)
		{
			copy_impl(i_source);
		}

		/** Copy-assigns a list. This function gives the strong exception guarantee. */
		DenseList & operator = (const DenseList & i_source)
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			copy_impl(i_source);
			return *this;
		}

		/** Destroys a list. This function never throws. */
		~DenseList() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
		}

		/** Returns the number of elements in the list. To check if a list is empty, the function empty() is faster. This function never throws. */
		size_t size() const REFLECTIVE_NOEXCEPT 
		{
			if (m_buffer != nullptr)
			{
				Header * const header = reinterpret_cast<Header*>(m_buffer) - 1;
				return header->m_count;
			}
			else
			{
				return 0;
			}
		}

		/** Returns wheter a list is empty. Note: This function is equivalent to size() != 0, but is faster. This function never throws. */
		bool empty() const REFLECTIVE_NOEXCEPT
		{
			return m_buffer == nullptr; 
		}

		class iterator;
		class const_iterator;

		class iterator
		{
		public:

			using iterator_category = std::forward_iterator_tag;
			using value_type = ELEMENT;
			using reference = ELEMENT &;
			using const_reference = const ELEMENT &;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
			using difference_type = ptrdiff_t;
			using size_type = size_t;

			using VoidPtr = void *;

			iterator() REFLECTIVE_NOEXCEPT
				: m_curr_element(nullptr), m_curr_type(nullptr) { }

			iterator(InternalConstructor, VoidPtr i_curr_element, const ElementType * i_curr_type) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_curr_element), m_curr_type(i_curr_type)
			{
			}

			iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				VoidPtr const prev_element = curr_element();
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				m_curr_element = reinterpret_cast<value_type*>( reinterpret_cast<uintptr_t>(prev_element) + curr_element_size );
				return *this;
			}

			iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				iterator copy(*this);
				operator ++ ();
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
				return m_curr_type == i_other.curr_type();
			}

			bool operator != (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type != i_other.curr_type();
			}

			value_type & operator * () const	REFLECTIVE_NOEXCEPT { return *curr_element(); }
			value_type * operator -> () const	REFLECTIVE_NOEXCEPT { return curr_element(); }

			value_type * curr_element() const REFLECTIVE_NOEXCEPT 
			{ 
				auto const curr_element_alignment = m_curr_type->alignment();
				assert(is_power_of_2(curr_element_alignment));
				return reinterpret_cast<value_type*>(
					(reinterpret_cast<uintptr_t>(m_curr_element) + (curr_element_alignment - 1)) & ~(curr_element_alignment - 1) );
			}

			const ElementType * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:
			VoidPtr m_curr_element;
			const ElementType * m_curr_type;
			friend class DenseList;
		};

		class const_iterator
		{
		public:

			using iterator_category = std::forward_iterator_tag;
			using value_type = const ELEMENT;
			using reference = const ELEMENT &;
			using const_reference = const ELEMENT &;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
			using difference_type = ptrdiff_t;
			using size_type = size_t;

			using VoidPtr = const void *;

			const_iterator() REFLECTIVE_NOEXCEPT
				: m_curr_element(nullptr), m_curr_type(nullptr) { }

			const_iterator(const iterator & i_iterator) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_iterator.m_curr_element), m_curr_type(i_iterator.m_curr_type)
			{
			}

			const_iterator(InternalConstructor, VoidPtr i_curr_element, const ElementType * i_curr_type) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_curr_element), m_curr_type(i_curr_type)
			{
			}

			const_iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				VoidPtr const prev_element = curr_element();
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				m_curr_element = reinterpret_cast<value_type*>( reinterpret_cast<uintptr_t>(prev_element) + curr_element_size );
				return *this;
			}

			const_iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				const_iterator copy(*this);
				operator ++ ();
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
				return m_curr_type == i_other.curr_type();
			}

			bool operator != (const const_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type != i_other.curr_type();
			}

			value_type & operator * () const	REFLECTIVE_NOEXCEPT { return *curr_element(); }
			value_type * operator -> () const	REFLECTIVE_NOEXCEPT { return curr_element(); }

			value_type * curr_element() const REFLECTIVE_NOEXCEPT 
			{ 
				auto const curr_element_alignment = m_curr_type->alignment();
				assert(is_power_of_2(curr_element_alignment));
				return reinterpret_cast<value_type*>(
					(reinterpret_cast<uintptr_t>(m_curr_element) + (curr_element_alignment - 1)) & ~(curr_element_alignment - 1) );
			}

			const ElementType * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:
			VoidPtr m_curr_element;
			const ElementType * m_curr_type;
			friend class DenseList;
		};

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		iterator end() REFLECTIVE_NOEXCEPT{ return iterator(InternalConstructorMem, nullptr, m_buffer + size()); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		const_iterator end() const REFLECTIVE_NOEXCEPT{ return const_iterator(InternalConstructorMem, nullptr, m_buffer + size()); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		const_iterator cend() const REFLECTIVE_NOEXCEPT{ return const_iterator(InternalConstructorMem, nullptr, m_buffer + size()); }

		bool operator == (const DenseList & i_source) const
		{
			// this in visual studio generates a warning, because the iterators are not "checked" // return (size() == i_source.size()) && std::equal(cbegin(), cend(), i_source.cbegin());
			if (size() != i_source.size())
			{
				return false;
			}
			else
			{
				const auto end_1 = cend();
				for (auto it_1 = cbegin(), it_2 = i_source.cbegin(); it_1 != end_1; ++it_1, ++it_2)
				{
					if (*it_1 != *it_2)
					{
						return false;
					}
				}
				return true;
			}
		}

		bool operator != (const DenseList & i_source) const		{ return !operator == (i_source); }
		
		template <typename ELEMENT_COMPLETE_TYPE>
			iterator push_back(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(m_buffer + size(), 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator push_front(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(m_buffer, 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		iterator pop_front()
		{
			return erase_impl(m_buffer, m_buffer + 1);
		}

		iterator pop_back()
		{
			auto const end_type = m_buffer + get_size_not_empty();
			return erase_impl(end_type - 1, end_type);
		}
		
		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(i_position.m_curr_type, 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, size_t i_count, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			if (i_count > 0)
			{
				return insert_n_impl(i_position.m_curr_type, i_count, ElementType::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
			}
			else
			{
				// inserting 0 elements
				return iterator(InternalConstructorMem, const_cast<void*>(i_position.m_curr_element), i_position.m_curr_type);
			}			
		}

		iterator erase(const_iterator i_position)
		{
			return erase_impl(i_position.m_curr_type, i_position.m_curr_type + 1);
		}

		iterator erase(const_iterator i_from, const_iterator i_to)
		{
			if (i_from.m_curr_type != i_to.m_curr_type)
			{
				return erase_impl(i_from.m_curr_type, i_to.m_curr_type);
			}
			else
			{
				// removing 0 elements
				return iterator(InternalConstructorMem, const_cast<void*>(i_from.m_curr_element), i_from.m_curr_type);
			}
		}
		
		void clear()
		{
			destroy_impl();
			m_buffer = nullptr;
		}

	private:

		ELEMENT * get_elements() const // this function gives a non-const element from a const container, but this avoids duplicating the function
		{
			if (m_buffer != nullptr)
			{
				assert(is_power_of_2(m_buffer[0].alignment()));
				const auto first_element_align_mask = m_buffer[0].alignment() - 1;
				auto ptr = reinterpret_cast<uintptr_t>(m_buffer + get_size_not_empty());
				ptr = (ptr + first_element_align_mask) & ~first_element_align_mask;
				return reinterpret_cast<ELEMENT *>(ptr);
			}
			else
			{
				return nullptr;
			}
		}

		size_t get_size_not_empty() const REFLECTIVE_NOEXCEPT
		{
			if (m_buffer != nullptr)
			{
				return (reinterpret_cast<Header*>(m_buffer)-1)->m_count;
			}
			else
			{
				return 0;
			}
		}

		struct Header
		{
			size_t m_count;
		};
		
		struct ListBuilder
		{
			ListBuilder() REFLECTIVE_NOEXCEPT
				: m_element_infos(nullptr)
			{
			}

			void init(ALLOCATOR & i_allocator, size_t i_count, size_t i_buffer_size, size_t i_buffer_alignment)
			{
				void * const memory_block = aligned_alloc(i_allocator, i_buffer_size + sizeof(Header), i_buffer_alignment, sizeof(Header));
				Header * header = static_cast<Header*>(memory_block);
				header->m_count = i_count;
				m_end_of_types = m_element_infos = reinterpret_cast<ElementType*>(header + 1);
				m_end_of_elements = m_elements = m_element_infos + i_count;

				#if REFLECTIVE_DENSE_LIST_DEBUG
					m_dbg_end_of_buffer = address_add(m_element_infos, i_buffer_size);
				#endif
			}

			/* Adds a (type-info, element) pair to the list. The new element is copy-constructed. 
				Note: ELEMENT is not the comlete type of the element, as the
				list allows polymorphic types. The use of the ElementType avoid slicing or partial constructions. */
			void * add_by_copy(const ElementType & i_element_info, const ELEMENT & i_source)
					// REFLECTIVE_NOEXCEPT_V(std::declval<ElementType>().copy_construct(nullptr, std::declval<ELEMENT>() ))
			{
				// copy-construct the element first (this may throw)
				void * new_element = address_upper_align(m_end_of_elements, i_element_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(new_element, address_add(new_element, i_element_info.size()));
				#endif
				i_element_info.copy_construct(new_element, i_source);
				// from now on, for the whole function, we cant except
				m_end_of_elements = address_add(new_element, i_element_info.size());

				// construct the typeinfo - if this would throw, the element just constructed would not be destroyed. A static_assert guarantees the noexcept-ness.
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_types, m_end_of_types + 1);
				#endif
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ElementType(i_element_info));
				new(m_end_of_types++) ElementType(i_element_info); 
				return new_element;
			}

			/* Adds a (element-info, element) pair to the list. The new element is move-constructed.
				Note: ELEMENT is not the complete type of the element, as the
				list allows polymorphic types. The use of the ElementType avoid slicing or partial constructions. */
			void * add_by_move(const ElementType & i_element_info, ELEMENT && i_source)
			{
				// move-construct the element first (this may throw)
				void * new_element = address_upper_align(m_end_of_elements, i_element_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(new_element, address_add(new_element, i_element_info.size()));
				#endif
				i_element_info.move_construct_if_no_except(new_element, std::move(i_source));
				// from now on, for the whole function, we cant except
				m_end_of_elements = address_add(new_element, i_element_info.size());

				// construct the typeinfo - if this would throw, the element just constructed would not be destroyed. A static_assert guarantees the noexcept-ness.
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_types, m_end_of_types + 1);
				#endif
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ElementType(i_element_info));
				new(m_end_of_types++) ElementType(i_element_info);
				return new_element;
			}

			void add_only_element_info(const ElementType & i_element_info) REFLECTIVE_NOEXCEPT
			{
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ElementType(i_element_info));
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_types, m_end_of_types + 1);
				#endif
				new(m_end_of_types++) ElementType(i_element_info);
			}

			ElementType * end_of_types()
			{
				return m_end_of_types;
			}

			ElementType * commit()
			{
				return m_element_infos;
			}

			void rollback(ALLOCATOR & i_allocator, size_t i_buffer_size, size_t i_buffer_alignment) REFLECTIVE_NOEXCEPT
			{
				if (m_element_infos != nullptr)
				{
					void * element = m_elements;
					for (ElementType * element_info = m_element_infos; element_info < m_end_of_types; element_info++)
					{
						element = address_upper_align(element, element_info->alignment());
						element_info->destroy( static_cast<ELEMENT*>(element));
						element = address_add(element, element_info->size());
						element_info->~ElementType();
					}
					aligned_free(i_allocator, reinterpret_cast<Header*>( m_element_infos ) - 1, i_buffer_size, i_buffer_alignment);
				}
			}

			#if REFLECTIVE_DENSE_LIST_DEBUG
				void dbg_check_range(const void * i_start, const void * i_end)
				{
					assert(i_start >= m_element_infos && i_end <= m_dbg_end_of_buffer );
				}
			#endif

			ElementType * m_element_infos;
			void * m_elements;
			ElementType * m_end_of_types;
			void * m_end_of_elements;
			#if REFLECTIVE_DENSE_LIST_DEBUG
				void * m_dbg_end_of_buffer;
			#endif
		};

		void destroy_impl() REFLECTIVE_NOEXCEPT
		{
			if (m_buffer != nullptr)
			{
				size_t dense_alignment = std::alignment_of<ElementType>::value;
				const auto end_it = end();
				const size_t dense_size = get_size_not_empty() * sizeof(ElementType);
				for (auto it = begin(); it != end_it; ++it)
				{
					auto curr_type = it.curr_type();
					dense_alignment = details::size_max(dense_alignment, curr_type->alignment());
					curr_type->destroy(it.curr_element());
					curr_type->ElementType::~ElementType();
				}

				Header * const header = reinterpret_cast<Header*>(m_buffer) - 1;
				aligned_free(*static_cast<ALLOCATOR*>(this), header, dense_size, dense_alignment);
			}
		}

		void copy_impl(const DenseList & i_source)
		{
			if (i_source.m_buffer != nullptr)
			{
				ListBuilder builder;
				size_t buffer_size = 0, buffer_alignment = 0;
				i_source.compute_buffer_size_and_alignment(&buffer_size, &buffer_alignment);
				try
				{
					const auto source_size = i_source.get_size_not_empty();
					builder.init(*static_cast<ALLOCATOR*>(this), source_size, buffer_size, buffer_alignment);
					auto const end_it = i_source.cend();
					for (auto it = i_source.cbegin(); it != end_it; ++it)
					{
						builder.add_by_copy(*it.m_curr_type, *it.curr_element());
					}

					m_buffer = builder.commit();
				}
				catch (...)
				{
					builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
					throw;
				}
			}
			else
			{
				m_buffer = nullptr;
			}
		}

		void move_impl(DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			m_buffer = i_source.m_buffer;
			i_source.m_buffer = nullptr;
		}

		template <typename... TYPES>
		static void make_impl(DenseList & o_dest_list, TYPES &&... i_args)
		{
			assert(o_dest_list.m_buffer == nullptr); // precondition

			size_t const buffer_size = RecursiveSize<RecursiveHelper<TYPES...>::s_element_count * sizeof(ElementType), TYPES...>::s_buffer_size;
			size_t const buffer_alignment = details::size_max(RecursiveHelper<TYPES...>::s_element_alignment, std::alignment_of<ElementType>::value);
			size_t const element_count = RecursiveHelper<TYPES...>::s_element_count;

			bool is_empty = element_count == 0;
			if (!is_empty) // this bool avoids 'warning C4127: conditional expression is constant' in visual studio
			{
				ListBuilder builder;
				try
				{
					builder.init(static_cast<ALLOCATOR&>(o_dest_list), element_count, buffer_size, buffer_alignment);

					RecursiveHelper<TYPES...>::construct(builder, std::forward<TYPES>(i_args)...);

					o_dest_list.m_buffer = builder.commit();
				}
				catch (...)
				{
					builder.rollback(static_cast<ALLOCATOR&>(o_dest_list), buffer_size, buffer_alignment);
					throw;
				}
			}

			#ifndef NDEBUG
				size_t dbg_buffer_size = 0, dbg_buffer_alignment = 0;
				o_dest_list.compute_buffer_size_and_alignment(&dbg_buffer_size, &dbg_buffer_alignment);
				assert(dbg_buffer_size == buffer_size);
				assert(dbg_buffer_alignment == buffer_alignment);
			#endif
		}

		void compute_buffer_size_and_alignment(size_t * o_buffer_size, size_t * o_buffer_alignment) const REFLECTIVE_NOEXCEPT
		{
			size_t buffer_size = size() * sizeof(ElementType);
			size_t buffer_alignment = std::alignment_of<ElementType>::value;
			auto const end_it = cend();
			for (auto it = cbegin(); it != end_it; ++it)
			{
				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_size > 0 && is_power_of_2(curr_alignment));
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;

				buffer_alignment = details::size_max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		void compute_buffer_size_and_alignment_for_insert(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const ElementType * i_insert_at, size_t i_new_element_count, const ElementType & i_new_type ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_new_type.size() > 0 && is_power_of_2(i_new_type.alignment())); // the size must be non-zero, the alignment must be a non-zero power of 2

			size_t buffer_size = (size() + i_new_element_count) * sizeof(ElementType);
			size_t buffer_alignment = details::size_max(std::alignment_of<ElementType>::value, i_new_type.alignment());
			auto const end_it = cend();
			for (auto it = cbegin(); ; ++it)
			{
				if (it.m_curr_type == i_insert_at && i_new_element_count > 0)
				{					
					const auto alignment_mask = i_new_type.alignment() - 1;
					buffer_size = (buffer_size + alignment_mask) & ~alignment_mask;
					buffer_size += i_new_type.size() * i_new_element_count;
				}

				if (it == end_it)
				{
					break;
				}

				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_size > 0 && is_power_of_2(curr_alignment)); // the size must be non-zero, the alignment must be a non-zero power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;
				buffer_alignment = details::size_max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}
		

		iterator insert_n_impl(const ElementType * i_position, size_t i_count_to_insert, const ElementType & i_source_type, const ELEMENT & i_source)
		{
			assert(i_count_to_insert > 0);

			const ElementType * return_element_info = nullptr;
			void * return_element = nullptr;
			
			size_t buffer_size = 0, buffer_alignment = 0;
			compute_buffer_size_and_alignment_for_insert(&buffer_size, &buffer_alignment, i_position, i_count_to_insert, i_source_type);

			ListBuilder builder;				
			try
			{
				builder.init(*static_cast<ALLOCATOR*>(this), size() + i_count_to_insert, buffer_size, buffer_alignment);
					
				size_t count_to_insert = i_count_to_insert;
				auto const end_it = cend();
				for (auto it = cbegin();;)
				{
					if (it.m_curr_type == i_position && count_to_insert > 0)
					{
						auto const end_of_types = builder.end_of_types();
						void * const new_element = builder.add_by_copy(i_source_type, i_source);
						if (count_to_insert == i_count_to_insert)
						{
							return_element_info = end_of_types;
							return_element = new_element;
						}
						count_to_insert--;
					}
					else
					{
						if (it == end_it)
						{
							break;
						}
						builder.add_by_copy(*it.m_curr_type, *it.curr_element());
						++it;
					}						
				}

				destroy_impl();

				m_buffer = builder.commit();
			}
			catch (...)
			{
				builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
				throw;
			}

			return iterator(InternalConstructorMem, return_element, return_element_info);
		}

		iterator erase_impl(const ElementType * i_from, const ElementType * i_to)
		{
			// test preconditions
			const auto prev_size = get_size_not_empty();
			assert( m_buffer != nullptr && i_from < i_to &&
				i_from >= m_buffer && i_from <= m_buffer + prev_size &&
				i_to >= m_buffer && i_to <= m_buffer + prev_size);

			const size_t size_to_remove = i_to - i_from;

			assert(size_to_remove <= prev_size);
			if (size_to_remove == prev_size)
			{
				// erasing all the elements
				assert(i_from == m_buffer && i_to == m_buffer + prev_size);
				clear();
				return begin();
			}
			else
			{
				size_t buffer_size = 0, buffer_alignment = 0;
				compute_buffer_size_and_alignment_for_erase(&buffer_size, &buffer_alignment, i_from, i_to);

				const ElementType * return_element_info = nullptr;
				void * return_element = nullptr;

				ListBuilder builder;
				try
				{
					builder.init(*static_cast<ALLOCATOR*>(this), prev_size - size_to_remove, buffer_size, buffer_alignment);

					const auto end_it = cend();
					bool is_in_range = false;
					bool first_in_range = false;
					for (auto it = cbegin(); ; it++)
					{
						if (it.m_curr_type == i_from)
						{
							is_in_range = true;
							first_in_range = true;
						}
						if (it.m_curr_type == i_to)
						{
							is_in_range = false;
						}

						if (it == end_it)
						{
							assert(!is_in_range);
							break;
						}

						if (!is_in_range)
						{
							auto const new_element_info = builder.end_of_types();
							auto const new_element = builder.add_by_copy(*it.m_curr_type, *it.curr_element());

							if (first_in_range)
							{
								return_element_info = new_element_info;
								return_element = new_element;
								first_in_range = false;
							}
						}
					}

					if (return_element_info == nullptr) // if no elements were copied after the erased range
					{
						assert(i_to == m_buffer + prev_size);
						return_element_info = builder.end_of_types();
					}

					destroy_impl();

					m_buffer = builder.commit();
				}
				catch (...)
				{
					builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
					throw;
				}
				return iterator(InternalConstructorMem, return_element, return_element_info);
			}
		}

		void compute_buffer_size_and_alignment_for_erase(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const ElementType * i_remove_from, const ElementType * i_remove_to ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_remove_to >= i_remove_from );
			const size_t size_to_remove = i_remove_to - i_remove_from;
			assert(size() >= size_to_remove);
			size_t buffer_size = (size() - size_to_remove) * sizeof(ElementType);
			size_t buffer_alignment = std::alignment_of<ElementType>::value;
			
			bool in_range = false;
			auto const end_it = cend();
			for (auto it = cbegin(); it != end_it; ++it)
			{
				if (it.m_curr_type == i_remove_from)
				{
					in_range = true;
				}
				if (it.m_curr_type == i_remove_to)
				{
					in_range = false;
				}

				if (!in_range)
				{
					const size_t curr_size = it.curr_type()->size();
					const size_t curr_alignment = it.curr_type()->alignment();
					assert(curr_size > 0 && is_power_of_2(curr_alignment)); // the size must be non-zero, the alignment must be a non-zero power of 2
					buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
					buffer_size += curr_size;
					buffer_alignment = details::size_max(buffer_alignment, curr_alignment);
				}
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		template <size_t PREV_ELEMENTS_SIZE, typename...> struct RecursiveSize
		{
			static const size_t s_buffer_size = PREV_ELEMENTS_SIZE;
		};
		template <size_t PREV_ELEMENTS_SIZE, typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveSize<PREV_ELEMENTS_SIZE, FIRST_TYPE, OTHER_TYPES...>
		{
			static const size_t s_aligned_prev_size = (PREV_ELEMENTS_SIZE + (std::alignment_of<FIRST_TYPE>::value - 1)) & ~(std::alignment_of<FIRST_TYPE>::value - 1);
			static const size_t s_buffer_size = RecursiveSize<s_aligned_prev_size + sizeof(FIRST_TYPE), OTHER_TYPES...>::s_buffer_size;
		};

		template <typename... TYPES>
			struct RecursiveHelper
		{
			static const size_t s_element_count = 0;
			static const size_t s_element_alignment = 1;
			static void construct(ListBuilder &, TYPES &&...) { }
		};
		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveHelper<FIRST_TYPE, OTHER_TYPES...>
		{
			static_assert(std::alignment_of<FIRST_TYPE>::value > 0 && (std::alignment_of<FIRST_TYPE>::value & (std::alignment_of<FIRST_TYPE>::value - 1)) == 0,
				"the alignment must be a non-zero integer power of 2" );

			static_assert(std::is_base_of<ELEMENT, FIRST_TYPE >::value, "Trying to initiaize a dense list with an object whose type is not a subtype of the base element type");

			static const size_t s_element_count = 1 + RecursiveHelper<OTHER_TYPES...>::s_element_count;
			static const size_t s_element_alignment = std::alignment_of<FIRST_TYPE>::value > RecursiveHelper<OTHER_TYPES...>::s_element_alignment ?
				std::alignment_of<FIRST_TYPE>::value : RecursiveHelper<OTHER_TYPES...>::s_element_alignment;

			inline static void construct(ListBuilder & i_builder, FIRST_TYPE && i_source, OTHER_TYPES && ... i_other_arguments)
				// REFLECTIVE_NOEXCEPT_V( new (nullptr) FIRST_TYPE(std::forward<FIRST_TYPE>(std::declval<FIRST_TYPE>())) )
			{
				void * new_element = address_upper_align(i_builder.m_end_of_elements, std::alignment_of<FIRST_TYPE>::value );
				new (new_element) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				i_builder.m_end_of_elements = address_add(new_element, sizeof(FIRST_TYPE));
				#if REFLECTIVE_DENSE_LIST_DEBUG
					i_builder.dbg_check_range(new_element, i_builder.m_end_of_elements);
				#endif
	
				i_builder.add_only_element_info(ElementType::template make<FIRST_TYPE>());

				RecursiveHelper<OTHER_TYPES...>::construct( i_builder, std::forward<OTHER_TYPES>(i_other_arguments)...);
			}
		};

	private:
		ElementType * m_buffer;
	};

} // namespace reflective