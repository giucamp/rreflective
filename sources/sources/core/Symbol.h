
#pragma once
#include "../reflective_settings.h"

namespace reflective
{
	struct SymbolNameHasher
	{
		uint32_t operator () (const SymbolName & i_source) const
		{
			return i_source.hash();
		}
	};

	class Symbol
	{
	public:

		Symbol(const SymbolName & i_name)
			: m_name(i_name) { }

		Symbol(SymbolName && i_name)
			: m_name(std::move(i_name)) { }

		const SymbolName & name() const			
			{ return m_name; }

	private:
		const SymbolName m_name;
	};
}

