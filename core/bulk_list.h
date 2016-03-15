
namespace reflective
{
	template <typename TYPE, typename TYPE_INFO, typename ALLOCATOR = std::allocator<TYPE> >
		class BulkList : private ALLOCATOR
	{
	public:

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

		TYPE * get_type_info_at(size_t i_index)
		{
			assert(i_index < m_size);
			return static_cast<TYPE**>(m_dictionary)[i_index];
		}

	private:
		size_t m_size;
		void * m_dictionary;
		void * m_items;
	};
}