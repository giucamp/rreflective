
#pragma once
#include "..\core\Symbol.h"
#include "SpecialFunctions.h"

namespace reflective
{
	class Type : public Symbol
	{
	public:

		static const size_t s_max_size = (1 << 24) - 1;
		static const size_t s_max_alignment = (1 << 8) - 11;

		Type(SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions);

		size_t size() const							{ return m_size; }

		size_t alignment() const					{ return m_alignment; }
		
	private:
		SpecialFunctions m_special_functions;
		const uint32_t m_size : 24;
		const uint32_t m_alignment : 8;
	};
}

#include "Type.inl"



