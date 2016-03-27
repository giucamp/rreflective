
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

	StringView GlobalRegistry::parse_template_arguments(StringView & io_input)
	{
		uint32_t bracket_depth = 1;		
		size_t curr_pos = 0;
		for (;  curr_pos < io_input.size(); curr_pos++)
		{
			if (io_input[curr_pos] == '<')
			{
				bracket_depth++;
			}
			else if (io_input[curr_pos] == '>')
			{
				REFLECTIVE_ASSERT(bracket_depth > 0, "wrong angular brackets in template argument list");
				bracket_depth--;
				if (bracket_depth == 0)
				{
					break;
				}
			}
		}
		const auto arguments = io_input.substr(0, curr_pos);
		io_input.remove_prefix(curr_pos);

		bool res = io_input.remove_prefix_char('>');
		REFLECTIVE_INTERNAL_ASSERT(res);
		return arguments;
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
			const char * const start_of_name = remaining_path.data();

			PathEntry curr_entry;
			auto name = remaining_path.remove_prefix_identifier();
			if (name.empty())
				break;
			
			if (remaining_path.remove_prefix_char('<'))
			{				
				curr_entry.m_template_argument_list = parse_template_arguments(remaining_path);
			}

			curr_entry.m_name = StringView(start_of_name, remaining_path.data() - start_of_name);

			REFLECTIVE_INTERNAL_ASSERT(prev_remaining_path_len > remaining_path.length());
			full_path_length += prev_remaining_path_len - remaining_path.length();

			curr_entry.m_full_path = i_full_name.substr(0, full_path_length);

			path_entries.push_back(curr_entry);

		} while (remaining_path.remove_prefix_literal("::"));

		REFLECTIVE_RUNTIME_CHECK(remaining_path.empty(), "badformed full type name");

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
}