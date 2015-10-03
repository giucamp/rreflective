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
	/***/
	class Namespace : public NamespaceMember
	{
	public:
		
		Namespace(SymbolName i_name);

		class MembersList;
		const MembersList & members() const { return m_members; }
		
		/** Registers a member in the namespace, storing a raw pointer to it. Adding a member
			to a namespace when it is already present in the same or another namespace is an
			error. Destroying a NamespaceMember while it is still present in a namespace leads
			to undefined behaviour (probably a crash). */
		void add_member(NamespaceMember & i_member);

		/** Removes a member from the namespace. Trying to remove an object not present */
		void remove_member(NamespaceMember & i_member);

		bool contains(NamespaceMember & i_member) const 
			{ return i_member.parent_namespace() == this; }

		class MembersList
		{
		public:

			MembersList();

			void add(NamespaceMember & i_member);

			void remove(NamespaceMember & i_member);

			class ConstIterator
			{
			public:

				ConstIterator(const NamespaceMember * i_curr)
					: m_curr(i_curr) { }

				ConstIterator & operator ++ ()
				{
					m_curr = m_curr->m_next_member;
					return *this;
				}

				bool operator == (ConstIterator & i_other) const
				{ 
					return m_curr == i_other.m_curr; 
				}

				bool operator != (ConstIterator & i_other) const
				{
					return m_curr != i_other.m_curr;
				}

				const NamespaceMember * operator ->() const
				{
					return m_curr;
				}

				const NamespaceMember & operator * () const
				{
					return *m_curr;
				}
				
			private:
				const NamespaceMember * m_curr;
			};

			ConstIterator begin() const
			{
				return ConstIterator(m_first_member);
			}

			ConstIterator end() const
			{
				return ConstIterator(nullptr);
			}

			ConstIterator cbegin() const
			{
				return ConstIterator(m_first_member); 
			}

			ConstIterator cend() const
			{
				return ConstIterator(nullptr);
			}

		private:
			NamespaceMember * m_first_member, *m_last_member;
		};	
		
		#if REFLECTIVE_ENABLE_TESTING
			static void unit_test();
		#endif

	private:
		MembersList m_members;
	};
}
