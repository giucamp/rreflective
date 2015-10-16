


namespace reflective
{
	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}

	void UnitTesingManager::add_test(StringView i_full_path, std::function<void()> i_test)
	{

	}

}