
namespace reflective
{
	Type::Type(SymbolTypeId i_type_id, SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions)
		: NamespaceMember(i_type_id, std::move(i_name)), m_special_functions(i_special_functions), m_size(i_size), m_alignment(i_alignment)
	{
		REFLECTIVE_ASSERT(i_size <= s_max_size, "The size of the type exceeds s_max_size");
		REFLECTIVE_ASSERT(is_power_of_2(i_alignment), "The alignment of the type is not a power of 2");
		REFLECTIVE_ASSERT(i_alignment <= s_max_alignment, "The alignment of the type exceeds s_max_alignment");
	}
}
