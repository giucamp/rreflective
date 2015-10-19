
namespace reflective
{
	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}

	UnitTesingManager::TestEntry * UnitTesingManager::find_entry(StringView i_full_path)
	{
		TestEntry * entry = &m_root;
		for_each_token(i_full_path, '/', [&entry](StringView i_token) {

			if (entry != nullptr)
			{
				auto entry_it = Ext::find_if(entry->m_children, [i_token](const TestEntry & i_entry) { return i_token == i_entry.m_name.c_str(); });

				if (entry_it == entry->m_children.end())
				{
					entry = nullptr;
				}
				else
				{
					entry = &*entry_it;
				}
			}
		});

		return entry;
	}

	UnitTesingManager::TestEntry & UnitTesingManager::find_or_add_entry(StringView i_full_path)
	{
		TestEntry * entry = &m_root;
		for_each_token(i_full_path, '/', [&entry](StringView i_token) {

			auto entry_it = Ext::find_if(entry->m_children, [i_token](const TestEntry & i_entry) { return i_token == i_entry.m_name.c_str(); });

			if (entry_it == entry->m_children.end())
			{
				entry->m_children.emplace_back();
				entry = &entry->m_children.back();
				entry->m_name.assign(i_token.data(), i_token.size());
			}
			else
			{
				entry = &*entry_it;
			}
		});

		return *entry;
	}
	
	void UnitTesingManager::add_test(StringView i_full_path, std::function<void()> i_test)
	{
		auto & entry = find_or_add_entry(i_full_path);
		entry.m_test = i_test;
	}

	void UnitTesingManager::run(StringView i_path)
	{
		auto entry = find_entry(i_path);
		if (entry != nullptr)
		{

		}
	}
}