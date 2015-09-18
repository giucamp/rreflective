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
	const Namespace & root_namespace();

	Namespace & edit_root_namespace();

	namespace detail
	{
		template <typename TYPE> struct _GetSymbolTypeId
		{
			static_assert(std::is_class<TYPE>::value || std::is_enum<TYPE>::value ||
				std::is_fundamental<TYPE>::value, "Type not supported" );

			static const SymbolTypeId s_type_id = std::is_class<TYPE>::value ? reflective::Class :
				(std::is_enum<TYPE>::value ? SymbolTypeId::enum_symbol : SymbolTypeId::primitive_type_symbol );
		};

		template <typename TYPE, SymbolTypeId TYPE_ID> struct _SymbolTraits;

		template <typename TYPE> 
			struct _SymbolTraits< TYPE, SymbolTypeId::primitive_type_symbol>
		{
			using ReflectedType = reflective::Type;
		};

		template <typename TYPE> 
			struct _SymbolTraits< TYPE, SymbolTypeId::class_symbol>
		{
			using ReflectedType = reflective::Class;
		};

		template <typename TYPE> 
			struct _SymbolTraits< TYPE, SymbolTypeId::enum_symbol >
		{
			using ReflectedType = reflective::Enum<std::underlying_type<TYPE>>;
		};		
	}

	template <typename TYPE>
		using reflecting_type = typename detail::_SymbolTraits<TYPE, detail::_GetSymbolTypeId<TYPE>::s_type_id>::ReflectedType;

	template <typename TYPE>
		const Type & get_type();

	template <typename TYPE>
		const Class * get_class();
}
