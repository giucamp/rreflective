/***********************************************************************************

Copyright 2011-2012 Giuseppe Campana - giu.campana@gmail.com
All rights reserved

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY GIUSEPPE CAMPANA ''AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL GIUSEPPE CAMPANA OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************************/

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	/** The global registry is a singleton class in which the reflection system automatically registers:
		- all the types instantiated to the presence of a call to get_type() or get_naked_type()
		- all the class templates instantiated to the presence of a call to get_type() or get_naked_type()
		- all the namespaces containing (directly or indirectly) the registered types and class templates.
		The global registry holds non-owning raw pointers to the registered object. If the pointed object
		is destroyed while being still registered, the pointer become dangling, and accessing the object
		leads to an undefined behavior. */
	class GlobalRegistry
	{
	public:

		static GlobalRegistry & instance();

		const Namespace & global_namespace() const { return m_global_namespace; }

		Class * add_class(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info);

		Type * add_primitive_type(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info);

		template <typename UNDERLYING_TYPE>
			Enum<UNDERLYING_TYPE> * add_enum(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info);

		/** Finds a type from a full name ("reflective::Property", or "::reflective::Enum<int>")
			@return pointer to the type if it has been found, false otherwise.
			Implementation note: currently the complexity of this method is the same of std::unordered_map::find. */
		const Type * find_type_by_full_name(StringView i_full_name) const;

		const Type * find_type_by_rtti(const std::type_info & i_type_info) const;

		const Namespace * find_namespace(StringView i_full_path) const;
						
		GlobalRegistry(const GlobalRegistry &) = delete;

		GlobalRegistry & operator = (const GlobalRegistry &) = delete;

	private:

		Namespace * parse_type_full_name(StringView i_full_name, StringView * o_type_name, StringView * o_template_argument_list);

		GlobalRegistry();
		
		~GlobalRegistry() = default;

		void register_member(NamespaceMember & i_member);

		void unregister_member(NamespaceMember & i_member);

		static StringView parse_template_arguments(StringView & io_input);

	private: // data members
		//std::unordered_multimap<SymbolName, NamespaceMember *, SymbolNameHasher > m_registry;

		unordered_map<SymbolName, unique_ptr<Namespace>, SymbolNameHasher > m_namespaces;
		unordered_map<std::type_index, unique_ptr<Type> > m_types_by_rtti;
		unordered_map<SymbolName, const Type*, SymbolNameHasher > m_types_by_full_name;
		Namespace m_global_namespace;
	};

	template <typename UNDERLYING_TYPE>
		Enum<UNDERLYING_TYPE> * GlobalRegistry::add_enum(StringView i_full_name, size_t i_size, size_t i_alignment, const std::type_info & i_type_info)
	{
		i_full_name.remove_prefix_literal("::");
		i_full_name.remove_prefix_literal("enum ");

		StringView type_name, template_argument_list;
		auto parent_namespace = parse_type_full_name(i_full_name, &type_name, &template_argument_list);

		// create the type
		auto enum_type = reflective::make_unique<Enum<UNDERLYING_TYPE>>(type_name, i_size, i_alignment);

		// add the type to m_types_by_rtti
		auto const enum_raw_ptr = enum_type.get();
		m_types_by_rtti.emplace(std::make_pair(std::type_index(i_type_info), std::move(enum_type)));

		// add the type to m_types_by_full_name		
		m_types_by_full_name.emplace(std::make_pair(i_full_name, enum_raw_ptr));

		// add the enum to the namespace
		parent_namespace->register_member(*enum_raw_ptr);

		return enum_raw_ptr;
	}

	template <typename UNDERLYING_STREAM>
		void parse_type_name(InTxtStreamAdapt<UNDERLYING_STREAM, char> & i_source, std::string & o_result)
	{
		do {
			auto name = i_source.accept_identifier();
			o_result += name;
			if (i_source.accept_char('<'))
			{
				o_result += '<';
				uint32_t depth = 1;
				i_source.accept_while([&depth](char i_char)->bool {
					if (i_char == '<')
					{
						depth++;
					} else if (i_char == '>')
					{
						depth--;
					}
					return depth > 0;
				});
			}		 
		} while (i_source.accept_literal("::"));
	}
}
