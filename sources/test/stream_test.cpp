
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"

void Stream_test()
{

	using namespace reflective;
	using namespace std;

	char buffer[16];
	TextOutBuffer out_stream(buffer);

	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write()

	cout << buffer;

	system("PAUSE");
}