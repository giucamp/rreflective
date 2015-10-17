
namespace reflective
{
	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}

	std::vector<UnitTesingManager::TestEntry>::iterator UnitTesingManager::internal_add_entry(StringView i_full_path)
	{
		//Ext::find( i_full_path, '/'); to di
		return std::vector<UnitTesingManager::TestEntry>::iterator();
	}

	void UnitTesingManager::add_test(StringView i_full_path, std::function<void()> i_test)
	{
		REFLECTIVE_ASSERT( !Ext::contains_if(i_full_path, [](char i_char) {
				return !std::isalnum(i_char) && i_char != '_' && i_char != '/';
			} ), "Invalid character inside a test path");

		//i_full_path.find('s');
		//i_full_path.find("ss");
	}

}