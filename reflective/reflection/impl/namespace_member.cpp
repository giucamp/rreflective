
void pt(const char*, size_t);

namespace reflective
{
	std::string NamespaceMember::full_name() const
	{
		int type_count = 1;
		std::array< const Symbol *, 16 > names;

		names[0] = this;
		size_t size_to_reserve = name().string().size();
		for (const Namespace * curr = m_parent; curr != nullptr; curr = curr->parent_namespace())
		{
			REFLECTIVE_ASSERT(type_count < static_cast<int>(names.size()), "Type too much scoped");
			size_to_reserve += curr->name().string().size() + 2;
			names[type_count] = curr;
			type_count++;
		}

		std::string res;
		res.reserve(size_to_reserve);
		for (int i = type_count - 1; i >= 0; i--)
		{
			const auto & name = names[i]->name().string();
			res.append(name.data(), name.length());
			if( i > 0 )
			{
				res += "::";
			}
		}

		return res;
	}
}
