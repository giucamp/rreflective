
#pragma once
#include "../reflective_settings.h"

namespace reflective
{
	template <typename SYMBOL>
		class SymbolList
	{
	public:

		SymbolList()
		{

		}

		SymbolList(std::initializer_list<std::unique_ptr<const SYMBOL>> && i_symbols)
			: m_symbols(std::move(i_symbols))
		{

		}

		SymbolList(const SymbolList &) = delete;

	private:
		std::vector< std::unique_ptr<const SYMBOL> > m_symbols;
	};
}

