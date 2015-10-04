

#include "..\sources\reflective.h"

void Name_test();
void SpecialFunctions_test();
void Type_test();
void Stream_test();
void bulk_list__test();

reflective::Type * volatile t = new reflective::Class("", 1, 1, reflective::SpecialFunctions());

int main()
{
	reflective::Namespace::unit_test();
	reflective::QualifiedTypePtr::unit_test();

	reflective::Class * volatile y = dynamic_cast<reflective::Class*>(t);

	Name_test();

	bulk_list__test();

	Type_test();

	Stream_test();	
		

	SpecialFunctions_test();

	return 0;
}
