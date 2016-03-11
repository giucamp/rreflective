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
		class NamespaceMembersList // this class is the return type of Namespace::members, but is not supposed to be referenced outside the library
		{
		public:

			NamespaceMembersList();

			void add(NamespaceMember & i_member);
			void remove(NamespaceMember & i_member);

			class ConstIterator; // defined later
			
			ConstIterator begin() const;
			ConstIterator end() const;

			ConstIterator cbegin() const;
			ConstIterator cend() const;

		private:
			NamespaceMember * m_first_member, * m_last_member;
		};
	}

	/** \brief A namespace is a named collection of types, inner namespaces, class templates and anything
		deriving from reflective::NamespaceMember. The user can add members to a non-const namespace with
		the method register_member(), and he can remove them with the method unregister_member(). Iteration
		of existing members is available using the method members().<br>
		The name of members in the namespace is not constrained to be unique: for example a namespace may
		contain a class C and a namespace with the same name, that contains inner types and class templates
		of the class C.<br>
		This class uses non-owning raw pointers to keep track of the members, so NamespaceMember's must be
		unregistered before being destroyed. A NamespaceMember can be present only in a Namespace at a time.<br>
		Implementation note: currently this class uses an intrusive linked-list to keep track of the 
		NamespaceMember objects. */
	class Namespace : public NamespaceMember
	{
	public:
		
		/** \brief Constructs a namespace, assigning a name to it */
		Namespace(SymbolName i_name);

		Namespace(const Namespace &) = delete;

		Namespace & operator = (const Namespace &) = delete;

		/** \brief Returns a reference to the container of all the members of the namespace. The actual type of 
			the container is an implementation detail. It support begin(), end(), cbegin() and cend(), so
			it is suitable for range loops, but it should used only with auto and decltype. The type 
			of the elements of the container is const NamespaceMember. <br>
			Note: the members of the namespace may be randomly distributed in the memory, so iterating them
			is slow because cache-unfriendly. If you want to look-up a NamespaceMember given its full name, 
			use the GlobalRegistry instead. */
		const details::NamespaceMembersList & members() const { return m_members; }
		
		/** \brief Registers a member in the namespace, storing a raw pointer to it. Adding a member to a 
			namespace when it is already present in the same or another namespace is an error. Destroying
			a NamespaceMember while it is still present in a namespace leads to undefined behavior (probably 
			a crash). <br>
			The complexity of this method is constant. */
		void register_member(NamespaceMember & i_member);

		/** \brief Removes a member from the registered members list. Trying to remove a member not present is an error.<br>
			The complexity of this method is linear in the count of the registered members */
		void unregister_member(NamespaceMember & i_member);

		/** Returns whether this namespace contains the specified member */
		bool contains(NamespaceMember & i_member) const 
			{ return i_member.parent_namespace() == this; }

		template <typename MEMBER_TYPE>
			MEMBER_TYPE * find_member(const SymbolName & i_name);

		template <typename MEMBER_TYPE>
			const MEMBER_TYPE * find_member(const SymbolName & i_name) const;

		#if REFLECTIVE_ENABLE_TESTING
		
			/** Runs an unit test for this class */
			static void unit_test();
		
		#endif

	private: // data member
		details::NamespaceMembersList m_members;
	};
}
