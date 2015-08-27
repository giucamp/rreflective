
namespace reflective
{
	Type::Type(SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions)
		: Symbol(std::move(i_name)), m_special_functions(i_special_functions), m_size(i_size), m_alignment(i_alignment)
	{
		assert(i_size <= s_max_size);
		assert(is_power_of_2(i_alignment));
		assert(i_alignment <= s_max_alignment);
	}
}