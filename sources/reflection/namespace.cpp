
namespace reflective
{
	Namespace::MembersList::MembersList()
		: m_first_member(nullptr), m_last_member(nullptr)
	{

	}

	void Namespace::MembersList::add(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(i_member.m_next_member != nullptr, "The Namespace data is corrupted");

		if (m_last_member != nullptr)
		{
			REFLECTIVE_INTERNAL_ASSERT(m_first_member != nullptr && m_last_member->m_next_member == nullptr, "Corrupted namespace data");
			m_last_member->m_next_member = &i_member;
			m_last_member = &i_member;
		}
		else
		{
			REFLECTIVE_INTERNAL_ASSERT(m_first_member == nullptr, "Corrupted namespace data");
			m_first_member = m_last_member = &i_member;
		}
	}

	void Namespace::MembersList::remove(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(m_first_member != nullptr && m_last_member != nullptr, "The Namespace data is corrupted");

		NamespaceMember * curr = m_first_member, *prev = nullptr;
		do {

			REFLECTIVE_INTERNAL_ASSERT((curr->m_next_member == nullptr) == (curr == m_last_member), "Corrupted namespace data");

			if (curr == &i_member)
			{
				break;
			}

			prev = curr;
			curr = curr->m_next_member;

		} while (curr != nullptr);

		if (curr == &i_member)
		{
			if (curr == m_last_member)
			{
				m_last_member = prev;
			}

			if (curr == m_first_member)
			{
				m_first_member = curr->m_next_member;
			}
			else
			{
				prev->m_next_member = curr->m_next_member;
			}
		}
	}

	Namespace::Namespace(SymbolName i_name)
		: NamespaceMember(std::move(i_name))
	{
	}

	void Namespace::add_member(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(i_member.m_parent != nullptr, "This object is already a member of a namespace");

		i_member.m_parent = this;

		m_members.add(i_member);
	}

	void Namespace::remove_member(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(i_member.m_parent, "This object is not a member of a namespace");
		
		m_members.remove(i_member);

		i_member.m_parent = nullptr;
	}

	#if REFLECTIVE_ENABLE_TESTING
		
		void Namespace::unit_test()
		{
			Namespace test("test");

			Namespace m1("m1");
			Namespace m2("m1");
			Namespace m3("m1");

			test.add_member(m1);
			test.add_member(m2);
			test.add_member(m3);

			/*for (auto m : test.members())
			{
				
			}*/
		}

	#endif
}