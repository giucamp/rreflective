
namespace reflective
{
	Namespace::MembersList::MembersList()
		: m_first_member(nullptr), m_last_member(nullptr)
	{

	}

	void Namespace::MembersList::add(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(i_member.m_next_member == nullptr, "The Namespace data is corrupted");

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
		REFLECTIVE_ASSERT(i_member.m_parent == nullptr, "This object is already a member of a namespace");

		i_member.m_parent = this;

		m_members.add(i_member);
	}

	void Namespace::remove_member(NamespaceMember & i_member)
	{
		REFLECTIVE_ASSERT(i_member.m_parent == this, "This object is not a member of this namespace");
		
		m_members.remove(i_member);

		i_member.m_parent = nullptr;
	}

	#if REFLECTIVE_ENABLE_TESTING
		
		void Namespace::unit_test()
		{
			std::array<size_t, 6> remove_order = { 0, 1, 2, 3, 4, 5 };
			do {

				Namespace test_namespace("test_namespace");
				std::vector<Namespace*> test_vector;

				// local function that assert( test_namespace == test_vector )
				auto test_equal = [&] {
					size_t index = 0;
					for (auto & member : test_namespace.members())
					{
						REFLECTIVE_TEST_ASSERT(index < test_vector.size() && &member == test_vector[index]);
						index++;
					}
					REFLECTIVE_TEST_ASSERT(test_vector.size() == index);
				};

				std::array<Namespace, 6> members = { "m1", "m2", "m3", "m4", "m5", "m6" };

				test_equal();

				// add members
				for (auto & member : members)
				{
					test_namespace.add_member(member);
					test_vector.push_back(&member);
					test_equal();
				}

				// remove members
				for (size_t index_to_remove : remove_order)
				{
					auto it = find(test_vector.begin(), test_vector.end(), &members[index_to_remove]);
					REFLECTIVE_TEST_ASSERT(it != test_vector.end());
					test_vector.erase(it);

					test_namespace.remove_member(members[index_to_remove]);

					test_equal();
				}

			} while( std::next_permutation(remove_order.begin(), remove_order.end()) );
		}

	#endif
}