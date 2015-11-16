
namespace reflective
{
	GlobalRegistry::GlobalRegistry()
	{
		m_registry.reserve(s_global_registry_reserve);
	}

	GlobalRegistry & GlobalRegistry::instance()
	{
		static GlobalRegistry s_instance;
		return s_instance;
	}

	void GlobalRegistry::register_type(const Type & i_type, const std::type_info & i_type_info)
	{
		register_member(i_type);
		auto res = m_types.insert(std::make_pair(&i_type_info, &i_type));
		REFLECTIVE_ASSERT(res.second, "A type with the same std::type_info has already been registered");
	}

	void GlobalRegistry::unregister_type(const Type & i_type, const std::type_info & i_type_info)
	{
		unregister_member(i_type);
		const auto removed_count = m_types.erase(&i_type_info);
		REFLECTIVE_ASSERT(removed_count == 1, "Type not unregistered");
	}

	const Type * GlobalRegistry::find_type(StringView i_full_name)
	{
		const SymbolName name(i_full_name);

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			const auto type_id = it->second->get_type_id();
			if ((type_id & SymbolTypeId::is_type) != SymbolTypeId::none)
			{
				return static_cast<const Type *>( it->second );
			}
		}

		return nullptr;
	}

	void GlobalRegistry::register_member(const NamespaceMember & i_member)
	{
		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());

		#if REFLECTIVE_ASSERT_ENABLED

			// check the type of the member
			int type_check = 0;
			type_check += (i_member.get_type_id() & SymbolTypeId::is_type) != SymbolTypeId::none ? 1 : 0;
			type_check += (i_member.get_type_id() & SymbolTypeId::is_namespace) != SymbolTypeId::none ? 1 : 0;
			type_check += (i_member.get_type_id() & SymbolTypeId::is_class_template) != SymbolTypeId::none ? 1 : 0;
			REFLECTIVE_ASSERT(type_check == 1, "The member must be a type, a class template or a namespace");

			// check for duplicates
			const auto membed_type_id = i_member.get_type_id();
			const auto range = m_registry.equal_range(name);
			for (auto it = range.first; it != range.second; it++)
			{
				REFLECTIVE_ASSERT(it->second != &i_member, "Member already registered");
				REFLECTIVE_ASSERT((it->second->get_type_id() & membed_type_id) != SymbolTypeId::none, "Member with the same name and type already registered");
			}

		#endif

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