
#include <inttypes.h>
#include <limits>
#include <array>
#include <map>
#include <ctype.h>


#include "reflective.h"

#include "core\string_view.cpp"
#include "core\out_buffer_text_stream.cpp"
#include "core\in_buffer_text_stream.cpp"

#include "reflection\symbol.cpp"
#include "reflection\special_functions.cpp"
#include "reflection\string_functions.cpp"
#include "reflection\primitive_types.cpp"
#include "reflection\namespace_member.cpp"
#include "reflection\type.cpp"
#include "reflection\qualified_type_ptr.cpp"
#include "reflection\obj_ptr.cpp"
#include "reflection\property\property.cpp"
#include "reflection\namespace.cpp"
#include "reflection\global_registry.cpp"

#if REFLECTIVE_ENABLE_TESTING
	#include "unit_testing\unit_testing.h"
	#include "reflection\type_derived_list_test.cpp"
	#include "reflection\type_inheritance_test.cpp"
#endif