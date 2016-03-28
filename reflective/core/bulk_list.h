
#pragma once

namespace reflective
{
	template <typename ELEMENT> class DefaultTypeInfo;
	template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, typename TYPE_INFO = DefaultTypeInfo<ELEMENT> > class BulkList;

	/** Creates and returns by value a BulkList, whose element base type is ELEMENT. The specified parameters
		are added to the list<br>
		Example: auto int_list = make_bulk_list<int>( 1, 2 * 2, 3 * 3 ); */
	template <typename ELEMENT, typename... TYPES>
		inline BulkList<ELEMENT, std::allocator<ELEMENT>, DefaultTypeInfo<ELEMENT>> make_bulk_list(TYPES &&... i_parameters)
	{
		return BulkList<ELEMENT, std::allocator<ELEMENT>, DefaultTypeInfo<ELEMENT>>::make( std::forward<TYPES>(i_parameters)... );
	}
	/*template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, typename TYPE_INFO = DefaultTypeInfo<ELEMENT>, typename... TYPES>
		inline BulkList<ELEMENT, ALLOCATOR, TYPE_INFO> allocate_bulk_list(ALLOCATOR && i_allocator, TYPES &&... i_parameters)
	{
		return BulkList<ELEMENT, ALLOCATOR, TYPE_INFO>::make_alloc(i_allocator, std::forward<TYPES>(i_parameters)...);
	}*/

	namespace details
	{
		template <typename ELEMENT>
			ELEMENT * address_upper_align(void * i_address) REFLECTIVE_NOEXCEPT
		{
			const size_t alignment_mask = std::alignment_of<ELEMENT>::value - 1;
			return reinterpret_cast<ELEMENT*>((reinterpret_cast<uintptr_t>(i_address) + alignment_mask) & ~alignment_mask);
		}
	}

	template <typename ELEMENT>
		class DefaultTypeInfo
	{
	public:

		DefaultTypeInfo() = delete;
		DefaultTypeInfo(const DefaultTypeInfo &)= delete;
		DefaultTypeInfo & operator = (const DefaultTypeInfo &) = delete;
		DefaultTypeInfo & operator = (DefaultTypeInfo &&) = delete;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			DefaultTypeInfo(DefaultTypeInfo && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
					{ }			
		#else
			DefaultTypeInfo(DefaultTypeInfo && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static DefaultTypeInfo make()
		{
			return DefaultTypeInfo(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, 
				&CopyConstructImpl< COMPLETE_TYPE >, &MoverDestructorImpl< COMPLETE_TYPE >);
		}

		size_t size() const { return m_size; }
		
		size_t alignment() const { return m_alignment; }
		
		void destroy(ELEMENT * i_element) const
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

		void copy(void * i_destination, const ELEMENT & i_source_element) const
		{
			(*m_copy_constructor)(i_destination, i_source_element);
		}

		void move_and_destroy(void * i_destination, ELEMENT * i_source_element) const
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

	private:

		using CopyConstructorPtr = void(*)(void * i_dest_element, const ELEMENT & i_source_element);
		using MoverDestructorPtr = void (*)(void * i_dest_element, ELEMENT * i_source_element);

		DefaultTypeInfo(size_t i_size, size_t i_alignment, CopyConstructorPtr i_copy_constructor, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_copy_constructor(i_copy_constructor), m_mover_destructor(i_mover_destructor)
				{ }

		template <typename COMPLETE_TYPE>
			static void CopyConstructImpl(void * /*i_destination*/, const ELEMENT & /*i_source_element*/)
		{
			// TO DO: detect if the type is copyable
			//new (i_destination) COMPLETE_TYPE(static_cast<const COMPLETE_TYPE&>(i_source_element));
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

	/** A bulk-list is a sequence container of heterogeneous elements. */
	template <typename ELEMENT, typename ALLOCATOR, typename TYPE_INFO >
		class BulkList : private ALLOCATOR
	{
		enum InternalConstructor { InternalConstructorMem };

		void destroy_impl() REFLECTIVE_NOEXCEPT
		{
			size_t bulk_alignment = std::alignment_of<TYPE_INFO>::value;
			const auto end_it = end();
			for (auto it = begin(); it != end_it; it++)
			{
				auto curr_type = it.curr_type();
				bulk_alignment = std::max(bulk_alignment, curr_type->alignment() );
				curr_type->destroy(it.curr_element());
				curr_type->TYPE_INFO::~TYPE_INFO();
			}
			free_mem(m_bulk, bulk_alignment);
		}

		void move_impl(BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			m_bulk = i_source.m_bulk;
			m_size = i_source.m_size;
			i_source.m_bulk = nullptr;
			i_source.m_size = 0;
		}

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
			inline static BulkList make(TYPES &&... i_tuple)
		{
			size_t const buffer_size = RecursiveSize<RecursiveHelper<TYPES...>::s_element_count * sizeof(TYPE_INFO), TYPES...>::s_buffer_size;
			size_t const buffer_alignment = std::max( RecursiveHelper<TYPES...>::s_element_alignment, std::alignment_of<TYPE_INFO>::value );
			size_t const element_count = RecursiveHelper<TYPES...>::s_element_count;

			BulkList new_list(InternalConstructorMem);
			void * const buffer = new_list.alloc_mem(buffer_size, buffer_alignment);
			TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
			void * const elements = types + element_count;

			RecursiveHelper<TYPES...>::construct(types, elements, std::forward<TYPES>(i_tuple)...);

			new_list.m_size = element_count;
			new_list.m_bulk = types;
			return std::move(new_list);
		}

		BulkList() REFLECTIVE_NOEXCEPT
			: m_size(0), m_bulk(nullptr)
				{ }

		BulkList(BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			move_impl(std::move(i_source));
		}

		BulkList & operator = (BulkList && i_source) REFLECTIVE_NOEXCEPT
		{
			assert(this != &i_source); // self move asignment gives undefined behaviour
			destroy_impl();
			move_impl(std::move(i_source));
			return *this;
		}

		/*BulkList(const BulkList & i_source)
			: m_size()
		{
			
		}

		BulkList & operator = (const BulkList & i_source)
		{
			return *this;
		}*/

		~BulkList() REFLECTIVE_NOEXCEPT
		{
			destroy_impl();
		}

		size_t size() const { return m_size; }

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

			iterator() REFLECTIVE_NOEXCEPT
				: m_curr_element(nullptr), m_curr_type(nullptr), m_end_type(nullptr){}

			iterator(InternalConstructor, ELEMENT * i_curr_element, TYPE_INFO * i_curr_type, TYPE_INFO * i_end_type) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_curr_element), m_curr_type(i_curr_type), m_end_type(i_end_type)
			{
			}

			iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				// m_curr_element is advanced by the size of the current element, and upper aligned to the next element. m_curr_type is just incremented
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				if (m_curr_type < m_end_type)
				{
					auto const next_element_alignment = m_curr_type->alignment();
					assert( (next_element_alignment & (next_element_alignment >> 1)) == 0); // internal check: the alignment must be a power of 2
					auto const alignment_mask = next_element_alignment - 1;
					m_curr_element = reinterpret_cast<ELEMENT*>((reinterpret_cast<uintptr_t>(m_curr_element)+curr_element_size + alignment_mask) & ~alignment_mask);
				}
				else
				{
					m_curr_element = nullptr;
				}
				return *this;
			}

			iterator operator++ ( int ) REFLECTIVE_NOEXCEPT
			{
				iterator copy(*this);
				operator ++ ();
				return copy;
			}

			bool operator == (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				assert((m_curr_element == i_other.m_curr_element) == (m_curr_type == i_other.m_curr_type)); // internal check: m_curr_type and m_curr_element must be consistent
				return m_curr_element == i_other.m_curr_element;
			}

			bool operator != (const iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				assert((m_curr_element == i_other.m_curr_element) == (m_curr_type == i_other.m_curr_type) ); // internal check: m_curr_type and m_curr_element must be consistent
				return m_curr_element != i_other.m_curr_element;
			}

			ELEMENT & operator * () const	REFLECTIVE_NOEXCEPT { return *m_curr_element; }
			ELEMENT * operator -> () const	REFLECTIVE_NOEXCEPT { return m_curr_element; }

			ELEMENT * curr_element() const REFLECTIVE_NOEXCEPT { return m_curr_element; }
			TYPE_INFO * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:			
			ELEMENT * m_curr_element;
			TYPE_INFO * m_curr_type;
			TYPE_INFO * m_end_type;
		};

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, reinterpret_cast<ELEMENT*>(m_bulk + m_size), m_bulk, m_bulk + m_size); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, nullptr, m_bulk + m_size, m_bulk + m_size); }

	private:

		void * alloc_mem(size_t i_size, size_t i_alignment)
		{
			return _aligned_malloc(i_size, i_alignment);
		}

		void free_mem(void * i_block, size_t /*i_alignment*/) REFLECTIVE_NOEXCEPT
		{
			_aligned_free(i_block);
		}

	private:
		

		BulkList(InternalConstructor)
		{

		}

		BulkList(InternalConstructor, const ALLOCATOR & i_allocator)
			: ALLOCATOR(i_allocator)
		{

		}

		// RecursiveSize< PREV_ELEMENTS_SIZE, OTHER_TYPES...>::s_buffer_size - space required to layout the specified types, respecting the alignemnt
		template <size_t PREV_ELEMENTS_SIZE, typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveSize
		{
			// s_this_size = sizeof(FIRST_TYPE) + upper_align(PREV_ELEMENTS_SIZE, alignof(FIRST_TYPE))
			static const size_t s_this_size = sizeof(FIRST_TYPE) + (PREV_ELEMENTS_SIZE + std::alignment_of<FIRST_TYPE>::value - 1)
				- (((PREV_ELEMENTS_SIZE + std::alignment_of<FIRST_TYPE>::value - 1)) % std::alignment_of<FIRST_TYPE>::value);
			static const size_t s_buffer_size = s_this_size + RecursiveSize<PREV_ELEMENTS_SIZE + s_this_size, OTHER_TYPES...>::s_buffer_size;
		};
		template <size_t PREV_ELEMENTS_SIZE, typename LAST_TYPE>
			struct RecursiveSize<PREV_ELEMENTS_SIZE, LAST_TYPE>
		{
			static const size_t s_buffer_size = (std::alignment_of<LAST_TYPE>::value - (PREV_ELEMENTS_SIZE % std::alignment_of<LAST_TYPE>::value)) + sizeof(LAST_TYPE);
		};


		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveHelper
		{
			static const size_t s_element_count = 1 + RecursiveHelper<OTHER_TYPES...>::s_element_count;
			static const size_t s_element_alignment = std::alignment_of<FIRST_TYPE>::value > RecursiveHelper<OTHER_TYPES...>::s_element_alignment ?
				std::alignment_of<FIRST_TYPE>::value : RecursiveHelper<OTHER_TYPES...>::s_element_alignment;

			inline static void construct(TYPE_INFO * i_types, void * i_elements, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				new(i_types) TYPE_INFO( TYPE_INFO::template make<FIRST_TYPE>() );
				FIRST_TYPE * const new_element = new(details::address_upper_align<FIRST_TYPE>(i_elements)) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				RecursiveHelper<OTHER_TYPES...>::construct(
					i_types + 1, new_element + 1, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename LAST_TYPE>
			struct RecursiveHelper<LAST_TYPE>
		{
			static const size_t s_element_count = 1;
			static const size_t s_element_alignment = std::alignment_of<LAST_TYPE>::value;

			inline static void construct(TYPE_INFO * i_types, void * i_elements, LAST_TYPE && i_source)
			{
				new(i_types) TYPE_INFO(TYPE_INFO::template make<LAST_TYPE>());
				new(details::address_upper_align<LAST_TYPE>(i_elements)) LAST_TYPE(std::forward<LAST_TYPE>(i_source));
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