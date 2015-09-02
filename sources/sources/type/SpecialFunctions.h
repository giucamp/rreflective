
#pragma once
#include "..\reflective_common.h"

namespace reflective
{
	class SpecialFunctions
	{
	public:

		using ScalarTor = void (*)(void * i_objects_start, void * i_objects_end );

		using CopyConstructor = void (*)(void * i_objects_start, void * i_objects_end, const void * i_source_start );

		using MoveConstructor = void(*)(void * i_objects_start, void * i_objects_end, void * i_source_start );

		SpecialFunctions(ScalarTor i_scalar_default_constructor, CopyConstructor i_scalar_copy_constructor,
			MoveConstructor const i_scalar_move_constructor, ScalarTor const i_scalar_destructor);

		ScalarTor const scalar_default_constructor() const			{ return m_scalar_default_constructor; }
		CopyConstructor const scalar_copy_constructor() const		{ return m_scalar_copy_constructor; }
		MoveConstructor const scalar_move_constructor() const		{ return m_scalar_move_constructor; }
		ScalarTor const scalar_destructor() const					{ return m_scalar_destructor;  }

	private:
		ScalarTor const m_scalar_default_constructor;
		CopyConstructor const m_scalar_copy_constructor;
		MoveConstructor const m_scalar_move_constructor;
		ScalarTor const m_scalar_destructor;
	};

	template <typename TYPE>
		SpecialFunctions get_special_functions();
}
