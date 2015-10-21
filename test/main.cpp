

#include "..\sources\reflective.h"
#include "..\sources\unit_testing\unit_testing.h"
#include <sstream>
#include <iostream>

void Name_test();
void SpecialFunctions_test();
void Type_test();
void Stream_test();
void bulk_list__test();

int main()
{
	{
		using namespace reflective;
		using namespace std;
		
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
			for_each_token(test.path, '/', [&copy](StringView i_token) {
				copy.push_back( i_token );
			});
			REFLECTIVE_INTERNAL_ASSERT(copy==test.tokens);			
		}

		UnitTesingManager::instance().add_performance_test("abc", [] {}, "");
		UnitTesingManager::instance().add_performance_test("abc/efg", [] {}, "");
		UnitTesingManager::instance().add_performance_test("abc/efg/rre", [] {}, "");
		UnitTesingManager::instance().run("abc");
		UnitTesingManager::instance().run("abc/efg/rre");
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
	reflective::QualifiedTypePtr::unit_test();

	Name_test();

	bulk_list__test();

	Stream_test();	
		

	SpecialFunctions_test();

	return 0;
}
