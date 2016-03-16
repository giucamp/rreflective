
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
			OTHER_TYPE * result = new(address_upper_align<OTHER_TYPE>(io_address))OTHER_TYPE(std::forward<CONSTR_PARAMS>(i_params)...);
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

		template <typename COMPLETE_TYPE>
			DefaultTypeInfo make()
		{
			return DefaultTypeInfo(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, 
				&DestructorImpl<COMPLETE_TYPE>, &MoverDestructorImpl<COMPLETE_TYPE>);
		}

	private:

		using DestructorPtr = void(*)(TYPE * i_element);
		using MoverDestructorPtr = void(*)(TYPE * i_dest_element, TYPE * i_source_element);

		DefaultTypeInfo(size_t i_size, size_t i_alignment, DestructorPtr i_destructor, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_destructor(i_destructor), i_mover_destructor(i_mover_destructor)
				{ }

		template <typename COMPLETE_TYPE>
			void DestructorImpl(TYPE * i_element)
		{
			static_cast<COMPLETE_TYPE*>(i_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

		void MoverDestructorImpl(void * i_destination, TYPE * i_source_element)
		{
			new (i_destination) COMPLETE_TYPE(std::move(*i_source_element));
			static_cast<COMPLETE_TYPE*>(i_source_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

	private:
		size_t const m_size, m_alignment;
		DestructorPtr const m_destructor;
		MoverDestructorPtr const m_mover_destructor;
	};

	template <typename TYPE, typename TYPE_INFO = DefaultTypeInfo<TYPE>, typename ALLOCATOR = std::allocator<TYPE> >
		class BulkList : private ALLOCATOR
	{
	public:

		template <typename... TYPES>
			inline static BulkList make(TYPES... i_tuple)
		{
			const size_t buffer_size = RecursiveHelper<TYPES...>::s_buffer_size;
			const size_t buffer_alignment = RecursiveHelper<TYPES...>::s_item_alignment;
			const size_t item_count = RecursiveHelper<TYPES...>::s_item_count;

			void * items = Alloc(buffer_size, buffer_alignment);
			void * dictionary = Alloc(sizeof(TYPE) * (item_count + 1) + item_count * sizeof(TYPE_INFO), 8);

			void * bulk = items;
			RecursiveHelper<TYPES...>::construct(bulk, std::forward<TYPES>(i_tuple)...);
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

		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct RecursiveHelper
		{
			static const size_t s_buffer_size = sizeof(FIRST_TYPE) + RecursiveHelper<OTHER_TYPES...>::s_buffer_size;
			static const size_t s_item_count = 1 + RecursiveHelper<OTHER_TYPES...>::s_item_count;
			static const size_t s_item_alignment = std::alignment_of<FIRST_TYPE>::value > RecursiveHelper<OTHER_TYPES...>::s_item_alignment ?
				std::alignment_of<FIRST_TYPE>::value : RecursiveHelper<OTHER_TYPES...>::s_item_alignment;

			inline static void construct(void * i_address, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				linear_alloc<FIRST_TYPE>(i_address, std::forward<FIRST_TYPE>(i_source));

				RecursiveHelper<OTHER_TYPES...>::construct(i_address, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename FIRST_TYPE>
			struct RecursiveHelper<FIRST_TYPE>
		{
			static const size_t s_buffer_size = sizeof(FIRST_TYPE);
			static const size_t s_item_count = 1;
			static const size_t s_item_alignment = std::alignment_of<FIRST_TYPE>::value;

			inline static void construct(void * i_address, FIRST_TYPE && i_source)
			{
				linear_alloc<FIRST_TYPE>(i_address, std::forward<FIRST_TYPE>(i_source));
			}
		};

	private:
		size_t m_size;
		void * m_dictionary;
		void * m_items;
	};
}