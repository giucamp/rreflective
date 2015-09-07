
#include "test_common.h"

class StreamTest
{
public:

	class Test
	{
	public:

		virtual ~Test() {}

		virtual void append_string(Rand & i_rand, reflective::TextOutBuffer & i_dest) = 0;		

		virtual void check_string(reflective::TextInBuffer & i_source) = 0;

		template <typename ANY>
		static void accept(reflective::TextInBuffer & i_source, ANY && i_any)
		{
			const bool result = i_source.accept(i_any);
			REFLECTIVE_ASSERT(result, "Test failed");
		}

	};

	template <typename INT_TYPE>
		class IntTest : public Test
	{
	private:
		INT_TYPE m_value;

	public:
		void append_string(Rand & i_rand, reflective::TextOutBuffer & i_dest) override
		{
			bool is_signed = std::numeric_limits<INT_TYPE>::is_signed;
			if (is_signed)
				m_value = static_cast<INT_TYPE>( i_rand.generate_sll() );
			else
				m_value = static_cast<INT_TYPE>(i_rand.generate_ull());

			i_dest << "i:";
			i_dest << m_value;
			i_dest << ":e";
		}

		void check_string(reflective::TextInBuffer & i_source) override
		{
			/*accept( i_source, "i:");
			accept( i_source, m_value);
			accept( i_source, ":e");*/
		}
	};

	StreamTest()
	{
		m_tests.emplace_back(std::make_unique<IntTest<int8_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<int16_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<int32_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<int64_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<uint8_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<uint16_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<uint32_t>>());
		m_tests.emplace_back(std::make_unique<IntTest<uint64_t>>());
	}

	void tick(Rand & i_rand)
	{
		using namespace reflective;
		using namespace std;
		using std::vector;

		const char check_char = 7;

		const uint32_t buffer_capacity = 64;
		char buffer[buffer_capacity];
		memset(buffer, check_char, sizeof(buffer));
		size_t buff_size = i_rand.generate_uint32(buffer_capacity);
				
		TextOutBuffer out_stream(buffer, buff_size);
		shuffle(m_tests.begin(), m_tests.end(), i_rand.get_generator());
		for (const auto & test : m_tests)
		{
			test->append_string(i_rand, out_stream);
		}

		TextInBuffer in_stream(buffer);
		for (const auto & test : m_tests)
		{
			test->check_string(in_stream);
		}
	}

private:
	std::vector< std::unique_ptr<Test> > m_tests;
};

void Stream_test_oneshot()
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
	TextInBuffer in_stream(buffer);
	
	out_stream = TextOutBuffer(buffer, buff_size);
	for(size_t i = 0; i + 1 < buff_size; i++)
	{
		out_stream.write_char('A' + (i % 20) );
		out_stream.flush();
		REFLECTIVE_ASSERT(!out_stream.is_truncated(), "test failed");
		REFLECTIVE_ASSERT(strlen(buffer) == i + 1, "test failed");
	}

	out_stream.write_char('Z');
	REFLECTIVE_ASSERT(out_stream.is_truncated(), "test failed");
}

void Stream_test_rnd()
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
		[&] { check_string += 'a'; out_stream.write_char('a'); null_out_stream.write_char('a'); },
		[&] { check_string += 'b'; out_stream.write_char('b'); null_out_stream.write_char('b'); },
		[&] { check_string += "c"; out_stream.write_carray("c"); null_out_stream.write_carray("c"); },
		
		[&] { check_string += "aaabbbb"; out_stream.write_carray("aaabbbb"); null_out_stream.write_carray("aaabbbb"); },

		[&] { check_string += "str"; out_stream.write_carray("str"); null_out_stream.write_carray("str"); },
		[&] { check_string += "str22"; out_stream.write_carray("str22"); null_out_stream.write_carray("str22"); },
		[&] { check_string += "str333"; out_stream.write_carray("str333"); null_out_stream.write_carray("str333"); },

		[&] { check_string += strings[0]; out_stream.write_cstr(strings[0].c_str()); null_out_stream.write_cstr(strings[0].c_str()); },
		[&] { check_string += strings[1]; out_stream.write_cstr(strings[1].c_str()); null_out_stream.write_cstr(strings[1].c_str()); },
		[&] { check_string += strings[2]; out_stream.write_cstr(strings[2].c_str()); null_out_stream.write_cstr(strings[2].c_str()); },
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
	
	check_string.clear();
	for (auto it = action_indices.begin(); it != action_indices.end(); it++ )
	{
		actions[*it]();

		REFLECTIVE_ASSERT(out_stream.is_full() == check_string.length() + 1 >= buff_size, "test failed");

		REFLECTIVE_ASSERT(out_stream.is_truncated() == check_string.length() + 1 > buff_size, "test failed");

		bool res = out_stream.is_full() || checks[*it]();
		REFLECTIVE_ASSERT(res, "test failed");

		REFLECTIVE_ASSERT(out_stream.needed_buffer_length() == null_out_stream.needed_buffer_length(), "test failed");

		REFLECTIVE_ASSERT(null_out_stream.is_truncated(), "test failed");

		if (out_stream.is_truncated())
		{
			size_t index = it - action_indices.begin();
			while(index < action_indices.size())
				action_indices.erase(action_indices.begin() + index);
			break;
		}

		if (rand.generate_uint32(3) == 0)
		{
			out_stream.flush();
			REFLECTIVE_ASSERT(strlen(buffer) == std::min(buff_size > 0 ? buff_size - 1 : 0, check_string.length() ), "test failed");
		}
	}

	in_stream = TextInBuffer(buffer);
	check_string.clear();
	for (auto action_index : action_indices)
	{
		bool res = in_stream.accept("__");
		REFLECTIVE_ASSERT(!res, "test failed");

		res = in_stream.accept('_');
		REFLECTIVE_ASSERT(!res, "test failed");

		res = checks[action_index]();
		REFLECTIVE_ASSERT(res, "test failed");
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
	static Rand rand;
	StreamTest test;
	for (int i = 0; i < 10000; i++)
	{
		test.tick(rand);
	}

	Stream_test_oneshot();

	for (int i = 0; i < 10000; i++)
	{
		Stream_test_rnd();
	}
}