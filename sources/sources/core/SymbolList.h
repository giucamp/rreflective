
#pragma once
#include "../reflective_settings.h"

namespace reflective
{
	/**
		This class is moveable but not copyable.
	*/
	template <typename SYMBOL>
		class SymbolList
	{
	public:

		SymbolList()
			: m_count(0)
		{
		}

		SymbolList(const SymbolList &) = delete;

		SymbolList(SymbolList && i_from)
			: m_symbols(std::move(i_from.m_symbols)), m_count(0)
		{
			i_from.m_count = 0;
		}

		SymbolList & operator = (SymbolList && i_from)
		{
			m_symbols = std::move(i_from.m_symbols);
			m_count = i_from.m_count;
			i_from.m_count = 0;
			return *this;
		}

		SymbolList(std::initializer_list<std::unique_ptr<const SYMBOL>> i_symbols)
			: m_symbols(std::move(i_symbols)), m_count( i_symbols.size() )
		{
		}		

		~SymbolList() = default;

	private:
		std::unique_ptr< std::unique_ptr<const SYMBOL>[] > m_symbols;
		size_t m_count;
	};
}

