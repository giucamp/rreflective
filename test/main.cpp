

#include "..\reflective\reflective.h"
#include "..\reflective\unit_testing\unit_testing.h"
#include <sstream>
#include <iostream>

void Name_test();
void SpecialFunctions_test();
void Type_test();

namespace reflective
{
	namespace details
	{
		namespace hier
		{
			void test_hier();
		}
	}
}

struct GGGG
{

};

namespace reflective
{
	void dense_list_benchmark();
}

int main()
{
	using namespace reflective;
	
	reference_globals();

	UnitTestingManager::instance().add_correctness_test("dense_list_test", 
		[](reflective::CorrectnessTestContext &) {dense_list_test(); });
	UnitTestingManager::instance().run("dense_list_test");

	std::cout << get_type<QualifiedTypePtr>() << std::endl;
	std::cout << get_type<StringView>() << std::endl;
	std::cout << get_type<WStringView>() << std::endl;
	std::cout << get_type<u16StringView>() << std::endl;
	std::cout << get_type<u32StringView>() << std::endl;
	
	std::cout << details::HasUnitTestFunc<Class>::value << std::endl;
	
	std::ostream & out_stream = std::cout;
	InTxtStreamAdapt<std::istream> in_stream(std::cin);
	out_stream << 43;
	out_stream << std::string("_dcsd");
	out_stream << "_dcsd";
	out_stream << 243.234;
	int h = 0;
	in_stream >> h;

	reflective::details::hier::test_hier();
	
	{
		using namespace reflective;
		using namespace std;
		using std::vector;
		
		struct
		{
			StringView path;
			vector<StringView> tokens;
		} tests[] = {
			{ "", { } },
			{ "/",{ "", "" } },
			{ "//",{ "", "", "" } },
			{ "abc", { "abc" } },
			{ "/def/efg",{ "", "def", "efg" } },
			{ "def/efg/",{ "def", "efg", "" } },
			{ "def//efg/",{ "def", "", "efg", "" } },
			{ "abc/def/efg",{ "abc", "def", "efg" } },
			{ "/abc/def/", { "", "abc", "def", "" } },
		};

		for (const auto & test : tests)
		{
			vector<StringView> copy;
			test.path.for_each_token( '/', [&copy](StringView i_token) {
				copy.push_back( i_token );
			});
			REFLECTIVE_INTERNAL_ASSERT(copy==test.tokens);			
		}

		UnitTestingManager::instance().add_performance_test("abc", [] {}, "");
		UnitTestingManager::instance().add_performance_test("abc/efg", [] {}, "");
		UnitTestingManager::instance().add_performance_test("abc/efg/rre", [] {}, "");
		UnitTestingManager::instance().run("abc");
		UnitTestingManager::instance().run("abc/efg/rre");
	}

	{
		using namespace std;
		using namespace reflective;
		cout << get_type<void***const**volatile*******>() << endl;
	}

	Type_test();

	std::ostringstream s;
	s << 1;
	s << 1.f;

	reflective::Type::unit_test();
	reflective::Namespace::unit_test();

	Name_test();

	SpecialFunctions_test();

	return 0;
}
