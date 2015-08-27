
#pragma once
#include "core\symbol.h"
#include "type\type.h"

namespace reflective
{
	class Namespace : public Symbol
	{
	public:

		Namespace(const SymbolName & i_name)
			: Symbol(i_name){}

		Namespace(SymbolName && i_name)
			: Symbol(std::move(i_name)) { }

		void add_child_type(std::unique_ptr<Type> && i_source)
		{
			m_types.insert(std::make_pair(i_source->name(), std::move(i_source)));
		}

		void add_child_namespace(std::unique_ptr<Namespace> && i_source)
		{
			m_namespaces.insert(std::make_pair(i_source->name(), std::move(i_source)));
		}

	private:

		std::unordered_map<SymbolName, std::unique_ptr<Type>, SymbolNameHasher> m_types;
		std::unordered_map<SymbolName, std::unique_ptr<Namespace>, SymbolNameHasher> m_namespaces;
	};
}
