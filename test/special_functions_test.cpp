
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\reflective\reflective.h"

class A
{
private:
	
	
public:

	A(){}
	A(const A &){}
	A(A &&){
		using namespace std;
		cout << "move" << endl;
	}
	~A(){}	
};

void SpecialFunctions_test()
{
	using namespace reflective;
	using namespace std;

	SpecialFunctions functions = SpecialFunctions::from_type<A>();

	A a;
	A b(std::move(a));

	functions.scalar_move_constructor()(&a, &a + 1, &b);

	/*Identifier<StringHasher<uint32_t>, PtrString> n("abs");	
	cout << n.string().c_str() << n.hash() << endl;

	Identifier<StringHasher<uint32_t>, std::string> n2("abs");
	cout << n2.string().c_str() << n2.hash() << endl;

	Identifier<StringHasher<uint32_t>, std::string> n3("abs");
	cout << n3.string().c_str() << n3.hash() << endl;

	Identifier<StringHasher<uint32_t>, PtrString> n4(PtrString("abs"));
	cout << n4.string().c_str() << n4.hash() << endl;*/

	system("PAUSE");
}