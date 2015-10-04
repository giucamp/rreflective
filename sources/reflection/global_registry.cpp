
namespace reflective
{
	template <typename TYPE>
		const TYPE * GlobalRegistry::find_member(StringView i_full_type_name)
	{
		const SymbolName name(i_full_type_name.data(), i_full_type_name.length());

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			const TYPE * const result = dynamic_cast<const TYPE*>(it->second);
			if (result != nullptr)
			{
				return result;
			}
		}

		return nullptr;
	}

	GlobalRegistry::GlobalRegistry()
	{
		m_registry.reserve(s_global_registry_reserve);
	}

	GlobalRegistry & GlobalRegistry::instance()
	{
		static GlobalRegistry s_instance;
		return s_instance;
	}

	void GlobalRegistry::register_member(const NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(!is_member_registered(i_member), "Member already registered");

		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());
		m_registry.insert(std::make_pair(name, &i_member));
	}

	void GlobalRegistry::unregister_member(const NamespaceMember & i_member)
	{
		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());

		bool found = false;

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			if (it->second == &i_member)
			{
				m_registry.erase(it);
				found = true;
			}
		}

		REFLECTIVE_ASSERT(found, "Member not found");
	}

	bool GlobalRegistry::is_member_registered(const NamespaceMember & i_member) const
	{
		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			if (it->second == &i_member)
			{
				return true;
			}
		}

		return false;
	}
}