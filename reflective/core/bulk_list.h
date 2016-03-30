
#pragma once

namespace reflective
{
	template <typename ELEMENT> class CopyableTypeInfo;
	template <typename ELEMENT> class MovableTypeInfo;
	template <typename ELEMENT> using DefaultTypeInfo = typename std::conditional< std::is_copy_constructible<ELEMENT>::value,
		CopyableTypeInfo<ELEMENT>, MovableTypeInfo<ELEMENT> >::type;
	template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, typename TYPE_INFO = DefaultTypeInfo<ELEMENT> > class BulkList;

	/** Creates and returns by value a BulkList, whose element base type is ELEMENT. The specified parameters
		are added to the list<br>
		Example: auto int_list = make_bulk_list<int>( 1, 2 * 2, 3 * 3 ); */
	template <typename ELEMENT, typename... TYPES>
		inline BulkList<ELEMENT, std::allocator<ELEMENT>, DefaultTypeInfo<ELEMENT>> make_bulk_list(TYPES &&... i_parameters)
	{
		return BulkList<ELEMENT, std::allocator<ELEMENT>, DefaultTypeInfo<ELEMENT>>::make( std::forward<TYPES>(i_parameters)... );
	}
	template <typename ELEMENT, typename ALLOCATOR, typename... TYPES>
		inline BulkList<ELEMENT, ALLOCATOR, DefaultTypeInfo<ELEMENT>> alloc_bulk_list(const ALLOCATOR & i_allocator, TYPES &&... i_parameters)
	{
		return BulkList<ELEMENT, ALLOCATOR, DefaultTypeInfo<ELEMENT>>::make_with_alloc(i_allocator, std::forward<TYPES>(i_parameters)... );
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

		void move_construct(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
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
		
		void move_construct(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
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
		template <typename ELEMENT>
			inline ELEMENT * address_upper_align(void * i_address) REFLECTIVE_NOEXCEPT
		{
			const size_t alignment_mask = std::alignment_of<ELEMENT>::value - 1;
			return reinterpret_cast<ELEMENT*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
		}

		inline bool is_valid_alignment(size_t i_alignment) REFLECTIVE_NOEXCEPT { return i_alignment > 0 && (i_alignment & (i_alignment - 1)) == 0; }

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

	/** A bulk-list is a sequence container of heterogeneous elements. */
	template <typename ELEMENT, typename ALLOCATOR, typename TYPE_INFO >
		class BulkList : private ALLOCATOR
	{
		enum InternalConstructor { InternalConstructorMem };

	public:

		using allocator_type = ALLOCATOR;
		using value_type = ELEMENT;
		using reference = ELEMENT &;
		using const_reference = const ELEMENT &;
		using pointer = typename std::allocator_traits<allocator_type>::pointer;
		using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
		using difference_type = ptrdiff_t;
		using size_type = size_t;

		template <typename... TYPES>
			inline static BulkList make(TYPES &&... i_args)
		{
			BulkList new_list(InternalConstructorMem);
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		template <typename... TYPES>
			inline static BulkList make_with_alloc(const ALLOCATOR & i_allocator, TYPES &&... i_args)
		{
			BulkList new_list(InternalConstructorMem, i_allocator);
			make_impl(new_list, std::forward<TYPES>(i_args)...);
			return std::move(new_list);
		}

		BulkList() REFLECTIVE_NOEXCEPT
			: m_bulk(nullptr), m_size(0)
				{ }

		BulkList(BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			move_impl(std::move(i_source));
		}

		BulkList & operator = (BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			move_impl(std::move(i_source));
			return *this;
		}

		BulkList(const BulkList & i_source)
		{
			copy_impl(i_source);
		}

		BulkList & operator = (const BulkList & i_source)
		{
			assert(this != &i_source); // self assignment not supported
			destroy_impl();
			copy_impl(i_source);
			return *this;
		}

		~BulkList() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }

		bool empty() const REFLECTIVE_NOEXCEPT  { return m_size == 0; }

		template <typename ELEMENT_TYPE>
			class gen_iterator
		{
		public:

			using iterator_category = std::forward_iterator_tag;
			using value_type = ELEMENT_TYPE;
			using reference = ELEMENT_TYPE &;
			using const_reference = const ELEMENT_TYPE &;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
			using difference_type = ptrdiff_t;
			using size_type = size_t;

			using VoidPtr = typename std::conditional<std::is_const<ELEMENT_TYPE>::value, const void *, void *>::type;

			gen_iterator() REFLECTIVE_NOEXCEPT
				: m_curr_element(nullptr), m_curr_type(nullptr) { }

			gen_iterator(InternalConstructor, VoidPtr i_curr_element, const TYPE_INFO * i_curr_type ) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_curr_element), m_curr_type(i_curr_type)
			{
			}

			gen_iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				VoidPtr const prev_element = curr_element();
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				m_curr_element = reinterpret_cast<ELEMENT_TYPE*>( reinterpret_cast<uintptr_t>(prev_element) + curr_element_size );
				return *this;
			}

			gen_iterator operator++ ( int ) REFLECTIVE_NOEXCEPT
			{
				gen_iterator copy(*this);
				operator ++ ();
				return copy;
			}

			template <typename OTHER_ELEMENT_TYPE>
				bool operator == (const gen_iterator<OTHER_ELEMENT_TYPE> & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type == i_other.m_curr_type;
			}

			template <typename OTHER_ELEMENT_TYPE>
				bool operator != (const gen_iterator<OTHER_ELEMENT_TYPE> & i_other) const REFLECTIVE_NOEXCEPT
			{
				return m_curr_type != i_other.m_curr_type;
			}

			ELEMENT_TYPE & operator * () const	REFLECTIVE_NOEXCEPT { return *curr_element(); }
			ELEMENT_TYPE * operator -> () const	REFLECTIVE_NOEXCEPT { return curr_element(); }

			ELEMENT_TYPE * curr_element() const REFLECTIVE_NOEXCEPT 
			{ 
				auto const curr_element_alignment = m_curr_type->alignment();
				assert(details::is_valid_alignment(curr_element_alignment));
				return reinterpret_cast<ELEMENT_TYPE*>(
					(reinterpret_cast<uintptr_t>(m_curr_element) + (curr_element_alignment - 1)) & ~(curr_element_alignment - 1) );
			}

			const TYPE_INFO * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:
			VoidPtr m_curr_element;
			const TYPE_INFO * m_curr_type;
			friend class BulkList;
		};

		using iterator = gen_iterator<ELEMENT>;
		using const_iterator = gen_iterator<const ELEMENT>;

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, get_elements(), m_bulk ); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, nullptr, m_bulk + m_size); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_bulk ); }
		const_iterator end() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_bulk + m_size); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, get_elements(), m_bulk ); }
		const_iterator cend() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_bulk + m_size); }

		bool operator == (const BulkList & i_source) const
		{
			//return (m_size == i_source.m_size) && std::equal(cbegin(), cend(), i_source.cbegin());
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

		bool operator != (const BulkList & i_source) const		{ return !operator == (i_source); }

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(i_position, 1, TYPE_INFO::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator insert(const_iterator i_position, size_t i_count, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(i_position, i_count, TYPE_INFO::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		template <typename ELEMENT_COMPLETE_TYPE>
			iterator push_back(const ELEMENT_COMPLETE_TYPE & i_source)
		{
			return insert_n_impl(cend(), 1, TYPE_INFO::template make<ELEMENT_COMPLETE_TYPE>(), i_source);
		}

		iterator erase(const_iterator i_position)
		{
			const_iterator to = i_position;
			++to;
			return erase(i_position, to);
		}

		iterator erase(const_iterator i_from, const_iterator i_to)
		{
			size_t buffer_size = 0, buffer_alignment = 0;
			compute_buffer_size_and_alignment_for_erase(&buffer_size, &buffer_alignment, i_from, i_to);
			const size_t size_to_remove = i_to.m_curr_type - i_from.m_curr_type;

			assert(size_to_remove <= m_size);
			if (size_to_remove == m_size)
			{
				assert(i_from == cbegin() && i_to == cend());
				clear();
				return begin();
			}
			else
			{
				BulkList new_list;

				void * const buffer = details::aligned_alloc(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
				TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
				const size_t new_size = m_size - size_to_remove;

				auto curr_element = reinterpret_cast<uintptr_t>(types + new_size);
				TYPE_INFO * curr_type = types;
				TYPE_INFO * return_type_info = nullptr;
				void * return_element = nullptr;

				const auto end_it = cend();
				bool is_range = false;
				for (auto it = cbegin(); it != end_it; it++)
				{
					// upper align curr_element to it.m_curr_type->alignment()
					const uintptr_t element_alignment = it.m_curr_type->alignment();
					assert(details::is_valid_alignment(element_alignment)); // internal check: the alignment must be a power of 2
					auto const alignment_mask = element_alignment - 1;
					curr_element = (curr_element + alignment_mask) & ~alignment_mask;

					if (it == i_from)
					{
						is_range = true;
						return_type_info = curr_type;
						return_element = reinterpret_cast<void*>(curr_element);
					}
					if (it == i_to)
					{
						is_range = false;
					}

					if (!is_range)
					{
						new(curr_type) TYPE_INFO(*it.m_curr_type);
																		
						curr_type->copy_construct(reinterpret_cast<void*>(curr_element), *it);
						
						curr_element += curr_type->size();
						curr_type++;
					}
				}

				// from now on, no exception can be thrown
				destroy_impl();
				m_size = new_size;
				m_bulk = types;

				return iterator(InternalConstructorMem, return_element, return_type_info);
			}
		}

		void clear()
		{
			destroy_impl();
			m_bulk = nullptr;
			m_size = 0;
		}

	private:

		ELEMENT * get_elements() const // this function gives a non-const element from a const container, but this avoids duplicating the function
		{
			if (m_size > 0)
			{
				assert(details::is_valid_alignment(m_bulk[0].alignment()));
				const auto first_element_align_mask = m_bulk[0].alignment() - 1;
				auto ptr = reinterpret_cast<uintptr_t>(m_bulk + m_size);
				ptr = (ptr + first_element_align_mask) & ~first_element_align_mask;
				return reinterpret_cast<ELEMENT *>(ptr);
			}
			else
			{
				return nullptr;
			}
		}
				
		void destroy_impl() REFLECTIVE_NOEXCEPT
		{
			size_t bulk_alignment = std::alignment_of<TYPE_INFO>::value;
			const auto end_it = end();
			const size_t bulk_size = m_size * sizeof(TYPE_INFO);
			for (auto it = begin(); it != end_it; ++it)
			{
				auto curr_type = it.curr_type();
				bulk_alignment = std::max(bulk_alignment, curr_type->alignment() );
				curr_type->destroy(it.curr_element());
				curr_type->TYPE_INFO::~TYPE_INFO();
			}
			details::aligned_free( *static_cast<ALLOCATOR*>(this), m_bulk, bulk_size, bulk_alignment);
		}

		void copy_impl(const BulkList & i_source)
		{
			// allocate the buffer
			size_t buffer_size = 0, buffer_alignment = 0;
			i_source.compute_buffer_size_and_alignment(&buffer_size, &buffer_alignment);
			void * const buffer = details::aligned_alloc(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
			TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
			
			auto curr_element = reinterpret_cast<uintptr_t>( types + i_source.size());
			TYPE_INFO * curr_type = types;
			auto const end_it = i_source.cend();
			for (auto it = i_source.cbegin(); it != end_it; ++it)
			{
				new (curr_type) TYPE_INFO( *(it.curr_type()) );
				
				// upper align curr_element to curr_type->alignment()
				const uintptr_t element_alignment = curr_type->alignment();
				assert(details::is_valid_alignment(element_alignment) ); // internal check: the alignment must be a power of 2
				auto const alignment_mask = element_alignment - 1;
				curr_element = (curr_element + alignment_mask) & ~alignment_mask;

				curr_type->copy_construct(reinterpret_cast<void*>(curr_element), *it);
				
				curr_element += curr_type->size();
				curr_type++;
			}

			m_size = i_source.size();
			m_bulk = types;
		}

		void move_impl(BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			m_bulk = i_source.m_bulk;
			m_size = i_source.m_size;
			i_source.m_bulk = nullptr;
			i_source.m_size = 0;
		}

		template <typename... TYPES>
			static void make_impl(BulkList & o_dest_list, TYPES &&... i_args)
		{
			size_t const buffer_size = RecursiveSize<RecursiveHelper<TYPES...>::s_element_count * sizeof(TYPE_INFO), TYPES...>::s_buffer_size;
			size_t const buffer_alignment = std::max( RecursiveHelper<TYPES...>::s_element_alignment, std::alignment_of<TYPE_INFO>::value );
			size_t const element_count = RecursiveHelper<TYPES...>::s_element_count;

			void * const buffer = details::aligned_alloc(static_cast<ALLOCATOR&>(o_dest_list), buffer_size, buffer_alignment);
			TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
			void * const elements = types + element_count;

			RecursiveHelper<TYPES...>::construct(types, elements, std::forward<TYPES>(i_args)...);
			o_dest_list.m_size = element_count;
			o_dest_list.m_bulk = types;

			#ifndef NDEBUG
				size_t dbg_buffer_size = 0, dbg_buffer_alignment = 0;
				o_dest_list.compute_buffer_size_and_alignment(&dbg_buffer_size, &dbg_buffer_alignment);
				assert(dbg_buffer_size == buffer_size);
				assert(dbg_buffer_alignment == buffer_alignment);
			#endif
		}

		void compute_buffer_size_and_alignment(size_t * o_buffer_size, size_t * o_buffer_alignment) const REFLECTIVE_NOEXCEPT
		{
			size_t buffer_size = size() * sizeof(TYPE_INFO);
			size_t buffer_alignment = std::alignment_of<TYPE_INFO>::value;
			auto const end_it = cend();
			for (auto it = cbegin(); it != end_it; ++it)
			{
				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_size > 0 && details::is_valid_alignment(curr_alignment)); // the alignment must be a power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;

				buffer_alignment = std::max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		void compute_buffer_size_and_alignment_for_insert(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const const_iterator & i_insert_at, size_t i_new_element_count, const TYPE_INFO & i_new_type ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_new_type.size() > 0 && details::is_valid_alignment(i_new_type.alignment())); // the size must be non-zero, the alignment must be a non-zero power of 2

			size_t buffer_size = (size() + i_new_element_count) * sizeof(TYPE_INFO);
			size_t buffer_alignment = std::max(std::alignment_of<TYPE_INFO>::value, i_new_type.alignment());
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
		
		iterator insert_n_impl(const_iterator i_position, size_t i_count, const TYPE_INFO & i_source_type, const ELEMENT & i_source)
		{
			const TYPE_INFO * new_type_info = nullptr;
			void * new_element = nullptr;

			if (i_count > 0)
			{
				size_t buffer_size = 0, buffer_alignment = 0;
				compute_buffer_size_and_alignment_for_insert(&buffer_size, &buffer_alignment, i_position, i_count, i_source_type );

				BulkList new_list;

				void * const buffer = details::aligned_alloc(*static_cast<ALLOCATOR*>(this), buffer_size, buffer_alignment);
				TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);

				size_t count_to_insert = i_count;

				auto curr_element = reinterpret_cast<uintptr_t>(types + size() + i_count);
				TYPE_INFO * curr_type = types;
				auto const end_it = cend();
				for (auto it = cbegin(); ; )
				{
					const bool iterator_match = it == i_position;

					const ELEMENT * copy_source = nullptr;
					if (iterator_match && count_to_insert > 0)
					{
						new(curr_type) TYPE_INFO(i_source_type);
						copy_source = &i_source;
						count_to_insert--;
					}
					else
					{
						if (it == end_it)
						{
							break;
						}

						new (curr_type) TYPE_INFO(*(it.curr_type()));
						copy_source = it.curr_element();
						++it;
					}

					// upper align curr_element to curr_type->alignment()
					const uintptr_t element_alignment = curr_type->alignment();
					assert(details::is_valid_alignment(element_alignment)); // internal check: the alignment must be a power of 2
					auto const alignment_mask = element_alignment - 1;
					curr_element = (curr_element + alignment_mask) & ~alignment_mask;

					if (iterator_match && new_type_info == nullptr)
					{
						new_type_info = curr_type;
						new_element = reinterpret_cast<ELEMENT*>(curr_element);
					}

					curr_type->copy_construct(reinterpret_cast<void*>(curr_element), *copy_source);

					curr_element += curr_type->size();
					curr_type++;
				}

				// from now on, no exception can be thrown
				destroy_impl();
				m_size = m_size + i_count;
				m_bulk = types;
			}
			else
			{
				new_type_info = i_position.m_curr_type;
				new_element = const_cast<void*>(i_position.m_curr_element);
			}
			return iterator(InternalConstructorMem, new_element, new_type_info);
		}

		void compute_buffer_size_and_alignment_for_erase(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const const_iterator & i_remove_from, const const_iterator & i_remove_to ) const REFLECTIVE_NOEXCEPT
		{
			assert(i_remove_to.m_curr_type >= i_remove_from.m_curr_type);
			const size_t size_to_remove = i_remove_to.m_curr_type - i_remove_from.m_curr_type;
			assert(size() >= size_to_remove);
			size_t buffer_size = (size() - size_to_remove) * sizeof(TYPE_INFO);
			size_t buffer_alignment = std::alignment_of<TYPE_INFO>::value;
			
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

		BulkList(InternalConstructor)	{ 	}

		BulkList(InternalConstructor, const ALLOCATOR & i_allocator) : ALLOCATOR(i_allocator) {}

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
			static void construct(TYPE_INFO * /*i_types*/, void * /*i_elements*/, TYPES &&...) { }
		};
		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveHelper<FIRST_TYPE, OTHER_TYPES...>
		{
			static_assert(std::alignment_of<FIRST_TYPE>::value > 0 && (std::alignment_of<FIRST_TYPE>::value & (std::alignment_of<FIRST_TYPE>::value - 1)) == 0,
				"the alignment must be a non-zero integer power of 2" );

			static_assert(std::is_base_of<ELEMENT, FIRST_TYPE >::value, "Trying to initiaize a bulk list with an object whose type is not a subtype of the base element type");

			static const size_t s_element_count = 1 + RecursiveHelper<OTHER_TYPES...>::s_element_count;
			static const size_t s_element_alignment = std::alignment_of<FIRST_TYPE>::value > RecursiveHelper<OTHER_TYPES...>::s_element_alignment ?
				std::alignment_of<FIRST_TYPE>::value : RecursiveHelper<OTHER_TYPES...>::s_element_alignment;

			inline static void construct(TYPE_INFO * i_types, void * i_elements, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				new(i_types) TYPE_INFO(TYPE_INFO::template make<FIRST_TYPE>());
				FIRST_TYPE * const new_element = new(details::address_upper_align<FIRST_TYPE>(i_elements)) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				RecursiveHelper<OTHER_TYPES...>::construct(
					i_types + 1, new_element + 1, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

	private:
		TYPE_INFO * m_bulk;
		size_t m_size;
	};

	#if REFLECTIVE_ENABLE_TESTING
		void bulk_list_test(CorrectnessTestContext & i_context);
	#endif
}