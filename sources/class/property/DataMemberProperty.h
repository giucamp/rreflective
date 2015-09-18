
#pragma once
#include "Property.h"

namespace reflective
{
	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		class DataMemberProperty : public Property
	{
	public:

		using MemberPtr = PROPERTY_TYPE (OWNER_CLASS::*);

		DataMemberProperty(SymbolName i_name, MemberPtr i_member, ClassMember::Flags i_flags)
			: Property(std::move(i_name), get_qualified_type<PROPERTY_TYPE>(), i_flags), m_member(i_member) { }

	private:

		void * get_value_inplace_impl(void * i_owner_object) const override
		{
			OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object);
			dbg_object_validate(owner_object);
			
			PROPERTY_TYPE & property_value = owner_object.*m_member;
			dbg_object_validate(property_value);
			
			return &property_value;
		}

		bool get_value_impl(const void * i_owner_object, void * i_dest) const override
		{
			const OWNER_CLASS & owner_object = *static_cast<const OWNER_CLASS*>(i_owner_object);
			dbg_object_validate(owner_object);

			const PROPERTY_TYPE & property_value = owner_object.*m_member;
			dbg_object_validate(property_value);

			new(i_dest) PROPERTY_TYPE( property_value );

			return true;
		}

		bool set_value_impl(void * i_owner_object, const void * i_source) const override
		{
			OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object);
			dbg_object_validate(owner_object);

			PROPERTY_TYPE & dest_property = owner_object.*m_member;
			dbg_object_validate(dest_property);

			const PROPERTY_TYPE & source_object = *static_cast<const PROPERTY_TYPE*>(i_source);
			dest_property = source_object;

			return true;
		}
		
	private:
		const MemberPtr m_member;
	};

	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		inline std::shared_ptr<DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>> new_property(SymbolName i_name, PROPERTY_TYPE(OWNER_CLASS::*i_member),
			ClassMember::Flags i_flags = ClassMember::Flags::none)
	{
		return std::make_unique<DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>>(i_name, i_member, i_flags);
	}
}
