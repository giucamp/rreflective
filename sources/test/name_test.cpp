
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\core\Identifier.h"
#include "..\sources\core\PtrString.h"
#include "..\sources\core\symbol.h"

void Name_test()
{
	using namespace reflective;
	using namespace std;

	Identifier<StringHasher<uint32_t>, PtrString> n("abs");	
	cout << n.string().c_str() << n.hash() << endl;

	Identifier<StringHasher<uint32_t>, std::string> n2("abs");
	cout << n2.string().c_str() << n2.hash() << endl;

	Identifier<StringHasher<uint32_t>, std::string> n3("abs");
	cout << n3.string().c_str() << n3.hash() << endl;

	Identifier<StringHasher<uint32_t>, PtrString> n4(PtrString("abs"));
	cout << n4.string().c_str() << n4.hash() << endl;

	//system("PAUSE");
}