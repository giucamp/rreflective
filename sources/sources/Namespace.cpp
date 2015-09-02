

#include "namespace.h"

namespace reflective
{
	Namespace::Namespace(SymbolName i_name)
		: Symbol(std::move(i_name)) { }

	void Namespace::register_type(const Type & i_type)
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
	}


	Registry & Registry::instance()
	{
		static Registry s_instance;
		return s_instance;
	}
}