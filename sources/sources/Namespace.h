
#pragma once
#include "core\symbol.h"
#include "type\type.h"

namespace reflective
{
	class Namespace : public Symbol
	{
	public:

		Namespace(const Namespace * i_parent_namespace, SymbolName i_name)
			: Symbol(std::move(i_name)), m_parent_namespace(i_parent_namespace) {  }

		void add_child_type(std::unique_ptr<Type> i_source)
		{
			m_types.insert(std::make_pair(i_source->name(), std::move(i_source)));
		}

		void add_child_namespace(std::unique_ptr<Namespace> i_source)
		{
			m_namespaces.insert(std::make_pair(i_source->name(), std::move(i_source)));
		}

		const Type * find_type(const SymbolName & i_name) const
		{
			const auto it = m_types.find(i_name);
			if (it != m_types.end())
				return it->second.get();
			else
				return nullptr;
		}

		const Namespace * find_child_namespace(const SymbolName & i_name) const
		{
			const auto it = m_namespaces.find(i_name);
			if (it != m_namespaces.end())
				return it->second.get();
			else
				return nullptr;
		}

		const Namespace * find_child_namespace(const char * i_path)
		{
			char tmp_storage[256];

		}

	private:
		std::unordered_map<SymbolName, std::unique_ptr<Type>, SymbolNameHasher, 
			std::equal_to<SymbolName>, MapAllocator<std::pair<SymbolName, std::unique_ptr<Type>>> > m_types;
		std::unordered_map<SymbolName, std::unique_ptr<Namespace>, SymbolNameHasher,
			std::equal_to<SymbolName>, MapAllocator<std::pair<SymbolName, std::unique_ptr<Namespace>>> > m_namespaces;
		const Namespace * const m_parent_namespace;
	};
}
