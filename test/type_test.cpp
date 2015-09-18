
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"

namespace MyNamespace
{
	namespace OtherNamespace
	{
		class MyClass
		{
		public:
			int m_int_var = 0;
			float m_float_var = 0.f;
		};

		enum MyEnum
		{
			eFirstEnumMember = 1,
			eSecondEnumMember = 2,
			eNthEnumMember = 5,
		};
	}
}


namespace reflective
{
	void setup_type(Class & i_class, MyNamespace::OtherNamespace::MyClass *)
	{
		using ThisClass = MyNamespace::OtherNamespace::MyClass;
		SymbolList<Property> empty = { 
			new_property<ThisClass>("IntVar", &ThisClass::m_int_var),
			new_property<ThisClass>("FloatVar", &ThisClass::m_float_var),
		};
		i_class.set_properites(std::move(empty));
	}
}


void Type_test()
{
	using namespace reflective;
	using namespace std;

	const Type & class_type = get_type<MyNamespace::OtherNamespace::MyClass>();
	
	{
		const Type & enum_type = get_type<MyNamespace::OtherNamespace::MyEnum>();
		
	}



	system("PAUSE");
}