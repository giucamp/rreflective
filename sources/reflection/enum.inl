
namespace reflective
{
	template <typename UNDERLYING_TYPE>
		Enum<UNDERLYING_TYPE>::Enum(SymbolName i_name, const SpecialFunctions & i_special_functions)
			: EnumBase(i_name, sizeof(UNDERLYING_TYPE), std::alignment_of<UNDERLYING_TYPE>::value, i_special_functions)
	{

	}
}