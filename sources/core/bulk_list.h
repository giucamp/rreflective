
namespace reflective
{
	template <typename TYPE>
		TYPE * address_upper_align(void * i_address)
	{
		const size_t alignmentMask = std::alignment_of<TYPE>::value - 1;
		return reinterpret_cast<TYPE*>((reinterpret_cast<uintptr_t>(i_address) + alignmentMask) & ~alignmentMask);
	}

	/*template <typename OTHER_TYPE, typename... CONSTR_PARAMS>
		inline OTHER_TYPE * linear_alloc(void * & io_address, CONSTR_PARAMS && ...i_params)
	{
		OTHER_TYPE * result = new(address_upper_align<OTHER_TYPE>(io_address)) OTHER_TYPE(std::forward<CONSTR_PARAMS>(i_params)...);
		io_address = result + 1;
		return result;
	}*/

	template <typename TYPE>
		class DefaultTypeInfo
	{
	public:

		DefaultTypeInfo() = delete;
		DefaultTypeInfo(const DefaultTypeInfo &) = delete;
		DefaultTypeInfo & operator = (const DefaultTypeInfo &) = delete;
		DefaultTypeInfo & operator = (DefaultTypeInfo &&) = delete;

		DefaultTypeInfo(DefaultTypeInfo && i_source)
			: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
		{

		}		

		template <typename COMPLETE_TYPE>
			static DefaultTypeInfo make()
		{
			return DefaultTypeInfo(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &MoverDestructorImpl < COMPLETE_TYPE >);
		}

		size_t size() const { return m_size; }
		
		size_t alignment() const { return m_alignment; }
		
		void destroy(TYPE * i_element)
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

		void move_and_destroy(void * i_destination, TYPE * i_source_element)
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

	private:

		using MoverDestructorPtr = void (*)(void * i_dest_element, TYPE * i_source_element);

		DefaultTypeInfo(size_t i_size, size_t i_alignment, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_mover_destructor(i_mover_destructor)
				{ }
		
		template <typename COMPLETE_TYPE>
			static void MoverDestructorImpl(void * i_destination, TYPE * i_source_element)
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

	template <typename TYPE, typename TYPE_INFO = DefaultTypeInfo<TYPE>, typename ALLOCATOR = std::allocator<TYPE> >
		class BulkList : private ALLOCATOR
	{
	public:

		template <typename... TYPES>
			inline static BulkList make(TYPES... i_tuple)
		{
			size_t const buffer_size = RecursiveSize<0, TYPES...>::s_buffer_size;
			size_t const buffer_alignment = RecursiveHelper<TYPES...>::s_item_alignment;
			size_t const item_count = RecursiveHelper<TYPES...>::s_item_count;

			BulkList new_list(InternalConstructorMemb);

			TYPE_INFO * const types = new_list.alloc_type_info_array(item_count);
			void * const items = new_list.Alloc(buffer_size, buffer_alignment);
			RecursiveHelper<TYPES...>::construct(types, items, std::forward<TYPES>(i_tuple)...);

			new_list.m_size = item_count;
			new_list.m_items = static_cast<TYPE*>( items );
			new_list.m_types = types;

			return std::move(new_list);
		}

		class iterator
		{
		public:

			iterator(BulkList & i_source)
				: m_curr_type(i_source.m_types), m_curr_item(i_source.m_items)
			{

			}

			iterator & operator ++ ()
			{
				/*size_t const curr_size = m_curr_type->size();
				m_curr_type = reinterpret_cast<uintptr_t>(m_curr_type) + curr_size;
				return reinterpret_cast<TYPE*>((reinterpret_cast<uintptr_t>(i_address) + alignmentMask) & ~alignmentMask);*/
				return *this;
			}

		private:
			TYPE_INFO * m_curr_type;
			TYPE * m_curr_item;
		};

	private:

		TYPE * get_items()
		{
			return m_items;
		}


		static void * Alloc(size_t i_size, size_t i_alignment)
		{
			return _aligned_malloc(i_size, i_alignment);
		}

		static void * Free(void * i_block)
		{
			return _aligned_free(i_block);
		}

		TYPE_INFO * alloc_type_info_array(size_t i_count)
		{
			using Allocator = typename std::allocator_traits<ALLOCATOR>::rebind_alloc<TYPE_INFO>;
			Allocator alloc(*static_cast<const ALLOCATOR*>(this));
			TYPE_INFO * new_array = alloc.allocate(i_count);
			return new_array;
		}

		void * delete_type_info_array(TYPE_INFO * i_array)
		{
			using Allocator = typename std::allocator_traits<ALLOCATOR>::rebind_alloc<TYPE_INFO>;
			Allocator alloc( *static_cast<const ALLOCATOR*>(this) );
			alloc.deallocate(i_array);
		}

	private:

		enum InternalConstructor { InternalConstructorMemb };

		BulkList(InternalConstructor)
		{

		}

		BulkList(InternalConstructor, const ALLOCATOR & i_allocator)
			: ALLOCATOR(i_allocator)
		{

		}

		/** RecursiveSize< PREV_ELEMENTS_SIZE, OTHER_TYPES...>::s_buffer_size - space required to layout the specified types, respecting he alignemnt */
		template <size_t PREV_ELEMENTS_SIZE, typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveSize
		{
			static const size_t s_this_size = sizeof(FIRST_TYPE) + (std::alignment_of<FIRST_TYPE>::value - (PREV_ELEMENTS_SIZE % std::alignment_of<FIRST_TYPE>::value));
			static const size_t s_buffer_size = s_this_size + RecursiveSize<PREV_ELEMENTS_SIZE + PREV_ELEMENTS_SIZE, OTHER_TYPES...>::s_buffer_size;
		};
		template <size_t PREV_ELEMENTS_SIZE, typename LAST_TYPE>
			struct RecursiveSize<PREV_ELEMENTS_SIZE, LAST_TYPE>
		{
			static const size_t s_buffer_size = (std::alignment_of<LAST_TYPE>::value - (PREV_ELEMENTS_SIZE % std::alignment_of<LAST_TYPE>::value)) + sizeof(LAST_TYPE);
		};


		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveHelper
		{
			static const size_t s_item_count = 1 + RecursiveHelper<OTHER_TYPES...>::s_item_count;
			static const size_t s_item_alignment = std::alignment_of<FIRST_TYPE>::value > RecursiveHelper<OTHER_TYPES...>::s_item_alignment ?
				std::alignment_of<FIRST_TYPE>::value : RecursiveHelper<OTHER_TYPES...>::s_item_alignment;

			inline static void construct(TYPE_INFO * i_types, void * i_items, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				new(i_types) TYPE_INFO( TYPE_INFO::make<FIRST_TYPE>() );
				FIRST_TYPE * const new_item = new(address_upper_align<FIRST_TYPE>(i_items)) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				RecursiveHelper<OTHER_TYPES...>::construct(
					i_types + 1, new_item + 1, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename LAST_TYPE>
			struct RecursiveHelper<LAST_TYPE>
		{
			static const size_t s_item_count = 1;
			static const size_t s_item_alignment = std::alignment_of<LAST_TYPE>::value;

			inline static void construct(TYPE_INFO * i_types, void * i_items, LAST_TYPE && i_source)
			{
				new(i_types) TYPE_INFO(TYPE_INFO::make<LAST_TYPE>());
				new(address_upper_align<LAST_TYPE>(i_items)) LAST_TYPE(std::forward<LAST_TYPE>(i_source));
			}
		};

	private:
		size_t m_size;
		TYPE * m_items;
		TYPE_INFO * m_types;
	};
}