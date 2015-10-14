
namespace reflective
{
	namespace selft_testing
	{
		/*void test_type_derived_list()
		{
			std::array<size_t, 6> remove_order = { 0, 1, 2, 3, 4, 5 };
			do {

				Type test_type(SymbolTypeId::is_type, "test_type", 16, 4, SpecialFunctions(), {} );
				std::vector<Type*> test_vector;

				// local function that assert( test_type == test_vector )
				auto test_equal = [&] {
					size_t index = 0;
					for (auto & type : test_type.derived_types())
					{
						REFLECTIVE_TEST_ASSERT(index < test_vector.size() && &type == test_vector[index]);
						index++;
					}
					REFLECTIVE_TEST_ASSERT(test_vector.size() == index);
				};

				std::array<Type, 6> types = { "m1", "m2", "m3", "m4", "m5", "m6" };

				test_equal();

				// add types
				for (auto & type : types)
				{
					test_type.register_type(type);
					test_vector.push_back(&type);
					test_equal();
				}

				// remove types
				for (size_t index_to_remove : remove_order)
				{
					auto it = find(test_vector.begin(), test_vector.end(), &types[index_to_remove]);
					REFLECTIVE_TEST_ASSERT(it != test_vector.end());
					test_vector.erase(it);

					test_type.unregister_type(types[index_to_remove]);

					test_equal();
				}

			} while (std::next_permutation(remove_order.begin(), remove_order.end()));
		}*/
	}
}
