
#pragma once
#include "..\core\Symbol.h"

namespace reflective
{
	class ClassMember : public Symbol
	{
	public:

		enum class Flags
		{
			private_member = 1 << 0,
			protected_member = 1 << 1,
			writeonly_member = 1 << 2,
			readonly_member = 1 << 3,
		};
		
		ClassMember(SymbolName i_name, Flags i_flags);

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
			if (has_flags(Flags::readonly_member))
			{
				REFLECTIVE_ASSERT(!has_flags(Flags::writeonly_member), "ClassMember::Flags: readonly_member and writeonly_member are incompatible");
			}

			if (has_flags(Flags::private_member))
			{
				REFLECTIVE_ASSERT(!has_flags(Flags::protected_member), "ClassMember::Flags: private_member and protected_member are incompatible");
			}
		#endif
	}
}


