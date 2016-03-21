
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"

namespace MyNamespace
{
	namespace OtherNamespace
	{
		class MyBaseClass
		{
		};

		class MyBaseClass1
		{
		};

		class MyClass: public MyBaseClass, public MyBaseClass1
		{
		public:
			int m_int_var = 0;
			float m_float_var = 0.f;
			const float m_const_float_var = 0.f;

			MyClass & operator = (const MyClass &) = delete;
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
	void setup_type(TypeSetupContext<MyNamespace::OtherNamespace::MyClass> & i_context)
	{
		using ThisClass = MyNamespace::OtherNamespace::MyClass;
		
		i_context.type()->set_base_types( {
			BaseType::from_types<ThisClass, MyNamespace::OtherNamespace::MyBaseClass>(),
			BaseType::from_types<ThisClass, MyNamespace::OtherNamespace::MyBaseClass1>() });
		
		auto properties = make_list<Property>(
			make_property<ThisClass>("IntVar", &ThisClass::m_int_var),
			make_property<ThisClass>("FloatVar", &ThisClass::m_float_var),
			make_property<ThisClass>("ConstFloatVar", &ThisClass::m_const_float_var)
		);
		i_context.type()->set_properites(std::move(properties));
	}
}

void Type_test()
{
	using namespace reflective;
	using namespace std;

	//auto b = reflective::has_to_string< reflective::QualifiedTypePtr>::value;

	const auto & t1 = get_type<float>();
	const auto & t2 = get_type<const float &>();
	const auto & t3 = get_type<float *const*volatile**>();
	const auto & t4 = get_type<float const*const*volatile**&>();
	const auto & t5 = get_type<volatile float>();

	const auto & tf1 = get_type<MyNamespace::OtherNamespace::MyClass>();
	const auto & tf2 = get_type<const MyNamespace::OtherNamespace::MyClass &>();
	const auto & tf3 = get_type<MyNamespace::OtherNamespace::MyClass *const*volatile**>();
	const auto & tf4 = get_type<MyNamespace::OtherNamespace::MyClass const*const*volatile**&>();
	const auto & tf5 = get_type<volatile MyNamespace::OtherNamespace::MyClass>();

	cout << to_std_string(t1) << endl;
	cout << to_std_string(t2) << endl;
	cout << to_std_string(t3) << endl;
	cout << to_std_string(t4) << endl;
	cout << to_std_string(t5) << endl;


	cout << to_std_string(tf1) << endl;
	cout << to_std_string(tf2) << endl;
	cout << to_std_string(tf3) << endl;
	cout << to_std_string(tf4) << endl;
	cout << to_std_string(tf5) << endl;


	const auto & class_type = get_naked_type<MyNamespace::OtherNamespace::MyClass>();
	const auto & enum_type = get_naked_type<MyNamespace::OtherNamespace::MyEnum>();


	cout << to_std_string(tf4) << endl;
	cout << to_std_string(tf5) << endl;

	cout << to_std_string(class_type.name()) << endl;
	cout << to_std_string(enum_type.name()) << endl;

	system("PAUSE");
}