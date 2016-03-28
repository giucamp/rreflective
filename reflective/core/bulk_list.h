
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
			: m_size(0), m_bulk(nullptr)
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

		size_t size() const { return m_size; }

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

			gen_iterator() REFLECTIVE_NOEXCEPT
				: m_curr_element(nullptr), m_curr_type(nullptr), m_end_type(nullptr){}

			gen_iterator(InternalConstructor, ELEMENT_TYPE * i_curr_element, TYPE_INFO * i_curr_type, TYPE_INFO * i_end_type) REFLECTIVE_NOEXCEPT
				: m_curr_element(i_curr_element), m_curr_type(i_curr_type), m_end_type(i_end_type)
			{
			}

			gen_iterator & operator ++ () REFLECTIVE_NOEXCEPT
			{
				// m_curr_element is advanced by the size of the current element, and upper aligned to the next element. m_curr_type is just incremented
				auto const curr_element_size = m_curr_type->size();
				m_curr_type++;
				if (m_curr_type < m_end_type)
				{
					auto const next_element_alignment = m_curr_type->alignment();
					assert( (next_element_alignment & (next_element_alignment >> 1)) == 0); // internal check: the alignment must be a power of 2
					auto const alignment_mask = next_element_alignment - 1;
					m_curr_element = reinterpret_cast<ELEMENT_TYPE*>((reinterpret_cast<uintptr_t>(m_curr_element)+curr_element_size + alignment_mask) & ~alignment_mask);
				}
				else
				{
					m_curr_element = nullptr;
				}
				return *this;
			}

			gen_iterator operator++ ( int ) REFLECTIVE_NOEXCEPT
			{
				gen_iterator copy(*this);
				operator ++ ();
				return copy;
			}

			bool operator == (const gen_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				assert((m_curr_element == i_other.m_curr_element) == (m_curr_type == i_other.m_curr_type)); // internal check: m_curr_type and m_curr_element must be consistent
				return m_curr_element == i_other.m_curr_element;
			}

			bool operator != (const gen_iterator & i_other) const REFLECTIVE_NOEXCEPT
			{
				assert((m_curr_element == i_other.m_curr_element) == (m_curr_type == i_other.m_curr_type) ); // internal check: m_curr_type and m_curr_element must be consistent
				return m_curr_element != i_other.m_curr_element;
			}

			ELEMENT_TYPE & operator * () const	REFLECTIVE_NOEXCEPT { return *m_curr_element; }
			ELEMENT_TYPE * operator -> () const	REFLECTIVE_NOEXCEPT { return m_curr_element; }

			ELEMENT_TYPE * curr_element() const REFLECTIVE_NOEXCEPT { return m_curr_element; }
			TYPE_INFO * curr_type() const REFLECTIVE_NOEXCEPT { return m_curr_type; }

		private:			
			ELEMENT_TYPE * m_curr_element;
			TYPE_INFO * m_curr_type;
			TYPE_INFO * m_end_type;
		};

		using iterator = gen_iterator<ELEMENT>;
		using const_iterator = gen_iterator<const ELEMENT>;

		iterator begin() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, 
			m_size > 0 ? reinterpret_cast<ELEMENT*>(m_bulk + m_size) : nullptr, m_bulk, m_bulk + m_size); }
		iterator end() REFLECTIVE_NOEXCEPT { return iterator(InternalConstructorMem, nullptr, m_bulk + m_size, m_bulk + m_size); }

		const_iterator begin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem,
				m_size > 0 ? reinterpret_cast<const ELEMENT*>(m_bulk + m_size) : nullptr, m_bulk, m_bulk + m_size); }

		const_iterator end() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_bulk + m_size, m_bulk + m_size); }

		const_iterator cbegin() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem,
				m_size > 0 ? reinterpret_cast<const ELEMENT*>(m_bulk + m_size) : nullptr, m_bulk, m_bulk + m_size); }

		const_iterator cend() const REFLECTIVE_NOEXCEPT { return const_iterator(InternalConstructorMem, nullptr, m_bulk + m_size, m_bulk + m_size); }

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
			iterator insert(const_iterator i_position, size_t i_count, const ELEMENT_COMPLETE_TYPE & i_source)
		{
			size_t buffer_size = 0, buffer_alignment = 0;
			compute_buffer_size_and_alignment_for_insert(&buffer_size, &buffer_alignment, i_position, i_count,
				sizeof(ELEMENT_COMPLETE_TYPE), std::alignment_of<ELEMENT_COMPLETE_TYPE>::value );

			BulkList new_list;
			
			void * const buffer = alloc_mem(buffer_size, buffer_alignment);
			TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
			
			size_t count_to_insert = i_count;

			auto curr_element = reinterpret_cast<uintptr_t>(types + size() + i_count);
			TYPE_INFO * curr_type = types;
			auto const end_it = cend();
			for (auto it = cbegin(); ; )
			{
				const ELEMENT * copy_source;
				if (it == i_position && count_to_insert > 0)
				{
					new(curr_type) TYPE_INFO(TYPE_INFO::template make<ELEMENT_COMPLETE_TYPE>());

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
					
					copy_source = &*it;

					it++;
				}
				
				// upper align curr_element to curr_type->alignment()
				const uintptr_t element_alignment = curr_type->alignment();
				assert(element_alignment > 0 && (element_alignment & (element_alignment >> 1)) == 0); // internal check: the alignment must be a power of 2
				auto const alignment_mask = element_alignment - 1;
				curr_element = (curr_element + alignment_mask) & ~alignment_mask;

				curr_type->copy_construct(reinterpret_cast<void*>(curr_element), *copy_source);

				curr_element += curr_type->size();
				curr_type++;
			}

			destroy_impl();
			m_size = m_size + i_count;
			m_bulk = types;

			return end(); // temp - this is wrong
		}

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

		void copy_impl(const BulkList & i_source)
		{
			// allocate the buffer
			size_t buffer_size = 0, buffer_alignment = 0;
			i_source.compute_buffer_size_and_alignment(&buffer_size, &buffer_alignment);
			void * const buffer = alloc_mem(buffer_size, buffer_alignment);
			TYPE_INFO * const types = static_cast<TYPE_INFO*>(buffer);
			
			auto curr_element = reinterpret_cast<uintptr_t>( types + i_source.size());
			TYPE_INFO * curr_type = types;
			auto const end_it = i_source.cend();
			for (auto it = i_source.cbegin(); it != end_it; it++)
			{
				new (curr_type) TYPE_INFO( *(it.curr_type()) );
				
				// upper align curr_element to curr_type->alignment()
				const uintptr_t element_alignment = curr_type->alignment();
				assert(element_alignment > 0 && (element_alignment & (element_alignment >> 1)) == 0); // internal check: the alignment must be a power of 2
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

			void * const buffer = o_dest_list.alloc_mem(buffer_size, buffer_alignment);
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
			for (auto it = cbegin(); it != end_it; it++)
			{
				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_alignment > 0 && (curr_alignment & (curr_alignment - 1)) == 0); // the alignment must be a power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;

				buffer_alignment = std::max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}

		void compute_buffer_size_and_alignment_for_insert(size_t * o_buffer_size, size_t * o_buffer_alignment,
			const const_iterator & i_insert_at, size_t i_new_element_count,
			size_t i_new_element_size, size_t i_new_element_alignment ) const REFLECTIVE_NOEXCEPT
		{
			size_t buffer_size = (size() + i_new_element_count) * sizeof(TYPE_INFO);
			size_t buffer_alignment = std::max(std::alignment_of<TYPE_INFO>::value, i_new_element_alignment);
			auto const end_it = cend();
			for (auto it = cbegin(); ; it++)
			{
				if (it == i_insert_at)
				{
					assert(i_new_element_size > 0 && i_new_element_alignment > 0 && (i_new_element_alignment & (i_new_element_alignment - 1)) == 0); // the size must be non-zero, the alignment must be a non-zero power of 2
					buffer_size = (buffer_size + (i_new_element_alignment - 1)) & ~(i_new_element_alignment - 1);
					buffer_size += i_new_element_size * i_new_element_count;
				}

				if (it == end_it)
				{
					break;
				}

				const size_t curr_size = it.curr_type()->size();
				const size_t curr_alignment = it.curr_type()->alignment();
				assert(curr_size > 0 && curr_alignment > 0 && (curr_alignment & (curr_alignment - 1)) == 0); // the size must be non-zero, the alignment must be a non-zero power of 2
				buffer_size = (buffer_size + (curr_alignment - 1)) & ~(curr_alignment - 1);
				buffer_size += curr_size;
				buffer_alignment = std::max(buffer_alignment, curr_alignment);
			}

			*o_buffer_size = buffer_size;
			*o_buffer_alignment = buffer_alignment;
		}
		

		BulkList(InternalConstructor)	{ 	}

		BulkList(InternalConstructor, const ALLOCATOR & i_allocator) : ALLOCATOR(i_allocator)
		{

		}

		template <size_t PREV_ELEMENTS_SIZE, typename...> struct RecursiveSize
		{
			static const size_t s_buffer_size = PREV_ELEMENTS_SIZE;
		};
		template <size_t PREV_ELEMENTS_SIZE, typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveSize<PREV_ELEMENTS_SIZE, FIRST_TYPE, OTHER_TYPES...>
		{
			static const size_t s_aligned_prev_size = ( PREV_ELEMENTS_SIZE + (std::alignment_of<FIRST_TYPE>::value - 1) ) & ~(std::alignment_of<FIRST_TYPE>::value - 1);
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