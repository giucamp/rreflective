
namespace reflective
{
	namespace details
	{
		class DerivedTypesList::ConstIterator
		{
		public:

			ConstIterator(const Type * i_curr)
				: m_curr(i_curr) { }

			ConstIterator & operator ++ ()
			{
				m_curr = m_curr->m_next_derived;
				return *this;
			}

			bool operator == (ConstIterator & i_other) const
			{
				return m_curr == i_other.m_curr;
			}

			bool operator != (ConstIterator & i_other) const
			{
				return m_curr != i_other.m_curr;
			}

			const Type * operator ->() const
			{
				return m_curr;
			}

			const Type & operator * () const
			{
				return *m_curr;
			}

		private:
			const Type * m_curr;
		};

		inline DerivedTypesList::ConstIterator DerivedTypesList::begin() const
		{
			return ConstIterator(m_first);
		}

		inline DerivedTypesList::ConstIterator DerivedTypesList::end() const
		{
			return ConstIterator(nullptr);
		}

		inline DerivedTypesList::ConstIterator DerivedTypesList::cbegin() const
		{
			return ConstIterator(m_first);
		}

		inline DerivedTypesList::ConstIterator DerivedTypesList::cend() const
		{
			return ConstIterator(nullptr);
		}
	} // details
}