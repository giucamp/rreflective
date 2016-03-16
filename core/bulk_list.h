
namespace reflective
{
	template <typename OTHER_TYPE, typename... CONSTR_PARAMS>
		inline OTHER_TYPE * linear_alloc(void * & io_address, CONSTR_PARAMS && ...i_params)
	{
		OTHER_TYPE * result = new OTHER_TYPE(std::forward<CONSTR_PARAMS>(i_params)...);
		io_address = result + 1;
		return result;
	}

	template <typename TYPE, typename TYPE_INFO, typename ALLOCATOR = std::allocator<TYPE> >
		class BulkList : private ALLOCATOR
	{
	public:

		template <typename... TYPES>
			inline static BulkList make(TYPES... i_tuple)
		{
			const size_t size = Size<TYPES...>::s_size;
			void * alloc = malloc(size);
			void * bulk = alloc;
			Size<TYPES...>::construct(alloc, nullptr, std::forward<TYPES>(i_tuple)...);
			return BulkList(bulk);
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


	private:


		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct Size
		{
			static const size_t s_size = sizeof(FIRST_TYPE) + Size<OTHER_TYPES...>::s_size;

			inline static void construct(void * & io_address, KNOT * i_prev_knot, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				KNOT * knot = linear_alloc<KNOT>(io_address);
				if (i_prev_knot != nullptr)
				{
					i_prev_knot->m_next = knot;
				}

				linear_alloc<FIRST_TYPE>(io_address, std::forward<FIRST_TYPE>(i_source));

				Size<OTHER_TYPES...>::construct(io_address, knot, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename FIRST_TYPE>
			struct Size<FIRST_TYPE>
		{
			static const size_t s_size = sizeof(FIRST_TYPE);

			inline static void construct(void * & io_address, KNOT * i_prev_knot, FIRST_TYPE && i_source)
			{
				KNOT * knot = linear_alloc<KNOT>(io_address);
				if (i_prev_knot != nullptr)
				{
					i_prev_knot->m_next = knot;
				}
				knot->m_next = nullptr;

				linear_alloc<FIRST_TYPE>(io_address, std::forward<FIRST_TYPE>(i_source));
			}
		};

	private:
		size_t m_size;
		void * m_dictionary;
		void * m_items;
	};
}