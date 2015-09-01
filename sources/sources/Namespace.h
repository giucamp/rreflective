
#pragma once
#include "core\symbol.h"
#include "type\type.h"

namespace reflective
{
	class Namespace : public Symbol
	{
	public:

		Namespace(SymbolName i_name)
			: Symbol(std::move(i_name)) { }

		void add_type(const Type & i_type)
		{
			m_types.push_back(&i_type);
		}

		void add_type(const Namespace & i_namespace)
		{
			m_namespace.push_back(&i_namespace);
		}

	private:
		vector< const Type * > m_types;
		vector< const Namespace * > m_namespace;
	};
}
