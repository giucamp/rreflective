
namespace reflective
{
	UnitTesingManager & UnitTesingManager::instance()
	{
		static UnitTesingManager instance;
		return instance;
	}

	std::vector<UnitTesingManager::TestEntry>::iterator UnitTesingManager::TestEntry::get_or_add_single(StringView i_token)
	{
		auto const existing_it = Ext::find_if(m_children, [&i_token](const TestEntry & i_entry) { return i_token == i_entry.m_name.c_str(); });
		if (existing_it != m_children.end())
		{
			return existing_it;
		}
		else
		{
			m_children.emplace_back();
			return m_children.end() - 1;
		}
	}

	std::vector<UnitTesingManager::TestEntry>::iterator UnitTesingManager::TestEntry::get_or_add_path(StringView i_full_path)
	{
		auto res = Ext::find(i_full_path, '/');
		if (res != i_full_path.end())
		{
			auto const separator_pos = res - i_full_path.begin();
			auto const first_token = i_full_path.substr(0, separator_pos);
			auto const remaining_path = i_full_path.substr(separator_pos + 1);
			auto const child_id = get_or_add_single(first_token);
			return child_id->get_or_add_path(remaining_path);
		}
		else
		{
			return get_or_add_single(i_full_path);
		}
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