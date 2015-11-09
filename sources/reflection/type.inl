
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

	class Type::BaseTypeContainer
	{
	public:
		BaseTypeContainer(const Type & i_type)
			: m_type(i_type)
		{
		}

		BaseTypeContainer(const BaseTypeContainer &) = delete;
		BaseTypeContainer & operator = (const BaseTypeContainer &) = delete;
		
		class const_iterator
		{
		public:

			const_iterator()
				: m_type(nullptr), m_curr(nullptr)
			{
			}

			const_iterator(const Type & i_type)
				: m_type(&i_type)
			{
				m_curr = &m_type->m_single_base;
				if (m_curr->base_type() == nullptr)
				{
					m_curr = nullptr;
				}
			}

			const_iterator operator ++ ()
			{
				const_iterator temp = *this;
				++(*this);
				return temp;
			}

			const_iterator & operator ++ (int)
			{
				REFLECTIVE_ASSERT(m_curr != nullptr, "This iterator cannot be incremented");
				if (m_curr == &m_type->m_single_base)
				{
					m_vect_it = m_type->m_other_base_types.cbegin();
				}
				else
				{
					m_vect_it++;
				}
				if (m_vect_it == m_type->m_other_base_types.cend())
				{
					m_curr = nullptr;
				}
				else
				{
					m_curr = &*m_vect_it;
				}
				return *this;
			}

			bool operator == (const const_iterator i_other) const
			{
				return m_curr == i_other.m_curr;
			}

			bool operator != (const const_iterator i_other) const
			{
				return m_curr != i_other.m_curr;
			}

			const BaseType & operator * () const
			{ 
				REFLECTIVE_ASSERT(m_curr != nullptr, "This iterator cannot be indirected");
				return *m_curr; 
			}

			const BaseType * operator -> () const
			{
				REFLECTIVE_ASSERT(m_curr != nullptr, "This iterator cannot be indirected");
				return m_curr; 
			}

		private:
			const Type * m_type;
			const BaseType * m_curr;
			std::vector<BaseType>::const_iterator m_vect_it;
		};

		const_iterator begin() const
		{
			return const_iterator(m_type);
		}

		const_iterator end() const
		{
			return const_iterator();
		}

		const_iterator cbegin() const
		{
			return const_iterator(m_type);
		}

		const_iterator cend() const
		{
			return const_iterator();
		}

	private:
		const Type & m_type;
	};

	inline const Type::BaseTypeContainer Type::base_types() const
	{
		// returns a temporary object that can be used to construct a const interator
		return BaseTypeContainer(*this);
	}

	#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE
		inline bool Type::can_upcast_to(const Type & i_base_type) const
		{
			return base_type_multeplicity(i_base_type) == 1;
		}
	#endif

}