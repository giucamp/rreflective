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
	struct BaseType
	{
	public:
		
		BaseType(const Type * i_base_type, const UpDownCaster<> & i_updown_caster)
			: m_base_type(i_base_type), m_updown_caster(i_updown_caster) { }

	private:
		const Type * m_base_type;
		UpDownCaster<> m_updown_caster;		
	};
	
	class Type : public NamespaceMember
	{
	public:

		static const size_t s_max_size = (1 << 24) - 1;
		static const size_t s_max_alignment = (1 << 8) - 11;

		Type(SymbolTypeId i_type_id, SymbolName i_name, size_t i_size, size_t i_alignment, const SpecialFunctions & i_special_functions);

		virtual ~Type() {}

		size_t size() const							{ return m_size; }

		size_t alignment() const					{ return m_alignment; }
								
		void full_name_to_string(OutStringBuffer & i_dest) const;
		
		static const Type * accept_full_name(InStringBuffer & i_source, OutStringBuffer & i_error_dest);

	private:
		SpecialFunctions m_special_functions;
		const size_t m_size;
		const size_t m_alignment;

		// misc
		StringFunctions m_string_functions;
	};
}

