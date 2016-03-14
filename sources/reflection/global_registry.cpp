
namespace reflective
{
	GlobalRegistry::GlobalRegistry()
		: m_global_namespace("global")
	{
	}

	GlobalRegistry & GlobalRegistry::instance()
	{
		static GlobalRegistry s_instance;
		return s_instance;
	}

	Namespace * GlobalRegistry::parse_type_full_name(StringView i_full_name, StringView * o_type_name, StringView * o_template_argument_list)
	{
		struct PathEntry
		{
			StringView m_full_path; // for example "reflective::MyClass<int>::MyInnerClass<int>"
			StringView m_name; // for example "MyInnerClass<int>"
			StringView m_template_argument_list; // for example "int"
		};
		vector<PathEntry> path_entries;

		auto remaining_path = i_full_name;
		size_t full_path_length = 0;
		do
		{
			const auto prev_remaining_path_len = remaining_path.length();

			PathEntry curr_entry;
			curr_entry.m_name = remaining_path.remove_prefix_identifier();
			if (curr_entry.m_name.empty())
				break;

			if (remaining_path.remove_prefix_char('<'))
			{
				// temp: this is a big simplification
				curr_entry.m_template_argument_list = remaining_path.remove_prefix_while([](char i_char){ return i_char != '>'; });
				bool res = remaining_path.remove_prefix_char('>');
				REFLECTIVE_INTERNAL_ASSERT(res);
			}

			REFLECTIVE_INTERNAL_ASSERT(prev_remaining_path_len > remaining_path.length());
			full_path_length += prev_remaining_path_len - remaining_path.length();

			curr_entry.m_full_path = i_full_name.substr(0, full_path_length);

			path_entries.push_back(curr_entry);

		} while (remaining_path.remove_prefix_literal("::"));

		REFLECTIVE_ASSERT(remaining_path.empty(), "badformed full type name");

		// loop all the path entries but the last to find or create the namespaces
		Namespace * curr_namespace = &m_global_namespace;
		for (size_t index = 0; index + 1 < path_entries.size(); index++)
		{
			const SymbolName & full_path(path_entries[index].m_full_path);
			auto res = m_namespaces.find(full_path);
			if (res == m_namespaces.end())
			{
				res = m_namespaces.emplace(std::make_pair(full_path, reflective::make_unique<Namespace>(full_path))).first;
			}

			curr_namespace = res->second.get();
		}

		const auto last_entry = path_entries.back();

		*o_type_name = last_entry.m_name;
		*o_template_argument_list = last_entry.m_template_argument_list;

		return curr_namespace;
	}

	Class * GlobalRegistry::add_class(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info)
	{
		i_full_name.remove_prefix_literal("class ");
		i_full_name.remove_prefix_literal("::");

		StringView type_name, template_argument_list;
		auto parent_namespace = parse_type_full_name(i_full_name, &type_name, &template_argument_list);

		// create the type
		auto class_type = reflective::make_unique<Class>(type_name, i_size, i_alignment);

		// add the type to m_types_by_rtti
		auto const class_raw_ptr = class_type.get();
		m_types_by_rtti.emplace(std::make_pair(std::type_index(i_type_info), std::move(class_type)));

		// add the type to m_types_by_full_name		
		m_types_by_full_name.emplace(std::make_pair(i_full_name, class_raw_ptr));

		// add the type to the namespace
		parent_namespace->register_member(*class_raw_ptr);

		return class_raw_ptr;
	}

	Type * GlobalRegistry::add_primitive_type(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info)
	{
		i_full_name.remove_prefix_literal("::");

		StringView type_name, template_argument_list;
		auto parent_namespace = parse_type_full_name(i_full_name, &type_name, &template_argument_list);

		// create the type
		auto primitive_type = reflective::make_unique<Type>(type_name, i_size, i_alignment);

		// add the type to m_types_by_rtti
		auto const primitive_type_raw_ptr = primitive_type.get();
		m_types_by_rtti.emplace(std::make_pair(std::type_index(i_type_info), std::move(primitive_type)));

		// add the type to m_types_by_full_name		
		m_types_by_full_name.emplace(std::make_pair(i_full_name, primitive_type_raw_ptr));

		// add the type to the namespace
		parent_namespace->register_member(*primitive_type_raw_ptr);

		return primitive_type_raw_ptr;
	}

	const Type * GlobalRegistry::find_type_by_full_name(StringView i_full_name) const
	{
		const SymbolName name(i_full_name);

		const auto iterator = m_types_by_full_name.find(name);
		if (iterator != m_types_by_full_name.end())
		{
			return iterator->second;
		}
		else
		{
			return nullptr;
		}
	}

	const Type * GlobalRegistry::find_type_by_rtti(const std::type_info & i_type_info) const
	{
		const auto iterator = m_types_by_rtti.find(i_type_info);
		if (iterator != m_types_by_rtti.end())
		{
			return iterator->second.get();
		}
		else
		{
			return nullptr;
		}
	}

	/*Namespace * GlobalRegistry::get_or_add_namespace(StringView i_full_name)
	{
		normalize_full_name(i_full_name);
		
		Namespace * result = nullptr;

		const SymbolName full_path(i_full_name);
		const auto range = m_registry.equal_range(full_path);
		for (auto it = range.first; it != range.second && result == nullptr; it++)
		{
			result = dynamic_cast<Namespace*>(it->second);
		}

		if (result == nullptr)
		{
			StringView name = i_full_name.remove_prefix_identifier();
		}

		return result;
	}

	void GlobalRegistry::register_type(Type & i_type, const std::type_info & i_type_info)
	{		
		auto res = m_types_by_rtti.insert(std::make_pair(&i_type_info, &i_type));
		REFLECTIVE_ASSERT(res.second, "A type with the same std::type_info has already been registered");

		register_member(i_type);
	}

	void GlobalRegistry::unregister_type(Type & i_type, const std::type_info & i_type_info)
	{	
		unregister_member(i_type);

		REFLECTIVE_ASSERT(m_types_by_rtti[&i_type_info] == &i_type, "Invalid paramters");
		const auto removed_count = m_types_by_rtti.erase(&i_type_info);
		REFLECTIVE_ASSERT(removed_count == 1, "Type not unregistered");
	}

	const Type * GlobalRegistry::find_type(StringView i_full_name)
	{
		const SymbolName name(i_full_name);

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			auto as_type = dynamic_cast<const Type*>(it->second);
			if (as_type != nullptr)
			{
				return as_type;
			}
		}

		return nullptr;
	}

	void GlobalRegistry::register_namespace(StringView i_full_path, Namespace & i_namespace)
	{
		i_full_path.remove_prefix_literal("::");

		const SymbolName name(i_full_path);
		m_registry.insert(std::make_pair(name, &i_namespace));
	}

	Namespace * GlobalRegistry::find_namespace(StringView i_full_path)
	{
		normalize_full_name(i_full_path);

		const SymbolName full_path(i_full_path);
		const auto range = m_registry.equal_range(full_path);
		for (auto it = range.first; it != range.second; it++)
		{
			auto as_namespace = dynamic_cast<Namespace*>(it->second);
			if (as_namespace != nullptr)
			{
				return as_namespace;
			}
		}
	}

	const Namespace * GlobalRegistry::find_namespace(StringView i_full_path) const
	{
		normalize_full_name(i_full_path);

		const SymbolName full_path(i_full_path);
		const auto range = m_registry.equal_range(full_path);
		for (auto it = range.first; it != range.second; it++)
		{
			auto as_namespace = dynamic_cast<Namespace*>(it->second);
			if (as_namespace != nullptr)
			{
				return as_namespace;
			}
		}
	}



	void GlobalRegistry::register_member(NamespaceMember & i_member)
	{
		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());
		m_registry.insert(std::make_pair(name, &i_member));
	}

	void GlobalRegistry::unregister_member(NamespaceMember & i_member)
	{
		const std::string full_name = i_member.full_name();
		const SymbolName name(full_name.c_str());

		bool found = false;

		const auto range = m_registry.equal_range(name);
		for (auto it = range.first; it != range.second; it++)
		{
			if (it->second == &i_member)
			{
				m_registry.erase(it);
				found = true;
			}
		}

		REFLECTIVE_ASSERT(found, "Member not found");
	}*/

	/*const Type * GlobalRegistry::find_type(StringView i_full_name)
	{

	}*/
}