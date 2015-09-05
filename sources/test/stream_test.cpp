
#include "test_common.h"

void test()
{
	Rand rand;

	
}

void Stream_test()
{

	using namespace reflective;
	using namespace std;

	char buffer[17];
	TextOutBuffer out_stream(buffer);

	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	while (out_stream.needed_length() < 50)
	{
		out_stream.write('c');
	}	

	cout << buffer << endl;

	{
		TextInBuffer in_stream(buffer);
		while (in_stream.accept("HELLL"))
		{
			cout << "----" << endl;
		}
		while (in_stream.accept("HELLO"))
		{
			cout << "*" << endl;
		}
		while (in_stream.accept("HELLL"))
		{
			cout << "----" << endl;
		}
		while (in_stream.accept('c'))
		{
			cout << "." << endl;
		}
		while (in_stream.accept("HELLL"))
		{
			cout << "----" << endl;
		}
	}

	{
		const char * s = "HELLO";
		const char * n = "HELLL";
		TextInBuffer in_stream(buffer);
		while (in_stream.accept(n))
		{
			cout << "----" << endl;
		}
		while (in_stream.accept(s))
		{
			cout << "*" << endl;
		}
		while (in_stream.accept(n))
		{
			cout << "----" << endl;
		}
		while (in_stream.accept('c'))
		{
			cout << "." << endl;
		}
		while (in_stream.accept(n))
		{
			cout << "----" << endl;
		}
	}

	system("PAUSE");
}