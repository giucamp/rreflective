

#include "..\sources\reflective.h"

void Name_test();
void SpecialFunctions_test();
void Type_test();
void Stream_test();
void bulk_list__test();

int main()
{
	reflective::QualifiedTypePtr::unit_test();

	Name_test();

	bulk_list__test();

	Type_test();

	Stream_test();	
		

	SpecialFunctions_test();

	return 0;
}
