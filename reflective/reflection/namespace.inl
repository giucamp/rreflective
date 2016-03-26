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
		class NamespaceMembersList::ConstIterator
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

		inline NamespaceMembersList::ConstIterator NamespaceMembersList::begin() const
		{
			return ConstIterator(m_first_member);
		}

		inline NamespaceMembersList::ConstIterator NamespaceMembersList::end() const
		{
			return ConstIterator(nullptr);
		}

		inline NamespaceMembersList::ConstIterator NamespaceMembersList::cbegin() const
		{
			return ConstIterator(m_first_member);
		}

		inline NamespaceMembersList::ConstIterator NamespaceMembersList::cend() const
		{
			return ConstIterator(nullptr);
		}

	} // details

	template <typename MEMBER_TYPE>
		MEMBER_TYPE * Namespace::find_member(const SymbolName & i_name)
	{
			static_cast(std::is_base_of<NamespaceMember, MEMBER_TYPE>::value);
		for (auto & member : m_members)
		{
			if (member.m_name == i_name)
			{
				MEMBER_TYPE * const name_match = dynamic_cast<MEMBER_TYPE*>(&member);
				if (name_match != nullptr)
				{
					return name_match;
				}
			}
		}
		return nullptr;
	}

	template <typename MEMBER_TYPE>
		const MEMBER_TYPE * Namespace::find_member(const SymbolName & i_name) const
	{
		for (const auto & member : m_members)
		{
			if (member.m_name == i_name)
			{
				const MEMBER_TYPE * const name_match = dynamic_cast<const MEMBER_TYPE*>(&member);
				if (name_match != nullptr)
				{
					return name_match;
				}
			}
		}
		return nullptr;
	}
}
