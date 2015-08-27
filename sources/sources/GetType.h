
#include "class/Class.h"
#include "type/Type.h"
#include "type/SpecialFunctions.h"

namespace reflective
{
	template <typename TYPE>
		const Type & get_type();

	template <typename TYPE>
		const Class * get_class();
}

#include "GetType.inl"
