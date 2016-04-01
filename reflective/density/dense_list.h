
#pragma once

#ifdef NDEBUG
	#define REFLECTIVE_DENSE_LIST_DEBUG		0
#else
	#define REFLECTIVE_DENSE_LIST_DEBUG		1
#endif

namespace reflective
{
	template <typename ELEMENT> class CopyableTypeInfo;
	template <typename ELEMENT> class MovableTypeInfo;
	template <typename ELEMENT> using DefaultTypeInfo = typename std::conditional< std::is_copy_constructible<ELEMENT>::value,
		CopyableTypeInfo<ELEMENT>, MovableTypeInfo<ELEMENT> >::type;
	template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, 
		typename TYPE_INFO = DefaultTypeInfo<ELEMENT> > class DenseList;

	/** Creates and returns by value a DenseList, whose element base type is ELEMENT. The specified parameters
		are added to the list<br>
		Example: auto int_list = make_dense_list<int>( 1, 2 * 2, 3 * 3 ); */
	template <typename ELEMENT, typename... TYPES>
		inline DenseList<ELEMENT, std::allocator<ELEMENT>> make_dense_list(TYPES &&... i_parameters)
	{
		return DenseList<ELEMENT, std::allocator<ELEMENT>>::make( std::forward<TYPES>(i_parameters)... );
	}
	template <typename ELEMENT, typename ALLOCATOR, typename... TYPES>
		inline DenseList<ELEMENT, ALLOCATOR> alloc_dense_list(const ALLOCATOR & i_allocator, TYPES &&... i_parameters)
	{
		return DenseList<ELEMENT, ALLOCATOR>::make_with_alloc(i_allocator, std::forward<TYPES>(i_parameters)... );
	}

	template <typename ELEMENT>
		class CopyableTypeInfo
	{
	public:

		CopyableTypeInfo() = delete;
		CopyableTypeInfo & operator = (const CopyableTypeInfo &) = delete;
		CopyableTypeInfo & operator = (CopyableTypeInfo &&) = delete;

		CopyableTypeInfo(const CopyableTypeInfo &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			CopyableTypeInfo(CopyableTypeInfo && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
					{ }			
		#else
			CopyableTypeInfo(CopyableTypeInfo && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static CopyableTypeInfo make()
		{
			return CopyableTypeInfo(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, 
				&CopyConstructImpl< COMPLETE_TYPE >, &MoverDestructorImpl< COMPLETE_TYPE >);
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
		
		size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
		
		void copy_construct(void * i_destination, const ELEMENT & i_source_element) const
		{
			(*m_copy_constructor)(i_destination, i_source_element);
		}

		void move_construct_if_no_except(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

		void destroy(ELEMENT * i_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

	private:

		using CopyConstructorPtr = void(*)(void * i_dest_element, const ELEMENT & i_source_element);
		using MoverDestructorPtr = void (*)(void * i_dest_element, ELEMENT * i_source_element);

		CopyableTypeInfo(size_t i_size, size_t i_alignment, CopyConstructorPtr i_copy_constructor, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_copy_constructor(i_copy_constructor), m_mover_destructor(i_mover_destructor)
				{ }

		template <typename COMPLETE_TYPE>
			static void CopyConstructImpl(void * i_destination, const ELEMENT & i_source_element)
		{
			new (i_destination) COMPLETE_TYPE(static_cast<const COMPLETE_TYPE&>(i_source_element));
		}

		template <typename COMPLETE_TYPE>
			static void MoverDestructorImpl(void * i_destination, ELEMENT * i_source_element)
		{
			if (i_destination != nullptr)
			{
				new (i_destination) COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_source_element)));
			}
			static_cast<COMPLETE_TYPE*>(i_source_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

	private:
		size_t const m_size, m_alignment;
		CopyConstructorPtr const m_copy_constructor;
		MoverDestructorPtr const m_mover_destructor;
	};

	template <typename ELEMENT>
		class MovableTypeInfo
	{
	public:

		MovableTypeInfo() = delete;
		MovableTypeInfo & operator = (const MovableTypeInfo &) = delete;
		MovableTypeInfo & operator = (MovableTypeInfo &&) = delete;

		MovableTypeInfo(const MovableTypeInfo &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			MovableTypeInfo(MovableTypeInfo && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
					{ }			
		#else
			MovableTypeInfo(MovableTypeInfo && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static MovableTypeInfo make()
		{
			return MovableTypeInfo(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &MoverDestructorImpl< COMPLETE_TYPE >);
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
		
		size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
		
		void move_construct_if_no_except(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

		void destroy(ELEMENT * i_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

	private:

		using MoverDestructorPtr = void (*)(void * i_dest_element, ELEMENT * i_source_element);

		MovableTypeInfo(size_t i_size, size_t i_alignment, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_mover_destructor(i_mover_destructor)
				{ }
		
		template <typename COMPLETE_TYPE>
			static void MoverDestructorImpl(void * i_destination, ELEMENT * i_source_element)
		{
			if (i_destination != nullptr)
			{
				new (i_destination) COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_source_element)));
			}
			static_cast<COMPLETE_TYPE*>(i_source_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

	private:
		size_t const m_size, m_alignment;
		MoverDestructorPtr const m_mover_destructor;
	};

	namespace details
	{
		inline bool is_valid_alignment(size_t i_alignment) REFLECTIVE_NOEXCEPT
		{
			return i_alignment > 0 && (i_alignment & (i_alignment - 1)) == 0;
		}

		template <typename TYPE>
			inline TYPE * address_upper_align(void * i_address) REFLECTIVE_NOEXCEPT
		{
			const size_t alignment_mask = std::alignment_of<TYPE>::value - 1;
			return reinterpret_cast<TYPE*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
		}

		inline void * address_upper_align(void * i_address, size_t i_alignment) REFLECTIVE_NOEXCEPT
		{
			assert(is_valid_alignment(i_alignment));
			const size_t alignment_mask = i_alignment - 1;
			return reinterpret_cast<void*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
		}

		template <typename TYPE>
			inline TYPE * address_add(TYPE * i_address, size_t i_offset) REFLECTIVE_NOEXCEPT
		{
			return reinterpret_cast<TYPE*>(reinterpret_cast<uintptr_t>(i_address) + i_offset);
		}

		struct AlignmentHeader
		{
			void * m_block;
		};

		template <typename ALLOCATOR>
			void * aligned_alloc(ALLOCATOR & i_allocator, size_t i_size, size_t i_alignment)
		{
			assert(is_valid_alignment(i_alignment));

			if (i_alignment <= std::alignment_of<void*>::value)
			{
				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<void *> other_alloc(i_allocator);
				return other_alloc.allocate((i_size + sizeof(void*) - 1) / sizeof(void*));
			}
			else
			{
				const size_t extra_size = (i_alignment >= sizeof(AlignmentHeader) ? i_alignment : sizeof(AlignmentHeader));
				const size_t actual_size = i_size + extra_size;

				typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(i_allocator);
				void * complete_block = char_alloc.allocate(actual_size);
				auto uint_address = reinterpret_cast<uintptr_t>(complete_block);

				uint_address += extra_size;
				uint_address &= ~(i_alignment - 1);

				AlignmentHeader * header = reinterpret_cast<AlignmentHeader*>(uint_address) - 1;
				header->m_block = complete_block;

				return reinterpret_cast<void*>(uint_address);
			}
		}

		template <typename ALLOCATOR>
			void aligned_free(ALLOCATOR & i_allocator, void * i_block, size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
		{
			if (i_block != nullptr)
			{
				if (i_alignment <= std::alignment_of<void*>::value)
				{
					typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<void *> other_alloc(i_allocator);
					other_alloc.deallocate(static_cast<void**>(i_block), (i_size + sizeof(void*) - 1) / sizeof(void*));
				}
				else
				{
					const size_t extra_size = (i_alignment >= sizeof(AlignmentHeader) ? i_alignment : sizeof(AlignmentHeader));
					const size_t actual_size = i_size + extra_size;

					AlignmentHeader * header = static_cast<AlignmentHeader*>(i_block) - 1;

					typename std::allocator_traits<ALLOCATOR>::template rebind_alloc<char> char_alloc(i_allocator);
					char_alloc.deallocate(static_cast<char*>(header->m_block), actual_size);
				}
			}
		}
			
	} // namespace details

	/** A dense-list is a sequence container of heterogeneous elements. */
	template <typename ELEMENT, typename ALLOCATOR, typename TYPE_INFO >
		class DenseList : private ALLOCATOR
	{
		enum InternalConstructor { InternalConstructorMem };

	public:

		using TypeInfo = TYPE_INFO;

		using allocator_type = ALLOCATOR;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		template <typename... TYPES>
			inline static DenseList make(TYPES &&... i_args)
		{
			DenseList new_list;
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		template <typename... TYPES>
			inline static DenseList make_with_alloc(const ALLOCATOR & i_allocator, TYPES &&... i_args)
		{
			DenseList new_list;
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		DenseList() REFLECTIVE_NOEXCEPT
			: m_buffer(nullptr), m_size(0)
				{ }

		DenseList(DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			move_impl(std::move(i_source));
		}

		DenseList & operator = (DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			move_impl(std::move(i_source));
			return *this;
		}

		DenseList(const DenseList & i_source)
		{
			copy_impl(i_source);
		}

		DenseList & operator = (const DenseList & i_source)
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			copy_impl(i_source);
			return *this;
		}

		~DenseList() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }

		bool empty() const REFLECTIVE_NOEXCEPT  { return m_size == 0; }

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

			iterator(InternalConstructor, VoidPtr i_curr_element, const TypeInfo * i_curr_type) REFLECTIVE_NOEXCEPT
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
				assert(details::is_valid_alignment(curr_element_alignment));
				return reinterpret_cast<value_type*>(
					(reinterpret_cast<uintptr_t>(m_curr_element) + (curr_element_alignment - 1)) & ~(curr_element_alignment - 1) );
			}

			const TypeInfo * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:
			VoidPtr m_curr_element;
			const TypeInfo * m_curr_type;
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

			const_iterator(InternalConstructor, VoidPtr i_curr_element, const TypeInfo * i_curr_type) REFLECTIVE_NOEXCEPT
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
				assert(details::is_valid_alignment(curr_element_alignment));
				return reinterpret_cast<value_type*>(
					(reinterpret_cast<uintptr_t>(m_curr_element) + (curr_element_alignment - 1)) & ~(curr_element_alignment - 1) );
			}

			const TypeInfo * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:
			VoidPtr m_curr_element;
			const TypeInfo * m_curr_type;
			friend class DenseList;
		};

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, nullptr, m_buffer + m_size); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		const_iterator end() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_buffer + m_size); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_buffer ); }
		const_iterator cend() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_buffer + m_size); }

		bool operator == (const DenseList & i_source) const
		{
			// this in visual studio generates a warning, because the iterators are not "checked" // return (m_size == i_source.m_size) && std::equal(cbegin(), cend(), i_source.cbegin());
			if (m_size != i_source.m_size)
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
			iterator insert(const_iterator i_position, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(i_position, 1, TypeInfo::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, size_t i_count, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(i_position, i_count, TypeInfo::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator push_back(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(cend(), 1, TypeInfo::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		iterator erase(const_iterator i_position)
		{
			const_iterator to = i_position;
			++to;
			return erase(i_position, to);
		}

		iterator erase(const_iterator i_from, const_iterator i_to)
		{
			const size_t size_to_remove = i_to.m_curr_type - i_from.m_curr_type;

			assert(size_to_remove <= m_size);
			if (size_to_remove == m_size)
			{
				// erasing all the elements
				assert(i_from == cbegin() && i_to == cend());
				clear();
				return begin();
			}
			else if(size_to_remove == 0)
			{
				// erasing 0 new elements...
				return iterator(InternalConstructorMem, const_cast<void*>(i_from.m_curr_element), i_from.m_curr_type);
			}
			else
			{
				size_t buffer_size = 0, buffer_alignment = 0;
				compute_buffer_size_and_alignment_for_erase(&buffer_size, &buffer_alignment, i_from, i_to);

				const TypeInfo * return_type_info = nullptr;
				void * return_element = nullptr;

				ListBuilder builder;
				try
				{
					builder.init(*static_cast<ALLOCATOR*>(this), m_size - size_to_remove, buffer_size, buffer_alignment);
			
					const auto end_it = cend();
					bool is_in_range = false;
					bool first_in_range = false;
					for (auto it = cbegin(); ; it++)
					{						
						if (it == i_from)
						{
							is_in_range = true;
							first_in_range = true;
						}
						if (it == i_to)
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
							auto const new_type_info = builder.end_of_type_infos();
							auto const new_element = builder.add_by_copy(*it.m_curr_type, *it.curr_element());

							if (first_in_range)
							{
								return_type_info = new_type_info;
								return_element = new_element;
								first_in_range = false;
							}
						}
					}

					if (return_type_info == nullptr) // if no elements were copied after the erased range
					{
						assert(i_to == cend());
						return_type_info = builder.end_of_type_infos();
					}

					destroy_impl();

					m_size -= size_to_remove;
					m_buffer = builder.commit();
				}
				catch (...)
				{
					builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
					throw;
				}
				return iterator(InternalConstructorMem, return_element, return_type_info);
			}
		}

		void clear()
		{
			destroy_impl();
			m_buffer = nullptr;
			m_size = 0;
		}

	private:

		ELEMENT * get_elements() const // this function gives a non-const element from a const container, but this avoids duplicating the function
		{
			if (m_size > 0)
			{
				assert(details::is_valid_alignment(m_buffer[0].alignment()));
				const auto first_element_align_mask = m_buffer[0].alignment() - 1;
				auto ptr = reinterpret_cast<uintptr_t>(m_buffer + m_size);
				ptr = (ptr + first_element_align_mask) & ~first_element_align_mask;
				return reinterpret_cast<ELEMENT *>(ptr);
			}
			else
			{
				return nullptr;
			}
		}
				
		class ListBuilder
		{
		public:

			ListBuilder() REFLECTIVE_NOEXCEPT
				: m_type_infos(nullptr)
			{
			}

			void init(ALLOCATOR & i_allocator, size_t i_count, size_t i_buffer_size, size_t i_buffer_alignment)
			{
				void * const buffer = details::aligned_alloc(i_allocator, i_buffer_size, i_buffer_alignment);
				m_end_of_type_infos = m_type_infos = static_cast<TypeInfo*>(buffer);
				m_end_of_elements = m_elements = m_type_infos + i_count;

				#if REFLECTIVE_DENSE_LIST_DEBUG
					m_dbg_end_of_buffer = details::address_add(buffer, i_buffer_size);
				#endif
			}

			void * add_by_copy(const TypeInfo & i_type_info, const ELEMENT & i_source)
			{
				void * return_element = details::address_upper_align(m_end_of_elements, i_type_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(return_element, details::address_add(return_element, i_type_info.size()));
				#endif
				i_type_info.copy_construct(return_element, i_source);
				m_end_of_elements = details::address_add(return_element, i_type_info.size());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_type_infos, m_end_of_type_infos + 1);
				#endif
				new(m_end_of_type_infos++) TypeInfo(i_type_info);

				return return_element;
			}

			void * add_by_move(const TypeInfo & i_type_info, ELEMENT && i_source)
			{
				void * return_element = details::address_upper_align(m_end_of_elements, i_type_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(return_element, details::address_add(return_element, i_type_info.size()));
				#endif
				i_type_info.move_construct_if_no_except(return_element, std::move(i_source));
				m_end_of_elements = details::address_add(return_element, i_type_info.size());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_type_infos, m_end_of_type_infos + 1);
				#endif
				new(m_end_of_type_infos++) TypeInfo(i_type_info);

				return return_element;
			}

			void * add_dont_constuct(const TypeInfo & i_type_info)
			{
				void * return_element = details::address_upper_align(m_end_of_elements, i_type_info.alignment());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(return_element, details::address_add(return_element, i_type_info.size()));
				#endif
				m_end_of_elements = details::address_add(return_element, i_type_info.size());
				#if REFLECTIVE_DENSE_LIST_DEBUG
					dbg_check_range(m_end_of_type_infos, m_end_of_type_infos + 1);
				#endif
				new(m_end_of_type_infos++) TypeInfo(i_type_info);

				return return_element;
			}

			TypeInfo * end_of_type_infos()
			{
				return m_end_of_type_infos;
			}

			TypeInfo * commit()
			{
				return m_type_infos;
			}

			void rollback(ALLOCATOR & i_allocator, size_t i_buffer_size, size_t i_buffer_alignment) REFLECTIVE_NOEXCEPT
			{
				if (m_type_infos != nullptr)
				{
					void * element = m_elements;
					for (TypeInfo * type_info = m_type_infos; type_info < m_end_of_type_infos; type_info++)
					{
						element = details::address_upper_align(m_end_of_elements, type_info->alignment());
						type_info->destroy( static_cast<ELEMENT*>(element));
						element = details::address_add(element, type_info->size());
						type_info->~TypeInfo();
					}
					details::aligned_free(i_allocator, m_type_infos, i_buffer_size, i_buffer_alignment);
				}
			}

		private:
			#if REFLECTIVE_DENSE_LIST_DEBUG
				void dbg_check_range(const void * i_start, const void * i_end)
				{
					assert(i_start >= m_type_infos && i_end <= m_dbg_end_of_buffer );
				}
			#endif

		private:
			TypeInfo * m_type_infos;
			void * m_elements;
			TypeInfo * m_end_of_type_infos;
			void * m_end_of_elements;
			#if REFLECTIVE_DENSE_LIST_DEBUG
				void * m_dbg_end_of_buffer;
			#endif
		};

		void destroy_impl() REFLECTIVE_NOEXCEPT
		{
			size_t dense_alignment = std::alignment_of<TypeInfo>::value;
			const auto end_it = end();
			const size_t dense_size = m_size * sizeof(TypeInfo);
			for (auto it = begin(); it != end_it; ++it)
			{
				auto curr_type = it.curr_type();
				dense_alignment = std::max(dense_alignment, curr_type->alignment() );
				curr_type->destroy(it.curr_element());
				curr_type->TypeInfo::~TypeInfo();
			}
			details::aligned_free( *static_cast<ALLOCATOR*>(this), m_buffer, dense_size, dense_alignment);
		}

		void copy_impl(const DenseList & i_source)
		{
			ListBuilder builder;
			size_t buffer_size = 0, buffer_alignment = 0;
			i_source.compute_buffer_size_and_alignment(&buffer_size, &buffer_alignment);

			try
			{
				builder.init(*static_cast<ALLOCATOR*>(this), i_source.m_size, buffer_size, buffer_alignment);
				auto const end_it = i_source.cend();
				for (auto it = i_source.cbegin(); it != end_it; ++it)
				{
					builder.add_by_copy(*it.m_curr_type, *it.curr_element());
				}

				m_size = i_source.size();
				m_buffer = builder.commit();
			}
			catch (...)
			{
				builder.rollback(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
				throw;
			}
		}

		void move_impl(DenseList && i_source) REFLECTIVE_NOEXCEPT
		{
			m_buffer = i_source.m_buffer;
			m_size = i_source.m_size;
			i_source.m_buffer = nullptr;
			i_source.m_size = 0;
		}

		template <typename... TYPES>
			static void make_impl(DenseList & o_dest_list, TYPES &&... i_args)
		{
			assert(o_dest_list.m_size == 0 && o_dest_list.m_buffer == nullptr); // precondition

			size_t const buffer_size = RecursiveSize<RecursiveHelper<TYPES...>::s_element_count * sizeof(TypeInfo), TYPES...>::s_buffer_size;
			size_t const buffer_alignment = std::max( RecursiveHelper<TYPES...>::s_element_alignment, std::alignment_of<TypeInfo>::value );
			size_t const element_count = RecursiveHelper<TYPES...>::s_element_count;

			ListBuilder builder;
			try
			{
				builder.init(static_cast<ALLOCATOR&>(o_dest_list), element_count, buffer_size, buffer_alignment);

				RecursiveHelper<TYPES...>::construct(builder, std::forward<TYPES>(i_args)...);

				o_dest_list.m_size = element_count;
				o_dest_list.m_buffer = builder.commit();
			}
			catch (...)
			{
				builder.rollback(static_cast<ALLOCATOR&>(o_dest_list), buffer_size, buffer_alignment);
				throw;
			}
			/*void * const buffer = details::aligned_alloc(static_cast<ALLOCATOR&>(o_dest_list), buffer_size, buffer_alignment);
			TypeInfo * const types = static_cast<TypeInfo*>(buffer);
			void * const elements = types + element_count;

			RecursiveHelper<TYPES...>::construct(types, elements, std::forward<TYPES>(i_args)...);
			o_dest_list.m_size = element_count;
			o_dest_list.m_buffer = types;*/

			#ifndef NDEBUG
				size_t dbg_buffer_size = 0, dbg_buffer_alignment = 0;
				o_dest_list.compute_buffer_size_and_alignment(&dbg_buffer_size, &dbg_buffer_alignment);
				assert(dbg_buffer_size == buffer_size);
				assert(dbg_buffer_alignment == buffer_alignment);
			#endif
		}

		void compute_buffer_size_and_alignment(size_t * o_buffer_size, size_t * o_buffer_alignment) const REFLECTIVE_NOEXCEPT
		{
			size_t buffer_size = size() * sizeof(TypeInfo);
			size_t buffer_alignment = std::alignment_of<TypeInfo>::value;
			auto const end_it = cend();
			for (auto it = cbegin(); it != end_it; ++it)
			{
				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_size > 0 && details::is_valid_alignment(curr_alignment));
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;

				buffer_alignment = std::max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		void compute_buffer_size_and_alignment_for_insert(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const const_iterator & i_insert_at, size_t i_new_element_count, const TypeInfo & i_new_type ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_new_type.size() > 0 && details::is_valid_alignment(i_new_type.alignment())); // the size must be non-zero, the alignment must be a non-zero power of 2

			size_t buffer_size = (size() + i_new_element_count) * sizeof(TypeInfo);
			size_t buffer_alignment = std::max(std::alignment_of<TypeInfo>::value, i_new_type.alignment());
			auto const end_it = cend();
			for (auto it = cbegin(); ; ++it)
			{
				if (it == i_insert_at && i_new_element_count > 0)
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
				assert(curr_size > 0 && details::is_valid_alignment(curr_alignment)); // the size must be non-zero, the alignment must be a non-zero power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;
				buffer_alignment = std::max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}
		
		iterator insert_n_impl(const_iterator i_position, size_t i_count_to_insert, const TypeInfo & i_source_type, const ELEMENT & i_source)
		{
			const TypeInfo * return_type_info = nullptr;
			void * return_element = nullptr;
			
			size_t buffer_size = 0, buffer_alignment = 0;
			compute_buffer_size_and_alignment_for_insert(&buffer_size, &buffer_alignment, i_position, i_count_to_insert, i_source_type);

			if (i_count_to_insert > 0)
			{
				ListBuilder builder;				
				try
				{
					builder.init(*static_cast<ALLOCATOR*>(this), m_size + i_count_to_insert, buffer_size, buffer_alignment);
					
					size_t count_to_insert = i_count_to_insert;
					auto const end_it = cend();
					for (auto it = cbegin();;)
					{
						if (it == i_position && count_to_insert > 0)
						{
							auto const end_of_type_infos = builder.end_of_type_infos();
							void * const new_element = builder.add_by_copy(i_source_type, i_source);
							if (count_to_insert == i_count_to_insert)
							{
								return_type_info = end_of_type_infos;
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

					m_size += i_count_to_insert;
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
				// inserting 0 new elements...
				return_type_info = i_position.m_curr_type;
				return_element = const_cast<void*>(i_position.m_curr_element);
			}

			return iterator(InternalConstructorMem, return_element, return_type_info);
		}

		void compute_buffer_size_and_alignment_for_erase(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const const_iterator & i_remove_from, const const_iterator & i_remove_to ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_remove_to.m_curr_type >= i_remove_from.m_curr_type);
			const size_t size_to_remove = i_remove_to.m_curr_type - i_remove_from.m_curr_type;
			assert(size() >= size_to_remove);
			size_t buffer_size = (size() - size_to_remove) * sizeof(TypeInfo);
			size_t buffer_alignment = std::alignment_of<TypeInfo>::value;
			
			bool in_range = false;
			auto const end_it = cend();
			for (auto it = cbegin(); it != end_it; ++it)
			{
				if (it == i_remove_from)
				{
					in_range = true;
				}
				if (it == i_remove_to)
				{
					in_range = false;
				}

				if (!in_range)
				{
					const size_t curr_size = it.curr_type()->size();
					const size_t curr_alignment = it.curr_type()->alignment();
					assert(curr_size > 0 && details::is_valid_alignment(curr_alignment)); // the size must be non-zero, the alignment must be a non-zero power of 2
					buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
					buffer_size += curr_size;
					buffer_alignment = std::max(buffer_alignment, curr_alignment);
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
			{
				void * new_element = i_builder.add_dont_constuct(TypeInfo::template make<FIRST_TYPE>());
				new (new_element) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				RecursiveHelper<OTHER_TYPES...>::construct( i_builder, std::forward<OTHER_TYPES>(i_other_arguments)...);
			}
		};

	private:
		TypeInfo * m_buffer;
		size_t m_size;
	};

	#if REFLECTIVE_ENABLE_TESTING
		void dense_list_test(CorrectnessTestContext & i_context);
	#endif
}