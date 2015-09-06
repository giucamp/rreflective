
#include "test_common.h"

void test()
{
	using namespace reflective;
	using namespace std;
	using std::vector;
	
	static Rand rand;

	const char check_char = 7;

	// create buffer[buff_size], out_stream and in_stream
	const uint32_t buffer_capacity = 64;
	char buffer[buffer_capacity];
	memset(buffer, check_char, sizeof(buffer));
	size_t buff_size = rand.generate_uint32(buffer_capacity);
	TextOutBuffer out_stream(buffer, buff_size);
	TextOutBuffer null_out_stream;
	TextInBuffer in_stream(buffer);
	string check_string;

	// some random strings
	vector<string> strings = { rand.generate_string(10), rand.generate_string(10), rand.generate_string(10) };

	// actions and checks
	vector<function<void()>> actions = {
		[&] { check_string += 'a'; out_stream.write('a'); null_out_stream.write('a'); },
		[&] { check_string += 'b'; out_stream.write('b'); null_out_stream.write('b'); },
		[&] { check_string += "c"; out_stream.write("c"); null_out_stream.write("c"); },
		
		[&] { check_string += "aaabbbb"; out_stream.write("aaabbbb"); null_out_stream.write("aaabbbb"); },

		[&] { check_string += "str"; out_stream.write("str"); null_out_stream.write("str"); },
		[&] { check_string += "str22"; out_stream.write("str22"); null_out_stream.write("str22"); },
		[&] { check_string += "str333"; out_stream.write("str333"); null_out_stream.write("str333"); },

		[&] { check_string += strings[0]; out_stream.write(strings[0].c_str()); null_out_stream.write(strings[0].c_str()); },
		[&] { check_string += strings[1]; out_stream.write(strings[1].c_str()); null_out_stream.write(strings[1].c_str()); },
		[&] { check_string += strings[2]; out_stream.write(strings[2].c_str()); null_out_stream.write(strings[2].c_str()); },
	};
	vector<function<bool()>> checks = {
		
		[&] { return in_stream.accept('a'); },
		[&] { return in_stream.accept("b"); },
		[&] { return in_stream.accept('c'); },

		[&] { bool r = true;
				r = r && in_stream.accept('a'); 
				r = r && in_stream.accept('a');
				r = r && in_stream.accept("ab");
				r = r && in_stream.accept('b');
				r = r && in_stream.accept("bb");
				return r; },

		[&] { return in_stream.accept("str"); },
		[&] { return in_stream.accept("str22"); },
		[&] { return in_stream.accept("str333"); },

		[&] { return in_stream.accept(strings[0].c_str()); },
		[&] { return in_stream.accept(strings[1].c_str()); },
		[&] { return in_stream.accept(strings[2].c_str()); },
	};


	vector<size_t> action_indices(400);
	std::generate(action_indices.begin(), action_indices.end(), [&] { return rand.generate_uint32(actions.size()); });
	
	for (auto action_index : action_indices)
	{
		actions[action_index]();

		REFLECTIVE_ASSERT(out_stream.is_full() == check_string.length() + 1 >= buff_size, "test failed");

		REFLECTIVE_ASSERT(out_stream.is_truncated() == check_string.length() + 1 > buff_size, "test failed");

		bool res = out_stream.is_full() || checks[action_index]();
		REFLECTIVE_ASSERT(res, "test failed");

		REFLECTIVE_ASSERT(out_stream.needed_buffer_length() == null_out_stream.needed_buffer_length(), "test failed");

		REFLECTIVE_ASSERT(null_out_stream.is_truncated(), "test failed");
	}

	// check the buffer between size and capacity
	for (size_t i = buff_size; i < buffer_capacity; i++)
	{
		REFLECTIVE_ASSERT(buffer[i] == check_char, "test failed");
	}

	cout << buffer << endl;
}

void Stream_test()
{
	for (;;)
		test();

	using namespace reflective;
	using namespace std;

	char buffer[17];
	TextOutBuffer out_stream(buffer);

	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	out_stream.write("HELLO");
	while (out_stream.needed_buffer_length() < 50)
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