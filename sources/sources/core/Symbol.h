
#pragma once
#include "../reflective_common.h"
#include "PtrString.h"
#include "Identifier.h"
#include "StringHash.h"

namespace reflective
{
	struct SymbolNameHasher
	{
		uint32_t operator () (const SymbolName & i_source) const
		{
			return i_source.hash();
		}
	};

	enum class SymbolType
	{
		primitive_type_symbol,
		class_symbol,
		enum_symbol
	};

	class Symbol
	{
	public:

		Symbol(SymbolName i_name)
			: m_name(std::move(i_name)) { }

		Symbol(const Symbol &) = delete;

		Symbol & operator = (const Symbol &) = delete;

		const SymbolName & name() const
		{
			return m_name;
		}

	private:
		const SymbolName m_name;
	};
}

