
#pragma once
#include "..\ClassMember.h"

namespace reflective
{
	class Property : public ClassMember
	{
	public:

		Property(SymbolName i_name)
			: ClassMember(std::move(i_name)) { }
		
	};
}
