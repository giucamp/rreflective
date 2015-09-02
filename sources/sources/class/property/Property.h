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
