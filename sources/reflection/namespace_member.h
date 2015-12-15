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

	enum class SymbolTypeId
	{
		none					= 0,
		is_type					= 1 << 0,
		is_class				= 1 << 1,
		is_enum					= 1 << 2,
		is_class_template		= 1 << 3,
		is_namespace			= 1 << 4,
	};

	inline SymbolTypeId operator | (SymbolTypeId i_first, SymbolTypeId i_seconds)
	{
		using underlying_type = std::underlying_type < SymbolTypeId >::type;
		return static_cast<SymbolTypeId>(static_cast<underlying_type>(i_first) | static_cast<underlying_type>(i_seconds));
	}

	inline SymbolTypeId operator & (SymbolTypeId i_first, SymbolTypeId i_seconds)
	{
		using underlying_type = std::underlying_type < SymbolTypeId >::type;
		return static_cast<SymbolTypeId>(static_cast<underlying_type>(i_first)& static_cast<underlying_type>(i_seconds));
	}

	class NamespaceMember : public Symbol
	{
	protected:

		NamespaceMember(SymbolTypeId i_type_id, SymbolName i_name)
			: Symbol(std::move(i_name)), m_type_id(i_type_id), m_parent(nullptr), m_next_member(nullptr)
		{

		}

	public:
		
		std::string full_name() const;

		const Namespace * parent_namespace() const			{ return m_parent; }

		const SymbolTypeId get_type_id() const				{ return m_type_id; }

	private: // data members
		const SymbolTypeId m_type_id;
		const Namespace * m_parent;
		NamespaceMember * m_next_member;
		friend class Namespace;
		friend class details::NamespaceMembersList;
	};
}

