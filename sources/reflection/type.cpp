
void pt(const char*, size_t);

namespace reflective
{
	Type::Type(SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions)
		: Symbol(std::move(i_name)), m_special_functions(i_special_functions), m_size(i_size), m_alignment(i_alignment),
		m_parent_namespace(nullptr), m_next_type_in_namespace(nullptr)
	{
		pt(i_name.string().cbegin(), i_size);
		REFLECTIVE_ASSERT(i_size <= s_max_size, "The size of the type exceeds s_max_size");
		REFLECTIVE_ASSERT(is_power_of_2(i_alignment), "The alignment of the type is not a power of 2");
		REFLECTIVE_ASSERT(i_alignment <= s_max_alignment, "The alignment of the type exceeds s_max_alignment");
	}

	std::string Type::full_name() const
	{
		int type_count = 1;
		std::array< const Symbol *, 16 > names;

		names[0] = this;
		size_t size_to_reserve = name().string().size();
		for (const Namespace * curr = m_parent_namespace; curr != nullptr; curr = curr->parent_namespace())
		{
			REFLECTIVE_ASSERT(type_count < names.size(), "Type too much scoped");
			size_to_reserve += curr->name().string().size() + 2;
			names[type_count] = curr;
			type_count++;
		}

		std::string res;
		res.reserve(size_to_reserve);
		for (int i = type_count - 1; i >= 0; i--)
		{
			res += names[i]->name().string().c_str();
			if( i > 0 )
			{
				res += "::";
			}
		}

		return res;
	}
}
