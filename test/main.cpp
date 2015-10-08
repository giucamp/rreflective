

#include "..\sources\reflective.h"
#include <sstream>

void Name_test();
void SpecialFunctions_test();
void Type_test();
void Stream_test();
void bulk_list__test();

int main()
{
	std::ostringstream s;
	s << 1;
	s << 1.f;

	reflective::Namespace::unit_test();
	reflective::QualifiedTypePtr::unit_test();

	Name_test();

	bulk_list__test();

	Type_test();

	Stream_test();	
		

	SpecialFunctions_test();

	return 0;
}
