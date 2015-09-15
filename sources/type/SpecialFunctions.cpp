
namespace reflective
{
	SpecialFunctions::SpecialFunctions(ScalarTor i_scalar_default_constructor, CopyConstructor i_scalar_copy_constructor,
		MoveConstructor const i_scalar_move_constructor, ScalarTor const i_scalar_destructor)
		: m_scalar_default_constructor(i_scalar_default_constructor), m_scalar_copy_constructor(i_scalar_copy_constructor),
		m_scalar_move_constructor(i_scalar_move_constructor), m_scalar_destructor(i_scalar_destructor)
	{

	}
}