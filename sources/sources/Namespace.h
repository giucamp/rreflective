
#pragma once
#include "core\symbol.h"
#include "type\type.h"

namespace reflective
{
	class Namespace : public Symbol
	{
	public:
		
		Namespace(SymbolName i_name);

		void register_type(const Type & i_type);

		void unregister_type(const Type & i_type);

		void register_namespace(const Namespace & i_namespace);

		void unregister_namespace(const Namespace & i_namespace);
	
	private:
		Type * m_first_child_type;
		Namespace * m_first_child_namespace;
		Namespace * m_parent_namespace;
	};

	class Registry
	{
	public:

		static Registry & instance();

		void register_type(const Type & i_type);

		void unregister_type(const Type & i_type);

		void register_namespace(const Namespace & i_namespace);

		void unregister_namespace(const Namespace & i_namespace);

	private:
		std::unordered_map<SymbolName, const Type * > m_type_registry;
	};
}
