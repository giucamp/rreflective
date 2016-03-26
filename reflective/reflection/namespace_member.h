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
	namespace details
	{
		class NamespaceMembersList;
	}

	/** A NamespaceMember is a symbol tht can be added to a reflective::Namespace. The namespace membership is
		implemented with an intrusive linked list. A namespace member can be added to only one reflective::Namespace.
		NamespaceMember allows to access the parent namespace with a pointer (see NamespaceMember::parent_namespace).
		NamespaceMember is the base class for Type, Namespace, and ClassTemplate. 
		This class is polymorphic. */
	class NamespaceMember : public Symbol
	{
	protected:

		/** Constructs a namespace member, providing the name, which is accessible with the getter method Symbol::name. */
		NamespaceMember(SymbolName i_name)
			: Symbol(std::move(i_name)), m_parent(nullptr), m_next_member(nullptr)
		{
		}

		/** Virtual destructor, to allow polymorphism. */
		virtual ~NamespaceMember() = default;

	public:
		
		std::string full_name() const;

		const Namespace * parent_namespace() const			{ return m_parent; }

	private: // data members
		const Namespace * m_parent;
		NamespaceMember * m_next_member;
		friend class Namespace;
		friend class details::NamespaceMembersList;
	};
}

