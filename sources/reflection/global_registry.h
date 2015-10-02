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
	/** The global registry is a static class in wich the reflection system automatically registers:
		- all the types instantiated to the presence of a call to get_type() or get_naked_type()
		- all the class templates instantiated to the presence of a call to get_type() or get_naked_type()
		- all the namespaces containing (directly or indirectly) the registered types and class templates.
		All the methods of the global registry are static to minimize the size of the generated code
		in the call sites (that usually are function templates).
		The global registry holds non-owning raw pointers to the registered object. If the pointed object
		is destroyed while beeing still registered, the pointer become dangling, and accessing the object
		leads to an undefined behaviour. */
	class GlobalRegistry
	{
	public:

		/** Finds a type from a full name ("reflective::Property", or "::reflective::Enum<int>")
			@return pointer to the type if it has been found, false otherwise.
			Implementation note: currently the complexity of this method is the same of std::unordered_map::find. */
		static const Type * find_type(StringView i_full_type_name);

		/** Registers a type, storing a raw pointer to it in the registry. Types instantiated due
			to the presence of a call to get_type() or get_naked_type() are automatically registered
			to the global registry. The user may use this method only for types created explicitly. 
			Registering a type already registered is an error. */
		static void register_type(const Type & i_type);

		/** Unregisters a type previously registered. Unregistering a type not registered is an error. */
		static void unregister_type(const Type & i_type);


		static void register_class_template(const ClassTemplate & i_class_template);

		static void unregister_class_template(const ClassTemplate & i_class_template);


		void register_namespace(const Namespace & i_namespace);

		const Namespace * unregister_namespace(SymbolName i_namespace_full_name);

		GlobalRegistry(const GlobalRegistry &) = delete;

		GlobalRegistry & operator = (const GlobalRegistry &) = delete;

	private:

		GlobalRegistry();
		
		~GlobalRegistry() = default;
		
		static GlobalRegistry & internal_instance();
		
	private: // data members
		std::unordered_map<SymbolName::HashType, const Type *> m_type_registry;
		std::unordered_map<SymbolName::HashType, const ClassTemplate *> m_class_template_registry;
		std::unordered_map<SymbolName::HashType, const Namespace *> m_namespace_registry;
	};
}
