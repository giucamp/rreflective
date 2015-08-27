
#pragma once
#include "..\Type\Type.h"

namespace reflective
{
	class Class : public Type
	{
	public:

		Class(SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions)
			: Type( std::move(i_name), i_size, i_alignment, i_special_functions) {}
	};
}
