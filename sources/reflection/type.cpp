
namespace reflective
{
	Type::Type(SymbolTypeId i_type_id, SymbolName i_name, size_t i_size, size_t i_alignment,
			const SpecialFunctions & i_special_functions,
			const ArrayView<BaseType> & i_base_types )
			: NamespaceMember(i_type_id, std::move(i_name)), m_special_functions(i_special_functions), m_size(i_size), m_alignment(i_alignment), 
			m_most_derived_func(nullptr), m_next_derived(nullptr)
	{
		REFLECTIVE_ASSERT(i_size <= s_max_size, "The size of the type exceeds s_max_size");
		REFLECTIVE_ASSERT(is_power_of_2(i_alignment), "The alignment of the type is not a power of 2");
		REFLECTIVE_ASSERT(i_alignment <= s_max_alignment, "The alignment of the type exceeds s_max_alignment");

		if (i_base_types.size() >= 1)
		{
			m_single_base = i_base_types[0];
			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE
				m_base_types.resize(i_base_types.size() - 1);
				for (size_t base_type_index = 1; base_type_index < i_base_types.size(); base_type_index++)
				{
					m_base_types[base_type_index - 1] = i_base_types[base_type_index];
				}
			#else
				REFLECTIVE_ASSERT(i_base_types.size() == 1, "More than one base type provided, but multiple inheritance is disabled (REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE si false)");
			#endif
		}
	}


			// derived types list

	inline details::DerivedTypesList::DerivedTypesList()
		: m_first(nullptr)
	{
	}

	inline void details::DerivedTypesList::add(Type & i_type)
	{
		REFLECTIVE_ASSERT(i_type.m_next_derived == nullptr, "Inconsistent list of derived types (calling add_derived twice?)");
		i_type.m_next_derived = m_first;
		m_first = &i_type;
	}

	inline void details::DerivedTypesList::remove(Type & i_type)
	{
		Type * curr = m_first, *prev = nullptr;
		do {

			if (curr == &i_type)
			{
				if (prev == nullptr)
				{
					m_first = curr->m_next_derived;
				}
				else
				{
					prev->m_next_derived = curr->m_next_derived;
				}
				break;
			}

			prev = curr;
			curr = curr->m_next_derived;

			REFLECTIVE_ASSERT(curr != nullptr, "Inconsistent list of derived types (calling remove_derived for a derived type not present?)");
		} while (curr != nullptr);

		i_type.m_next_derived = nullptr;
	}

	void Type::add_derived(Type & i_derived_type)
	{
		m_derived_types.add(i_derived_type);
	}

	void Type::remove_derived(Type & i_derived_type)
	{
		m_derived_types.remove(i_derived_type);
	}

			// inheritance

	bool Type::is_or_inherits_from(const Type & i_base_type) const
	{
		const Type * curr_type = this;
		do {

			if (curr_type == &i_base_type)
			{
				return true;
			}

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

				for (const auto & base : m_base_types)
				{
					if (base.base_type()->is_or_inherits_from(i_base_type))
					{
						return true;
					}
				}

			#endif

			curr_type = curr_type->m_single_base.base_type();

		} while (curr_type != nullptr);

		return false;
	}

	const Type * Type::most_derived(const void * i_object) const
	{
		const void * curr_object = i_object;
		const Type * curr_type = this;
		do {

			if (curr_type->m_most_derived_func != nullptr)
			{
				return &(*curr_type->m_most_derived_func)(curr_object);
			}

			const auto & base = curr_type->m_single_base;
			curr_type = base.base_type();
			curr_object = base.updown_caster().derived_to_base(curr_object);

		} while (curr_type != nullptr);

		return nullptr;
	}

	void * Type::upcast(const Type & i_base_type, void * i_object) const
	{
		void * curr_object = i_object;
		if (curr_object == nullptr)
		{
			return nullptr;
		}

		const Type * curr_type = this;
		do {

			if (curr_type == &i_base_type)
			{
				return curr_object;
			}

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

				for (const auto & base : m_base_types)
				{
					void * casted = base.base_type()->upcast(i_base_type, curr_object);
					if (casted != nullptr)
					{
						return casted;
					}
				}

			#endif

			const auto & base = curr_type->m_single_base;
			curr_type = base.base_type();
			curr_object = base.updown_caster().derived_to_base(curr_object);

		} while (curr_type != nullptr);

		return false;
	}

	void * Type::downcast(const Type & /*i_derived_type*/, void * i_object) const
	{
		return i_object;
	}
}
