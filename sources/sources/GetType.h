
#include "class/Class.h"
#include "type/Type.h"
#include "Namespace.h"
#include "type/SpecialFunctions.h"

namespace reflective
{	
	const Namespace & root_namespace();

	Namespace & edit_root_namespace();

	template <typename TYPE>
		const Type & get_type();

	template <typename TYPE>
		const Class * get_class();
}

#include "GetType.inl"
