
namespace reflective
{
	namespace details
	{
		void test_type_derived_list()
		{
			std::array<size_t, 6> remove_order = { 0, 1, 2, 3, 4, 5 };
			do {

				details::DerivedTypesList test_type;
				std::vector<Type*> test_vector;

				// local function that assert( test_type == test_vector )
				auto test_equal = [&] {

					std::vector<Type*> test_vector_copy(test_vector);
				
					for (auto & type : test_type)
					{
						auto it = std::find(test_vector_copy.begin(), test_vector_copy.end(), &type);
						REFLECTIVE_TEST_ASSERT(it != test_vector_copy.end() );						
						test_vector_copy.erase(it);
					}
					REFLECTIVE_TEST_ASSERT(test_vector_copy.size() == 0);
				};

				std::array<Type, 6> types = {
					Type{ SymbolTypeId::is_type, "t1", 16, 4, {} },
					Type{ SymbolTypeId::is_type, "t2", 16, 4, {} },
					Type{ SymbolTypeId::is_type, "t3", 16, 4, {} },
					Type{ SymbolTypeId::is_type, "t4", 16, 4, {} },
					Type{ SymbolTypeId::is_type, "t5", 16, 4, {} },
					Type{ SymbolTypeId::is_type, "t6", 16, 4, {} } };

				test_equal();

				// add types
				for (auto & type : types)
				{
					test_type.add(type);
					test_vector.push_back(&type);
					test_equal();
				}

				// remove types
				for (size_t index_to_remove : remove_order)
				{
					auto it = find(test_vector.begin(), test_vector.end(), &types[index_to_remove]);
					REFLECTIVE_TEST_ASSERT(it != test_vector.end());
					test_vector.erase(it);

					test_type.remove(types[index_to_remove]);

					test_equal();
				}

			} while (std::next_permutation(remove_order.begin(), remove_order.end()));
		}
	}
}