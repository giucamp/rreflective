
#include <inttypes.h>
#include <limits>
#include <array>
#include <map>
#include <ctype.h>

#include "reflective.h"

#include "density\density.cpp"

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
	#include "unit_testing\unit_testing.h"
	#include "reflection\testing\type_derived_list_test.cpp"
	#include "reflection\testing\type_inheritance_test.cpp"
	#include "reflection\testing\qualified_type_ptr_test.cpp"
#endif