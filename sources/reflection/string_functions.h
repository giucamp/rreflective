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
	class StringFunctions final
	{
	public:

		using ToString = void(*)(const void * i_object, OutStringBuffer & i_dest);
		using AssignFromString = bool (*)(void * i_object, InStringBuffer & i_source, OutStringBuffer & i_error_dest);

		StringFunctions()
			: m_to_string_function(nullptr), m_assign_from_string_function(nullptr)
		{
		}

		StringFunctions(ToString i_to_string_function, AssignFromString i_assign_from_string_function)
			: m_to_string_function(i_to_string_function), m_assign_from_string_function(i_assign_from_string_function)
		{
		}

		template <typename TYPE>
			static StringFunctions from_type()
				{ return StringFunctions( &to_string_method_adater<TYPE>, &assign_from_string_method_adater<TYPE> ); }
			
	private:
		
		template <typename TYPE> void to_string_method_adater(const void * i_object, OutStringBuffer & i_dest)
		{
			const TYPE & obj = static_cast<const TYPE*>(i_object);
			dbg_object_validate(obj);
			obj.to_string(i_dest);
		}

		template <typename TYPE> bool assign_from_string_method_adater(void * i_object, InStringBuffer & i_source, OutStringBuffer & i_error_dest)
		{
			TYPE & obj = static_cast<TYPE*>(i_object);
			dbg_object_validate(obj);
			return obj.assign_from_string(i_source, i_error_dest);
		}

	private:
		ToString m_to_string_function;
		AssignFromString m_assign_from_string_function;
	};
}
