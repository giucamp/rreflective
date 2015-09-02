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
	class Namespace : public Symbol
	{
	public:
		
		Namespace(SymbolName i_name);

		void register_type(const Type & i_type);

		void unregister_type(const Type & i_type);

		void register_namespace(const Namespace & i_namespace);

		void unregister_namespace(const Namespace & i_namespace);
	
	private:
		Type * m_first_child_type;
		Namespace * m_first_child_namespace;
		Namespace * m_parent_namespace;
	};

	class Registry
	{
	public:

		static Registry & instance();

		void register_type(SymbolName i_type_full_name, Type & i_type);

		Type * unregister_type(SymbolName i_type_full_name);

		void register_namespace(SymbolName i_namespace_full_name, Namespace & i_namespace);

		Namespace * unregister_namespace(SymbolName i_namespace_full_name);

	private:
		std::unordered_map<SymbolName, Type *, SymbolNameHasher > m_type_registry;
		std::unordered_map<SymbolName, Namespace *, SymbolNameHasher > m_namespace_registry;
	};
}
