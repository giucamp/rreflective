
namespace reflective
{
	GlobalRegistry::GlobalRegistry()
		: m_global_namespace("global")
	{
		m_registry.reserve(s_global_registry_reserve);
	}

	GlobalRegistry & GlobalRegistry::instance()
	{
		static GlobalRegistry s_instance;
		return s_instance;
	}

	void GlobalRegistry::register_type_info(const Type & i_type, const std::type_info & i_type_info)
	{		
		auto res = m_types.insert(std::make_pair(&i_type_info, &i_type));
		REFLECTIVE_ASSERT(res.second, "A type with the same std::type_info has already been registered");
	}

	void GlobalRegistry::unregister_type_info(const Type & i_type, const std::type_info & i_type_info)
	{		
		const auto removed_count = m_types.erase(&i_type_info);
		REFLECTIVE_ASSERT(removed_count == 1, "Type not unregistered");
	}

	const Type * GlobalRegistry::find_type(StringView i_full_name)
	{
		const SymbolName name(i_full_name);

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			auto as_type = dynamic_cast<const Type*>( it->second);
			if (as_type != nullptr)
			{
				return as_type;
			}
		}

		return nullptr;
	}

	void GlobalRegistry::register_member(const NamespaceMember & i_member)
	{
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

	/*const Type * GlobalRegistry::find_type(StringView i_full_name)
	{

	}*/
}