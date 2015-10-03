
namespace reflective
{
	inline GlobalRegistry::GlobalRegistry()
	{
		m_type_registry.reserve(s_type_registry_reserve);
		m_class_template_registry.reserve(s_class_template_registry_reserve);
	}

	inline GlobalRegistry & GlobalRegistry::internal_instance()
	{
		static GlobalRegistry s_instance;
		return s_instance;
	}

	void GlobalRegistry::register_type(const Type & i_type)
	{
		GlobalRegistry & instance = internal_instance();
		const SymbolName full_name(i_type.full_name().c_str());
		const auto res = instance.m_type_registry.insert(std::make_pair(full_name.hash(), &i_type));
		REFLECTIVE_ASSERT(res.second, "Type already registered");
	}

	void GlobalRegistry::unregister_type(const Type & i_type)
	{
		const SymbolName full_name(i_type.full_name().c_str());
		GlobalRegistry & instance = internal_instance();
		const auto res = instance.m_type_registry.find(full_name.hash());
		const bool found = res != instance.m_type_registry.end();
		REFLECTIVE_ASSERT(found, "Type not registered");
		if (found)
		{
			instance.m_type_registry.erase(res);
		}
	}

	/*void GlobalRegistry::register_class_template(const ClassTemplate & i_class_template)
	{
		GlobalRegistry & instance = internal_instance();
		const SymbolName full_name(i_type.full_name().c_str());
		const auto res = instance.m_class_template_registry.insert(std::make_pair(full_name.hash(), &i_type));
		REFLECTIVE_ASSERT(res.second, "Type already registered");
	}

	void GlobalRegistry::unregister_class_template(const ClassTemplate & i_class_template)
	{
	}*/

	/*void GlobalRegistry::register_namespace(SymbolName i_namespace_full_name, const Namespace & i_namespace)
	{
		const auto res = m_namespace_registry.insert(std::make_pair(i_namespace_full_name, &i_namespace));
		REFLECTIVE_ASSERT(res.second, "Duplicate namespace?");
	}

	const Namespace * GlobalRegistry::unregister_namespace(SymbolName i_namespace_full_name)
	{
		const auto res = m_namespace_registry.find(i_namespace_full_name);
		if (res != m_namespace_registry.end())
		{
			const Namespace * namespace_obj = res->second;
			m_namespace_registry.erase(res);
			return namespace_obj;
		}
		else
		{
			return nullptr;
		}
	}*/
}