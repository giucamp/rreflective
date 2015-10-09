


namespace reflective
{
	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}

	void UnitTesingManager::add_test(std::function<void()> i_test)
	{
		TestEntry entry;
		m_tests.push_back(entry);
	}
}