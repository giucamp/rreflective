
#include <inttypes.h>
#include <limits>
#include <array>
#include <map>
#include <ctype.h>

#include "reflective.h"

#include "reflection\impl\symbol.cpp"
#include "reflection\impl\special_functions.cpp"
#include "reflection\impl\string_functions.cpp"
#include "reflection\impl\primitive_types.cpp"
#include "reflection\impl\namespace_member.cpp"
#include "reflection\impl\type.cpp"
#include "reflection\impl\qualified_type_ptr.cpp"
#include "reflection\impl\obj_ptr.cpp"
#include "reflection\property\property.cpp"
#include "reflection\impl\namespace.cpp"
#include "reflection\impl\global_registry.cpp"

#if REFLECTIVE_ENABLE_TESTING
	#include "unit_testing\uint_test_manager.h"
	#include "reflection\testing\type_derived_list_test.cpp"
	#include "reflection\testing\type_inheritance_test.cpp"
	#include "reflection\testing\qualified_type_ptr_test.cpp"
#endif


namespace reflective
{
	void dense_fixed_queue_test();
	void dense_list_test();
	void dense_list_benchmark();

	void reference_globals()
	{	
		dense_fixed_queue_test();

		dense_list_test();
		for (int i = 0; i < 20; i++ )
			dense_list_benchmark();
	}
}