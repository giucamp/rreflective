
namespace reflective
{
	Namespace::Namespace(SymbolName i_name)
		: Symbol(std::move(i_name)), m_first_child_type(nullptr), m_first_child_namespace(nullptr), m_parent_namespace(nullptr) {}

	/*void Namespace::register_type(const Type & i_type)
	{
	m_types.push_back(&i_type);
	}

	void Namespace::unregister_type(const Type & i_type)
	{
	const auto it = std::find(m_types.begin(), m_types.end(), &i_type);
	REFLECTIVE_ASSERT(it != m_types.end(), "unregistering type not registered");
	m_types.erase(it);
	}

	void Namespace::register_namespace(const Namespace & i_namespace)
	{
	m_namespace.push_back(&i_namespace);
	}

	void Namespace::unregister_namespace(const Namespace & i_namespace)
	{
	const auto it = std::find(m_namespace.begin(), m_namespace.end(), &i_namespace);
	REFLECTIVE_ASSERT(it != m_namespace.end(), "unregistering namespace not registered");
	m_namespace.erase(it);
	}*/


	Registry & Registry::instance()
	{
		static Registry s_instance;
		return s_instance;
	}

	void Registry::register_type(SymbolName i_type_full_name, const Type & i_type)
	{
		const auto res = m_type_registry.insert(std::make_pair(i_type_full_name, &i_type));
		REFLECTIVE_ASSERT(res.second, "Duplicate type?");
	}

	const Type * Registry::unregister_type(SymbolName i_type_full_name)
	{
		const auto res = m_type_registry.find(i_type_full_name);
		if (res != m_type_registry.end())
		{
			const Type * type = res->second;
			m_type_registry.erase(res);
			return type;
		}		
		else
		{
			return nullptr;
		}
	}

	void Registry::register_namespace(SymbolName i_namespace_full_name, const Namespace & i_namespace)
	{
		const auto res = m_namespace_registry.insert(std::make_pair(i_namespace_full_name, &i_namespace));
		REFLECTIVE_ASSERT(res.second, "Duplicate namespace?");
	}

	const Namespace * Registry::unregister_namespace(SymbolName i_namespace_full_name)
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
	}
}