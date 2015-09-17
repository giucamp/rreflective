
#pragma once
#include "Property.h"

namespace reflective
{
	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		class DataMemberProperty : public Property
	{
	public:

		using MemberPtr = PROPERTY_TYPE (OWNER_CLASS::*);

		DataMemberProperty(SymbolName i_name, MemberPtr, ClassMember::Flags i_flags)
			: ClassMember(std::move(i_name), i_flags), m_type(get_qualified_type<PROPERTY_TYPE>()) { }


	private:

		virtual void * get_value_inplace_impl(void * i_owner_object, OutStringBuffer * i_error_stream) const
		{
			return 0;
		}

		virtual bool get_value_impl(const void * i_owner_object, void * i_dest, OutStringBuffer * i_error_stream) const
		{
			return false;
		}

		virtual bool set_value_impl(const void * i_owner_object, const void * i_source, OutStringBuffer * i_error_stream) const
		{
			return false;
		}
		
	private:
		const QualifiedTypeRef m_type;
	};

	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		inline std::unique_ptr<DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>> new_property(SymbolName i_name, PROPERTY_TYPE(OWNER_CLASS::*i_member),
			ClassMember::Flags i_flags = ClassMember::Flags::none)
	{
		return std::make_unique<DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>>(i_name, i_member, i_flags);
	}
}
