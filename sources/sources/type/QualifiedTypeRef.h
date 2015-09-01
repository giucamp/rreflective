
#pragma once
#include "Type.h"
#include "StaticQualification.h"
#include "..\GetType.h"

namespace reflective
{
	class QualifiedTypeRef
	{
	public:

		static const uint32_t s_max_indirection_levels = (1 << 4) - 1;
		
		QualifiedTypeRef(const Type & i_underlying_type, uint32_t i_indirection_levels = 0, uint32_t i_constness_word = 0, uint32_t i_volatilenessness_word = 0);

		QualifiedTypeRef(const QualifiedTypeRef & i_source) = default;

		QualifiedTypeRef & operator = (const QualifiedTypeRef & i_source) = delete;

		const Type & front_type() const									{ return m_indirection_levels > 0 ? get_type<void*>() : m_underlying_type; }
			
		const Type & underlying_type() const							{ return m_underlying_type; }

		uint32_t indirection_levels() const								{ return m_indirection_levels; }
		
		uint32_t constness_word() const									{ return m_constness_word; }

		uint32_t volatilenessness_word() const							{ return m_volatilenessness_word; }

		bool is_const(uint32_t i_indirection_level) const				{ return (m_constness_word & (1 << i_indirection_level)) != 0; }

		bool is_volatile(uint32_t i_indirection_level) const			{ return (m_constness_word & (1 << i_indirection_level)) != 0; }

		bool operator == (const QualifiedTypeRef & i_source) const;

		bool operator != (const QualifiedTypeRef & i_source) const		{ return !operator == (i_source); }

		template <typename OUT_STREAM>
			void to_string(OUT_STREAM & i_dest);

	private:
		const Type & m_underlying_type;
		const uint32_t m_indirection_levels : 4;
		const uint32_t m_constness_word : 14;
		const uint32_t m_volatilenessness_word : 14;
	};

	template <typename TYPE>
		QualifiedTypeRef get_qualified_type()
	{
		static_assert(StaticQualification<TYPE>::s_indirection_levels < s_max_indirection_levels);

		return QualifiedTypeRef(get_type<TYPE>(), 
			StaticQualification<TYPE>::s_indirection_levels,
			StaticQualification<TYPE>::s_constness_word,
			StaticQualification<TYPE>::s_volatileness_word );			
	}
}
