
#pragma once
#include "Property.h"

namespace reflective
{
	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		class DataMemberProperty : public Property
	{
	public:

		using MemberPtr = PROPERTY_TYPE (OWNER_CLASS::*);

		Property(SymbolName i_name, MemberPtr, ClassMember::Flags i_flags)
			: ClassMember(std::move(i_name), i_flags), m_type(i_type) { }


	protected:

		virtual void * get_value_inplace_impl(void * i_owner_object, TextOutStream * i_error_stream) const = 0;

		virtual bool get_value_impl(const void * i_owner_object, void * i_dest, TextOutStream * i_error_stream) const = 0;

		virtual bool set_value_impl(const void * i_owner_object, const void * i_source, TextOutStream * i_error_stream) const = 0;
		
	private:
		const QualifiedTypeRef m_type;
	};
}
