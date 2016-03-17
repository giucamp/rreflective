
namespace reflective
{
	template <typename TYPE>
		TYPE * address_upper_align(void * i_address)
	{
		const size_t alignmentMask = std::alignment_of<TYPE>::value - 1;
		return reinterpret_cast<TYPE*>((reinterpret_cast<uintptr_t>(i_address) + alignmentMask) & ~alignmentMask);
	}

	template <typename OTHER_TYPE, typename... CONSTR_PARAMS>
		inline OTHER_TYPE * linear_alloc(void * & io_address, CONSTR_PARAMS && ...i_params)
	{
		OTHER_TYPE * result = new(address_upper_align<OTHER_TYPE>(io_address)) OTHER_TYPE(std::forward<CONSTR_PARAMS>(i_params)...);
		io_address = result + 1;
		return result;
	}

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
				new (i_destination)COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_source_element)));
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
			const size_t buffer_size = RecursiveSize<0, TYPES...>::s_buffer_size;
			const size_t buffer_alignment = RecursiveHelper<TYPES...>::s_item_alignment;
			const size_t item_count = RecursiveHelper<TYPES...>::s_item_count;

			void * dictionary = Alloc(item_count * sizeof(TYPE*) + item_count * sizeof(TYPE_INFO), 8);
			TYPE * * item_dictionary = static_cast<TYPE * *>(dictionary);
			TYPE_INFO * type_dictionary = reinterpret_cast<TYPE_INFO *>(item_dictionary + item_count);
			
			void * items = Alloc(buffer_size, buffer_alignment);
			RecursiveHelper<TYPES...>::construct(item_dictionary, type_dictionary, items, std::forward<TYPES>(i_tuple)...);

			return BulkList(InternalConstructor(), item_count, dictionary, items );
		}

		template <typename TYPE>
			void push_back(const TYPE & i_source)
		{

		}

	private:

		TYPE * get_item_at(size_t i_index)
		{
			assert(i_index < m_size);
			return static_cast<TYPE**>(m_dictionary)[i_index];
		}

		const TYPE * get_item_at(size_t i_index) const
		{
			assert(i_index < m_size);
			return static_cast<TYPE**>(m_dictionary)[i_index];
		}

		const TYPE_INFO & get_type_info_at(size_t i_index)
		{
			assert(i_index < m_size);
			TYPE_INFO * static_cast<TYPE**>(m_dictionary)[i_index];
		}

		static void * Alloc(size_t i_size, size_t i_alignment)
		{
			return _aligned_malloc(i_size, i_alignment);
		}

		static void * Free(void * i_block)
		{
			return _aligned_free(i_block);
		}

	private:

		enum InternalConstructor { };

		BulkList(InternalConstructor, size_t i_size, void * i_dictionary, void * i_items)
			: m_size(i_size), m_dictionary(i_dictionary), m_items(i_items)
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

			inline static void construct(TYPE * * & io_item_dictionary, TYPE_INFO * & io_type_dictionary, void * i_items, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				*io_item_dictionary++ = linear_alloc<FIRST_TYPE>(i_items, std::forward<FIRST_TYPE>(i_source));
				new(io_type_dictionary++) TYPE_INFO( TYPE_INFO::make<FIRST_TYPE>() );
				RecursiveHelper<OTHER_TYPES...>::construct(io_item_dictionary, io_type_dictionary, i_items, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename LAST_TYPE>
			struct RecursiveHelper<LAST_TYPE>
		{
			static const size_t s_item_count = 1;
			static const size_t s_item_alignment = std::alignment_of<LAST_TYPE>::value;

			inline static void construct(TYPE * * & io_item_dictionary, TYPE_INFO * & io_type_dictionary, void * i_items, LAST_TYPE && i_source)
			{
				*io_item_dictionary = linear_alloc<LAST_TYPE>(i_items, std::forward<LAST_TYPE>(i_source));
				new(io_type_dictionary)TYPE_INFO(TYPE_INFO::make<LAST_TYPE>());
			}
		};

	private:
		size_t m_size;
		void * m_dictionary;
		void * m_items;
	};
}