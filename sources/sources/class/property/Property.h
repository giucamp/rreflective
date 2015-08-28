
#pragma once
#include "..\ClassMember.h"
#include "..\..\type\QualifiedTypeRef.h"
#include "..\..\core\Streams.h"

namespace reflective
{
	class Property : public ClassMember
	{
	public:

		Property(SymbolName i_name, QualifiedTypeRef i_type, ClassMember::Flags i_flags )
			: ClassMember(std::move(i_name), i_flags), m_type(i_type) { }

		const QualifiedTypeRef & type() const	{ return m_type; }

		bool get_value(const void * i_owner_object, void * i_dest, TextOutStream * i_error_stream) const;

		bool set_value(const void * i_owner_object, const void * i_source, TextOutStream * i_error_stream) const;

		const void * get_value_inplace(const void * i_owner_object, TextOutStream * i_error_stream) const;

		void * get_editable_value_inplace(void * i_owner_object, TextOutStream * i_error_stream) const;

	protected:

		virtual void * get_value_inplace_impl(void * i_owner_object, TextOutStream * i_error_stream) const = 0;

		virtual bool get_value_impl(const void * i_owner_object, void * i_dest, TextOutStream * i_error_stream) const = 0;

		virtual bool set_value_impl(const void * i_owner_object, const void * i_source, TextOutStream * i_error_stream) const = 0;
		
	private:
		const QualifiedTypeRef m_type;
	};
}
