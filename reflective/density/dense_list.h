
#pragma once
#include <memory>
#include "element_type.h"
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

	template < typename ALLOCATOR, typename ELEMENT_TYPE >
		class DenseListBase : private ALLOCATOR
	{
	public:

		size_t size() const REFLECTIVE_NOEXCEPT
		{
			if (m_types != nullptr)
			{
				Header * const header = reinterpret_cast<Header*>(m_types) - 1;
				return header->m_count;
			}
			else
			{
				return 0;
			}
		}

		bool empty() const REFLECTIVE_NOEXCEPT
		{
			return m_types == nullptr;
		}

		void clear() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
			m_types = nullptr;
		}

	protected:
		
		DenseListBase() REFLECTIVE_NOEXCEPT
			: m_types(nullptr)
				{ }

		DenseListBase(DenseListBase && i_source) REFLECTIVE_NOEXCEPT
		{
			move_impl(std::move(i_source));
		}

		DenseListBase & operator = (DenseListBase && i_source) REFLECTIVE_NOEXCEPT
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			move_impl(std::move(i_source));
			return *this;
		}

		template <typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
			DenseListBase(const DenseListBase & i_source)
		{
			copy_impl(i_source);
		}

		template <typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
			DenseListBase & operator = (const DenseListBase & i_source)
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			copy_impl(i_source);
			return *this;
		}

		~DenseListBase() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
		}
		
		struct BaseIterator
		{
			BaseIterator() REFLECTIVE_NOEXCEPT
				: m_unaligned_curr_element(nullptr), m_curr_type(nullptr) { }

			BaseIterator(void * i_curr_element, const ELEMENT_TYPE * i_curr_type) REFLECTIVE_NOEXCEPT
				: m_unaligned_curr_element(i_curr_element), m_curr_type(i_curr_type)
			{
			}

			void move_next() REFLECTIVE_NOEXCEPT
			{
				void * const prev_element = curr_element();
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				m_unaligned_curr_element = reinterpret_cast<value_type*>(reinterpret_cast<uintptr_t>(prev_element) + curr_element_size);
			}			
			
			void * curr_element() const REFLECTIVE_NOEXCEPT
			{
				auto const curr_element_alignment = m_curr_type->alignment();
				return address_upper_align(m_unaligned_curr_element, curr_element_alignment);
			}

			bool operator == (const BaseIterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type == i_other.m_curr_type;
			}

			bool operator != (const BaseIterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type != i_other.m_curr_type;
			}

			void operator ++ () REFLECTIVE_NOEXCEPT
			{
				move_next();
			}

			void * m_unaligned_curr_element;
			const ELEMENT_TYPE * m_curr_type;
			
		}; // class BaseIterator

		BaseIterator begin() const REFLECTIVE_NOEXCEPT { return BaseIterator(get_elements(), m_types); }
		BaseIterator end() const REFLECTIVE_NOEXCEPT { return BaseIterator(nullptr, m_types + size()); }
		
		void * get_elements() const // this function gives a non-const element from a const container, but this avoids duplicating the function
		{
			return m_types + size();
		}

		size_t get_size_not_empty() const REFLECTIVE_NOEXCEPT
		{
			if (m_types != nullptr)
			{
				return (reinterpret_cast<Header*>(m_types)-1)->m_count;
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
				m_end_of_types = m_element_infos = reinterpret_cast<ELEMENT_TYPE*>(header + 1);
				m_end_of_elements = m_elements = m_element_infos + i_count;

				#if REFLECTIVE_DENSE_LIST_DEBUG
					m_dbg_end_of_buffer = address_add(m_element_infos, i_buffer_size);
				#endif
			}

			/* Adds a (type-info, element) pair to the list. The new element is copy-constructed. 
				Note: ELEMENT is not the comlete type of the element, as the
				list allows polymorphic types. The use of the ELEMENT_TYPE avoid slicing or partial constructions. */
			template <typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
				void * add_by_copy(const ELEMENT_TYPE & i_element_info, const void * i_source)
					// REFLECTIVE_NOEXCEPT_V(std::declval<ELEMENT_TYPE>().copy_construct(nullptr, std::declval<ELEMENT>() ))
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
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ELEMENT_TYPE(i_element_info));
				new(m_end_of_types++) ELEMENT_TYPE(i_element_info);
				return new_element;
			}

			/* Adds a (element-info, element) pair to the list. The new element is move-constructed.
				Note: ELEMENT is not the complete type of the element, as the
				list allows polymorphic types. The use of the ELEMENT_TYPE avoid slicing or partial constructions. */
			void * add_by_move(const ELEMENT_TYPE & i_element_info, void * i_source)
			{
				// move-construct the element first (this may throw)
				void * new_element = address_upper_align(m_end_of_elements, i_element_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(new_element, address_add(new_element, i_element_info.size()));
				#endif
				i_element_info.move_construct(new_element, i_source);
				// from now on, for the whole function, we cant except
				m_end_of_elements = address_add(new_element, i_element_info.size());

				// construct the typeinfo - if this would throw, the element just constructed would not be destroyed. A static_assert guarantees the noexcept-ness.
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_types, m_end_of_types + 1);
				#endif
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ELEMENT_TYPE(i_element_info));
				new(m_end_of_types++) ELEMENT_TYPE(i_element_info);
				return new_element;
			}

			void add_only_element_info(const ELEMENT_TYPE & i_element_info) REFLECTIVE_NOEXCEPT
			{
				REFLECTIVE_ASSERT_NOEXCEPT(new(m_end_of_types++) ELEMENT_TYPE(i_element_info));
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_types, m_end_of_types + 1);
				#endif
				new(m_end_of_types++) ELEMENT_TYPE(i_element_info);
			}

			ELEMENT_TYPE * end_of_types()
			{
				return m_end_of_types;
			}

			ELEMENT_TYPE * commit()
			{
				return m_element_infos;
			}

			void rollback(ALLOCATOR & i_allocator, size_t i_buffer_size, size_t i_buffer_alignment) REFLECTIVE_NOEXCEPT
			{
				if (m_element_infos != nullptr)
				{
					void * element = m_elements;
					for (ELEMENT_TYPE * element_info = m_element_infos; element_info < m_end_of_types; element_info++)
					{
						element = address_upper_align(element, element_info->alignment());
						element_info->destroy(element);
						element = address_add(element, element_info->size());
						element_info->~ELEMENT_TYPE();
					}
					aligned_free(i_allocator, reinterpret_cast<Header*>( m_element_infos ) - 1, i_buffer_size, i_buffer_alignment);
				}
			}

			#if REFLECTIVE_DENSE_LIST_DEBUG
				void dbg_check_range(const void * i_start, const void * i_end) REFLECTIVE_NOEXCEPT
				{
					assert(i_start >= m_element_infos && i_end <= m_dbg_end_of_buffer );
				}
			#endif

			ELEMENT_TYPE * m_element_infos;
			void * m_elements;
			ELEMENT_TYPE * m_end_of_types;
			void * m_end_of_elements;
			#if REFLECTIVE_DENSE_LIST_DEBUG
				void * m_dbg_end_of_buffer;
			#endif
		};

		void destroy_impl() REFLECTIVE_NOEXCEPT
		{
			if (m_types != nullptr)
			{
				size_t dense_alignment = std::alignment_of<ELEMENT_TYPE>::value;
				const auto end_it = end();
				const size_t dense_size = get_size_not_empty() * sizeof(ELEMENT_TYPE);
				for (auto it = begin(); it != end_it; ++it)
				{
					auto curr_type = it.m_curr_type;
					dense_alignment = details::size_max(dense_alignment, curr_type->alignment());
					curr_type->destroy(it.curr_element());
					curr_type->ELEMENT_TYPE::~ELEMENT_TYPE();
				}

				Header * const header = reinterpret_cast<Header*>(m_types) - 1;
				aligned_free(*static_cast<ALLOCATOR*>(this), header, dense_size, dense_alignment);
			}
		}

		template <typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
			void copy_impl(const DenseListBase & i_source)
		{
			if (i_source.m_types != nullptr)
			{
				ListBuilder builder;
				size_t buffer_size = 0, buffer_alignment = 0;
				i_source.compute_buffer_size_and_alignment(&buffer_size, &buffer_alignment);
				try
				{
					const auto source_size = i_source.get_size_not_empty();
					builder.init(*static_cast<ALLOCATOR*>(this), source_size, buffer_size, buffer_alignment);
					auto const end_it = i_source.end();
					for (auto it = i_source.begin(); it != end_it; ++it)
					{
						builder.add_by_copy(*it.m_curr_type, it.curr_element());
					}

					m_types = builder.commit();
				}
				catch (...)
				{
					builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
					throw;
				}
			}
			else
			{
				m_types = nullptr;
			}
		}

		void move_impl(DenseListBase && i_source) REFLECTIVE_NOEXCEPT
		{
			m_types = i_source.m_types;
			i_source.m_types = nullptr;
		}

		template <typename... TYPES>
			static void make_impl(DenseListBase & o_dest_list, TYPES &&... i_args)
		{
			assert(o_dest_list.m_types == nullptr); // precondition

			size_t const buffer_size = RecursiveSize<RecursiveHelper<TYPES...>::s_element_count * sizeof(ELEMENT_TYPE), TYPES...>::s_buffer_size;
			size_t const buffer_alignment = details::size_max(RecursiveHelper<TYPES...>::s_element_alignment, std::alignment_of<ELEMENT_TYPE>::value);
			size_t const element_count = RecursiveHelper<TYPES...>::s_element_count;

			bool is_empty = element_count == 0;
			if (!is_empty) // this bool avoids 'warning C4127: conditional expression is constant' in visual studio
			{
				ListBuilder builder;
				try
				{
					builder.init(static_cast<ALLOCATOR&>(o_dest_list), element_count, buffer_size, buffer_alignment);

					RecursiveHelper<TYPES...>::construct(builder, std::forward<TYPES>(i_args)...);

					o_dest_list.m_types = builder.commit();
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
			size_t buffer_size = size() * sizeof(ELEMENT_TYPE);
			size_t buffer_alignment = std::alignment_of<ELEMENT_TYPE>::value;
			auto const end_it = end();
			for (auto it = begin(); it != end_it; ++it)
			{
				const size_t curr_size = it.m_curr_type->size();
				const size_t curr_alignment = it.m_curr_type->alignment();
				assert(curr_size > 0 && is_power_of_2(curr_alignment));
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;

				buffer_alignment = details::size_max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		void compute_buffer_size_and_alignment_for_insert(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const ELEMENT_TYPE * i_insert_at, size_t i_new_element_count, const ELEMENT_TYPE & i_new_type ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_new_type.size() > 0 && is_power_of_2(i_new_type.alignment())); // the size must be non-zero, the alignment must be a non-zero power of 2

			size_t buffer_size = (size() + i_new_element_count) * sizeof(ELEMENT_TYPE);
			size_t buffer_alignment = details::size_max(std::alignment_of<ELEMENT_TYPE>::value, i_new_type.alignment());
			auto const end_it = end();
			for (auto it = begin(); ; ++it)
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

				const size_t curr_size = it.m_curr_type->size();
				const size_t curr_alignment = it.m_curr_type->alignment();
				assert(curr_size > 0 && is_power_of_2(curr_alignment)); // the size must be non-zero, the alignment must be a non-zero power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;
				buffer_alignment = details::size_max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		enum class InsertOp
		{
			copy,
			move
		};
		template <InsertOp> static void * insert_single(ListBuilder & i_builder, const ELEMENT_TYPE & i_source_type, void * i_source);
		template <> static void * insert_single<InsertOp::copy>(ListBuilder & i_builder, const ELEMENT_TYPE & i_source_type, void * i_source)
		{
			return i_builder.add_by_copy(i_source_type, i_source);
		}
		template <> static void * insert_single<InsertOp::move>(ListBuilder & i_builder, const ELEMENT_TYPE & i_source_type, void * i_source)
		{
			return i_builder.add_by_move(i_source_type, i_source);
		}

		template <InsertOp OP>
			BaseIterator insert_n_impl(const ELEMENT_TYPE * i_position, size_t i_count_to_insert, const ELEMENT_TYPE & i_source_type, void * i_source )
		{
			assert(i_count_to_insert > 0);

			const ELEMENT_TYPE * return_element_info = nullptr;
			void * return_element = nullptr;
			
			size_t buffer_size = 0, buffer_alignment = 0;
			compute_buffer_size_and_alignment_for_insert(&buffer_size, &buffer_alignment, i_position, i_count_to_insert, i_source_type);

			ListBuilder builder;				
			try
			{
				builder.init(*static_cast<ALLOCATOR*>(this), size() + i_count_to_insert, buffer_size, buffer_alignment);
					
				size_t count_to_insert = i_count_to_insert;
				auto const end_it = end();
				for (auto it = begin();;)
				{
					if (it.m_curr_type == i_position && count_to_insert > 0)
					{
						auto const end_of_types = builder.end_of_types();
						void * const new_element = insert_single<OP>(builder, i_source_type, i_source);
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
						builder.add_by_move(*it.m_curr_type, it.curr_element());
						it.move_next();
					}						
				}

				destroy_impl();

				m_types = builder.commit();
			}
			catch (...)
			{
				builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
				throw;
			}

			return BaseIterator(return_element, return_element_info);
		}

		BaseIterator erase_impl(const ELEMENT_TYPE * i_from, const ELEMENT_TYPE * i_to)
		{
			// test preconditions
			const auto prev_size = get_size_not_empty();
			assert( m_types != nullptr && i_from < i_to &&
				i_from >= m_types && i_from <= m_types + prev_size &&
				i_to >= m_types && i_to <= m_types + prev_size);

			const size_t size_to_remove = i_to - i_from;

			assert(size_to_remove <= prev_size);
			if (size_to_remove == prev_size)
			{
				// erasing all the elements
				assert(i_from == m_types && i_to == m_types + prev_size);
				clear();
				return begin();
			}
			else
			{
				size_t buffer_size = 0, buffer_alignment = 0;
				compute_buffer_size_and_alignment_for_erase(&buffer_size, &buffer_alignment, i_from, i_to);

				const ELEMENT_TYPE * return_element_info = nullptr;
				void * return_element = nullptr;

				ListBuilder builder;
				try
				{
					builder.init(*static_cast<ALLOCATOR*>(this), prev_size - size_to_remove, buffer_size, buffer_alignment);

					const auto end_it = end();
					bool is_in_range = false;
					bool first_in_range = false;
					for (auto it = begin(); ; ++it)
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
							auto const new_element = builder.add_by_move(*it.m_curr_type, it.curr_element());

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
						assert(i_to == m_types + prev_size);
						return_element_info = builder.end_of_types();
					}

					destroy_impl();

					m_types = builder.commit();
				}
				catch (...)
				{
					builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
					throw;
				}
				return BaseIterator(return_element, return_element_info);
			}
		}

		void compute_buffer_size_and_alignment_for_erase(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const ELEMENT_TYPE * i_remove_from, const ELEMENT_TYPE * i_remove_to ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_remove_to >= i_remove_from );
			const size_t size_to_remove = i_remove_to - i_remove_from;
			assert(size() >= size_to_remove);
			size_t buffer_size = (size() - size_to_remove) * sizeof(ELEMENT_TYPE);
			size_t buffer_alignment = std::alignment_of<ELEMENT_TYPE>::value;
			
			bool in_range = false;
			auto const end_it = end();
			for (auto it = begin(); it != end_it; ++it)
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
					const size_t curr_size = it.m_curr_type->size();
					const size_t curr_alignment = it.m_curr_type->alignment();
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
	
				i_builder.add_only_element_info(ELEMENT_TYPE::template make<FIRST_TYPE>());

				RecursiveHelper<OTHER_TYPES...>::construct( i_builder, std::forward<OTHER_TYPES>(i_other_arguments)...);
			}
		};

		ELEMENT_TYPE * m_types;
	};

	/** A dense-list is a polymorphic sequence container optimized to be compact in both heap memory and inline storage.
		Elements is a DenseList are allocated respecting their alignment requirements.
		In a polymorphic container every element can have a different complete type, provided that this type is covariant to the type ELEMENT.
		All the elements of a DenseList are arranged in the same memory block of the heap. 
		Insertions\removals of a non-zero number of elements and clear() always reallocate the memory blocks and invalidate existing iterators.
		The inline storage of DenseList is the same of a pointer. An empty DenseList does not use heap memory.
		All the functions of DenseList gives at least the strong exception guarantee. */
	template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, typename ELEMENT_TYPE = AutomaticElementType<ELEMENT> >
		class DenseList final : public DenseListBase<ALLOCATOR, ELEMENT_TYPE>
	{
	private:
		using BaseClass = DenseListBase<ALLOCATOR, ELEMENT_TYPE>;
		
	public:

		using allocator_type = ALLOCATOR;
		using difference_type = ptrdiff_t;
		using size_type = size_t;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

		/** Alias for the template arguments */
		using ElementType = ELEMENT_TYPE;
		
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
			static_assert(AllCovariant<ELEMENT, TYPES...>::value, "Al the paraneter types must be covariant to ELEMENT" );
			DenseList new_list;
			BaseClass::make_impl(new_list, std::forward<TYPES>(i_args)...);
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
			static_assert(AllCovariant<ELEMENT, TYPES...>::value, "Al the paraneter types must be covariant to ELEMENT");
			DenseList new_list;
			BaseClass::make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		class iterator;
		class const_iterator;

		using BaseIterator = typename DenseListBase<ALLOCATOR, ELEMENT_TYPE>::BaseIterator;

		class iterator final : private BaseIterator
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

			iterator(const BaseIterator & i_source) REFLECTIVE_NOEXCEPT
				: BaseIterator(i_source) {  }
			
			value_type & operator * () const REFLECTIVE_NOEXCEPT { return *curr_element(); }
			value_type * operator -> () const REFLECTIVE_NOEXCEPT { return curr_element(); }
			value_type * curr_element() const REFLECTIVE_NOEXCEPT
				{ return static_cast<value_type *>(BaseIterator::curr_element()); }

			iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				move_next();
				return *this;
			}

			iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				iterator copy(*this);
				move_next();
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
			
			const ELEMENT_TYPE * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

			friend class const_iterator;

		}; // class iterator

		class const_iterator final : private BaseIterator
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

			const_iterator(const BaseIterator & i_source) REFLECTIVE_NOEXCEPT
				: BaseIterator(i_source) {  }

			const_iterator(const iterator & i_source) REFLECTIVE_NOEXCEPT
				: BaseIterator(i_source) {  }
									
			value_type & operator * () const REFLECTIVE_NOEXCEPT { return *curr_element(); }
			value_type * operator -> () const REFLECTIVE_NOEXCEPT { return curr_element(); }
			value_type * curr_element() const REFLECTIVE_NOEXCEPT
				{ return static_cast<value_type *>(BaseIterator::curr_element()); }

			const_iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				move_next();
				return *this;
			}

			const_iterator operator++ (int) REFLECTIVE_NOEXCEPT
			{
				iterator copy(*this);
				move_next();
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

			const ELEMENT_TYPE * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }
		
			friend class DenseList;

		}; // class const_iterator

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(BaseClass::begin()); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(BaseClass::end()); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::begin()); }
		const_iterator end() const REFLECTIVE_NOEXCEPT{ return const_iterator(BaseClass::end()); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::begin()); }
		const_iterator cend() const REFLECTIVE_NOEXCEPT { return const_iterator(BaseClass::end()); }

		template <typename ELEMENT_COMPLETE_TYPE, typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
			void push_back(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			insert_n_impl<InsertOp::copy>(m_types + size(), 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(),
				const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source) );
		}

		template <typename ELEMENT_COMPLETE_TYPE, typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::copy_only) != 0, void >::type>
			void push_front(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			insert_n_impl<InsertOp::copy>(m_types, 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(),
				const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source));
		}

		template <typename ELEMENT_COMPLETE_TYPE, typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::move_only) != 0, void >::type>
			void push_back(ELEMENT_COMPLETE_TYPE && i_source)
		{
			insert_n_impl<InsertOp::move>(m_types + size(), 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(),
				const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source) );
		}

		template <typename ELEMENT_COMPLETE_TYPE, typename = typename std::enable_if< (ELEMENT_TYPE::s_caps & ElementTypeCaps::move_only) != 0, void >::type>
			void push_front(ELEMENT_COMPLETE_TYPE && i_source)
		{
			insert_n_impl<InsertOp::move>(m_types, 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(),
				const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source));
		}

		void pop_front()
		{
			erase_impl(m_types, m_types + 1);
		}

		void pop_back()
		{
			auto const end_type = m_types + get_size_not_empty();
			erase_impl(end_type - 1, end_type);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl<InsertOp::copy>(i_position.m_curr_type, 1, ElementType::template make<ELEMENT_COMPLETE_TYPE>(),
				const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source));
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, size_t i_count, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			if (i_count > 0)
			{
				return insert_n_impl<InsertOp::copy>(i_position.m_curr_type, i_count, ElementType::template make<ELEMENT_COMPLETE_TYPE>(), const_cast<ELEMENT_COMPLETE_TYPE*>(&i_source));
			}
			else
			{
				// inserting 0 elements
				return iterator(i_position);
			}
		}

		iterator erase(const_iterator i_position)
		{
			return erase_impl(i_position.m_curr_type, i_position.m_curr_type + 1);
		}

		iterator erase(const_iterator i_from, const_iterator i_to)
		{
			auto from_type = i_from.curr_type();
			auto to_type = i_to.curr_type();
			if (from_type != to_type)
			{
				return erase_impl(from_type, to_type);
			}
			else
			{
				// removing 0 elements
				return iterator(i_from);
			}
		}

					/////////////////////////

		/* to do, & WARNING!: this function is slicing-comparing. Fix or delete. */
		bool equal_to(const DenseList & i_source) const
		{
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

		bool operator == (const DenseList & i_source) const { return equal_to(i_source); }
		bool operator != (const DenseList & i_source) const { return !equal_to(i_source); }

	}; // class DenseList;

	template <typename ELEMENT, typename... TYPES>
		inline DenseList<ELEMENT> make_dense_list(TYPES &&... i_args)
	{
		return DenseList<ELEMENT>::make(std::forward<TYPES>(i_args)...);
	}

} // namespace reflective