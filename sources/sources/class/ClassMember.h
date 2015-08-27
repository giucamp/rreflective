
#pragma once
#include "..\core\Symbol.h"

namespace reflective
{
	class ClassMember : public Symbol
	{
	public:
		ClassMember(const SymbolName & i_name)
			: Symbol(i_name){}

		ClassMember(SymbolName && i_name)
			: Symbol(std::move(i_name)) { }
	};
}
