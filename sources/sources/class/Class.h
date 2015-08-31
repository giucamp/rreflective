
#pragma once
#include "..\Type\Type.h"
#include "..\core\SymbolList.h"
#include "property\Property.h"

namespace reflective
{
	class Class : public Type
	{
	public:

		Class(SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions)
			: Type( std::move(i_name), i_size, i_alignment, i_special_functions) { }

		const SymbolList<Property> & properites() const					{ return m_properites; }

		void set_properites(SymbolList<Property> && i_properites)		{ m_properites = std::move(i_properites); }

	private:
		SymbolList<Property> m_properites;
		//SymbolList<Actions> m_actions;
	};
}
