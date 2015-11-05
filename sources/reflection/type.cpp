
namespace reflective
{
	Type::Type(SymbolTypeId i_type_id, SymbolName i_name, size_t i_size, size_t i_alignment )
			: NamespaceMember(i_type_id, std::move(i_name)), m_size(i_size), m_alignment(i_alignment), 
			m_most_derived_func(nullptr), m_next_derived(nullptr)
	{
		m_implicit_reflection = false;

		REFLECTIVE_ASSERT(i_size <= s_max_size, "The size of the type exceeds s_max_size");
		REFLECTIVE_ASSERT(is_power_of_2(i_alignment), "The alignment of the type is not a power of 2");
		REFLECTIVE_ASSERT(i_alignment <= s_max_alignment, "The alignment of the type exceeds s_max_alignment");
	}

	void Type::set_base_types(const ArrayView<const BaseType> & i_base_types)
	{
		if (i_base_types.size() >= 1)
		{
			m_single_base = i_base_types[0];
			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE
				m_other_base_types.resize(i_base_types.size() - 1);
				for (size_t base_type_index = 1; base_type_index < i_base_types.size(); base_type_index++)
				{
					m_other_base_types[base_type_index - 1] = i_base_types[base_type_index];
				}
			#else
				REFLECTIVE_ASSERT(i_base_types.size() == 1, "More than one base type provided, but multiple inheritance is disabled (REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE si false)");
			#endif
		}
		else
		{
			m_single_base = BaseType();
			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE
				m_other_base_types.clear();
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

	#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE	

		/* this internal function count the occurrences of i_base_type among the base 
			types of this type, but stop the search once the count is equal or greater than 2 */
		size_t Type::base_type_multeplicity(const Type & i_base_type) const
		{
			size_t result = 0;
			const Type * curr_type = this;
			do {

				if (curr_type == &i_base_type)
				{
					result++;
					if (result >= 2)
					{
						return result;
					}
				}

				for (const auto & base : m_other_base_types)
				{
					result += base.base_type()->base_type_multeplicity(i_base_type);

					if (result >= 2)
					{
						return result;
					}
				}

				curr_type = curr_type->m_single_base.base_type();

			} while (curr_type != nullptr);

			return result;
		}

	#else

		bool Type::can_upcast_to(const Type & i_base_type) const
		{
			const Type * curr_type = this;
			do {

				if (curr_type == &i_base_type)
				{
					return true;
				}

				curr_type = curr_type->m_single_base.base_type();

			} while (curr_type != nullptr);

			return false;
		}
	#endif

	const Type * Type::most_derived(const void * i_object) const
	{
		const void * curr_object = i_object;
		const Type * curr_type = this;
		do {

			if (curr_type->m_most_derived_func != nullptr)
			{
				return &(*curr_type->m_most_derived_func)(curr_object);
			}

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

				for (const auto & base : m_other_base_types)
				{
					const void * base_object = base.updown_caster().derived_to_base(curr_object);
					auto res = base.base_type()->most_derived(base_object);
					if (res != nullptr)
					{
						return res;
					}
				}

			#endif

			const auto & base = curr_type->m_single_base;
			curr_type = base.base_type();
			if (curr_type != nullptr)
			{
				curr_object = base.updown_caster().derived_to_base(curr_object);
			}

		} while (curr_type != nullptr);

		return nullptr;
	}

	void * Type::upcast(void * i_object, const Type & i_dest_base_type) const
	{
		void * curr_object = i_object;
		if (curr_object == nullptr)
		{
			return nullptr;
		}

		const Type * curr_type = this;
		do {

			if (curr_type == &i_dest_base_type)
			{
				return curr_object;
			}

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

				for (const auto & base : curr_type->m_other_base_types)
				{
					void * base_object = base.updown_caster().derived_to_base(curr_object);
					void * casted = base.base_type()->upcast(base_object, i_dest_base_type);
					if (casted != nullptr)
					{
						return casted;
					}
				}

			#endif

			const auto & base = curr_type->m_single_base;
			curr_type = base.base_type();
			if (curr_type != nullptr)
			{
				curr_object = base.updown_caster().derived_to_base(curr_object);
			}

		} while (curr_type != nullptr);

		return nullptr;
	}

	struct TypeInfo
	{
		int dist = -1;
		BaseType base;
	};

	bool Type::internal_find_path_to_type(const Type & i_source_type, const Type & i_target_type, std::vector<BaseType> & io_base_types)
	{
		std::map<const Type*, TypeInfo> map;

		std::vector<const Type*> stack;

		stack.push_back(&i_source_type);

		while( stack.size() > 0 )
		{
			const auto curr_type = stack.back();
			stack.pop_back();

			for( const auto & base : curr_type->m_other_base_types )
			{
				if (map[base.base_type()].dist == -1)
				{
					auto & map_item = map[base.base_type()];
					map_item.dist = map[curr_type].dist + 1;
					map_item.base = base;
					stack.push_back(base.base_type());
				}
				else
				{
					return false;
				}
			}
		}



		/*const Type * curr_type = this;
		do {

			if (curr_type == &i_target_type)
			{
				return true;
			}			

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

			for (const auto & base : curr_type->m_other_base_types)
			{
				const size_t original_size = io_base_types.size();
				if (base.base_type()->internal_find_path_to_type(io_base_types, i_target_type))
				{
					io_base_types.push_back(base);
					return true;
				}
				io_base_types.resize(original_size);
			}			

			#endif

			const auto & base = curr_type->m_single_base;
			io_base_types.push_back(base);
			curr_type = base.base_type();

		} while (curr_type != nullptr);*/

		return false;
	}

	void * Type::try_dynamic_cast(void * i_source_object, const Type & i_dest_type) const
	{
		const Type * most_derived_type = nullptr;
		
		/* upcast until a type that can tell which the most derived type is (that is a type for 
			which m_most_derived_func != nullptr). The aim is obtaining a pointer to the most
			derived type, but if the destination type is found, return immediately (the cast succeeded). */
		void * curr_object = i_source_object;
		const Type * curr_type = this;
		do {

			if (curr_type == &i_dest_type)
			{
				return curr_object;
			}

			if (curr_type->m_most_derived_func != nullptr)
			{
				most_derived_type = &(*curr_type->m_most_derived_func)(curr_object);
				break;
			}

			#if REFLECTIVE_ENABLE_MULTIPLE_INHERITANCE

				for (const auto & base : m_other_base_types)
				{
					void * base_object = base.updown_caster().derived_to_base(curr_object);
					most_derived_type = base.base_type()->most_derived(base_object);					
					if (most_derived_type != nullptr)
					{
						break;
					}
				}

				if (most_derived_type != nullptr)
				{
					break;
				}

			#endif

			const auto & base = curr_type->m_single_base;
			curr_type = base.base_type();
			if (curr_type != nullptr)
			{
				curr_object = base.updown_caster().derived_to_base(curr_object);
			}

		} while (curr_type != nullptr);

		if (most_derived_type != nullptr)
		{
			/* now we need a path from this type to the most derived type, to obtain a pointer
				to the most derived type */
			std::vector<BaseType> base_types;
			if (internal_find_path_to_type(*this, *most_derived_type, base_types))
			{
				REFLECTIVE_INTERNAL_ASSERT(base_types.size() == 0 || std::find_if(base_types.begin(), base_types.end(), [this](const BaseType & i_base) {
					return i_base.base_type() == this;
				}) != base_types.end());
				REFLECTIVE_INTERNAL_ASSERT(base_types.size() == 0 || std::find_if(base_types.begin(), base_types.end(), [most_derived_type](const BaseType & i_base) {
					return i_base.base_type() == most_derived_type;
				}) == base_types.end());

				// downcast from this type to the most derived
				curr_object = i_source_object;
				for (auto base_it = base_types.crbegin(); base_it != base_types.crend(); base_it++ )
				{
					curr_object = base_it->updown_caster().base_to_derived(curr_object);
				}

				// now upcast from the most derived to the destination type.
				void * result = most_derived_type->upcast(curr_object, i_dest_type);
				return result;
			}
			else
			{
				return nullptr; // i_dest_type is not a base of the most derived class
			}
		}
		else
		{
			/* the cast may be possible, but there is not any base type with a MostDerivedFunc, so
				there is no way of getting the most derived type. */
			return nullptr;
		}
	}

	#if REFLECTIVE_ENABLE_TESTING

		namespace details
		{
			void test_type_derived_list();
		}

		void Type::unit_test()
		{
			details::test_type_derived_list();
		}

	#endif

}
