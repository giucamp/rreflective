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
	class ClassMember : public Symbol
	{
	public:

		enum class Flags
		{
			none = 0,
			private_member = 1 << 0,
			protected_member = 1 << 1,
			can_get_value_by_copy = 1 << 2,
			can_get_value_by_move = 1 << 3,
			can_inspect_value_inplace = 1 << 4,
			can_set_value_by_copy = 1 << 5,
			can_set_value_by_move = 1 << 6,
			can_edit_value_inplace = 1 << 7,
		};
		
		ClassMember(SymbolName i_name, Flags i_flags);

		#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
			ClassMember(const ClassMember&) = delete;
		#endif

		bool has_flags(Flags i_flags) const;

	private:
		const Flags m_flags;
	};

	inline ClassMember::Flags operator | (ClassMember::Flags i_first, ClassMember::Flags i_seconds)
	{
		using underlying_type = std::underlying_type < ClassMember::Flags >::type;
		return static_cast<ClassMember::Flags>(static_cast<underlying_type>(i_first) | static_cast<underlying_type>(i_seconds));
	}

	inline ClassMember::Flags operator & (ClassMember::Flags i_first, ClassMember::Flags i_seconds)
	{
		using underlying_type = std::underlying_type < ClassMember::Flags >::type;
		return static_cast<ClassMember::Flags>(static_cast<underlying_type>(i_first) & static_cast<underlying_type>(i_seconds));
	}

	inline bool ClassMember::has_flags(ClassMember::Flags i_flags) const
	{
		return (m_flags & i_flags) == i_flags;
	}

	inline ClassMember::ClassMember(SymbolName i_name, Flags i_flags)
		: Symbol(i_name), m_flags(i_flags) 
	{
		#if REFLECTIVE_ASSERT_ENABLED
			if (has_flags(Flags::private_member))
			{
				REFLECTIVE_ASSERT(!has_flags(Flags::protected_member), "ClassMember::Flags: private_member and protected_member are incompatible");
			}
		#endif
	}
}


