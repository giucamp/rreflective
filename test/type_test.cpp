
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
	void setup_type(reflecting_type<MyNamespace::OtherNamespace::MyClass> & i_class, MyNamespace::OtherNamespace::MyClass *)
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
	
	const auto & t1 = get_qualified_type<float>();
	const auto & t2 = get_qualified_type<const float &>();
	const auto & t3 = get_qualified_type<float *const*volatile**>();
	const auto & t4 = get_qualified_type<float const*const*volatile**&>();
	const auto & t5 = get_qualified_type<volatile float>();

	const auto & tf1 = get_qualified_type<MyNamespace::OtherNamespace::MyClass>();
	const auto & tf2 = get_qualified_type<const MyNamespace::OtherNamespace::MyClass &>();
	const auto & tf3 = get_qualified_type<MyNamespace::OtherNamespace::MyClass *const*volatile**>();
	const auto & tf4 = get_qualified_type<MyNamespace::OtherNamespace::MyClass const*const*volatile**&>();
	const auto & tf5 = get_qualified_type<volatile MyNamespace::OtherNamespace::MyClass>();

	const auto & class_type = get_type<MyNamespace::OtherNamespace::MyClass>();
	const auto & enum_type = get_type<MyNamespace::OtherNamespace::MyEnum>();



	system("PAUSE");
}