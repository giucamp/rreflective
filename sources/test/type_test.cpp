
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"

namespace MyNamespace
{
	namespace OtherNamespace
	{
		class A
		{
		public:
			int m_intVar = 0;
		};
	}
}


void Type_test()
{
	using namespace reflective;
	using namespace std;

	const Type & A_type = get_type<MyNamespace::OtherNamespace::A>();

	//system("PAUSE");
}