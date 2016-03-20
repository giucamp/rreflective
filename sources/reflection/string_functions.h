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
	template <typename TYPE, typename = VoidT<>> struct HasStringizer : std::false_type { };
	template <typename TYPE> struct HasStringizer<TYPE, VoidT<decltype(   TYPE::method6    )>> : std::true_type { };

	// http://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function

	/** Checks at compile time if a type has member function with this name and signature:
			void to_string(OutStringBuffer & i_dest) const;
		Usage: const bool has = has_to_string<MyClass>::value; */
	template <typename TYPE> class has_to_string
	{
	private:
		typedef char Yes;
		typedef Yes No[2];

		template <typename U, U> struct really_has;

		template<typename C> static Yes& Test(really_has <void (C::*)(OutStringBuffer & i_dest) const, &C::to_string>*);
		template<typename> static No& Test(...);

	public:
		static bool const value = sizeof(Test<TYPE>(0)) == sizeof(Yes);
	};

	/** Checks at compile time if a type has member function with this name and signature:
			bool assign_from_string(StringView & i_source, OutStringBuffer & i_error_dest);
		Usage: const bool has = has_assign_from_string<MyClass>::value; */
	template <typename TYPE> class has_assign_from_string
	{
	private:
		typedef char Yes;
		typedef Yes No[2];

		template <typename U, U> struct really_has;

		template<typename C> static Yes& Test(really_has <bool (C::*)(StringView & i_source, OutStringBuffer & i_error_dest), &C::assign_from_string>*);
		template<typename> static No& Test(...);

	public:
		static bool const value = sizeof(Test<TYPE>(0)) == sizeof(Yes);
	};

	class StringFunctions final
	{
	public:

		using ToString = void(*)(const void * i_object, OutStringBuffer & i_dest);
		using ToStdStream = void(*)(const void * i_object, std::ostream & i_dest);
		using AssignFromString = bool (*)(void * i_object, StringView & i_source, OutStringBuffer & i_error_dest);

		StringFunctions()
			: m_to_string(nullptr), m_to_std_stream(nullptr), m_assign_from_string_function(nullptr)
		{
		}

		StringFunctions(ToString i_to_string, ToStdStream i_to_std_stream, AssignFromString i_assign_from_string_function)
			: m_to_string(i_to_string), m_to_std_stream(i_to_std_stream), m_assign_from_string_function(i_assign_from_string_function)
		{
		}

		template <typename TYPE>
			static StringFunctions from_type()
				{ return StringFunctions( 
					&to_string_method_adater<TYPE>, 
					&to_std_stream_method_adater<TYPE>,
					&assign_from_string_method_adater<TYPE> ); }
			
	private:
		
		template <typename TYPE> void to_string_method_adater(const void * i_object, OutStringBuffer & i_dest)
		{
			const TYPE & obj = static_cast<const TYPE*>(i_object);
			dbg_object_validate(obj);
			i_dest << obj;
		}

		template <typename TYPE> void to_std_stream_method_adater(const void * i_object, std::ostream & i_dest)
		{
			const TYPE & obj = static_cast<const TYPE*>(i_object);
			dbg_object_validate(obj);
			i_dest << obj;
		}

		template <typename TYPE> bool assign_from_string_method_adater(void * i_object, StringView & i_source, OutStringBuffer & i_error_dest)
		{
			TYPE & obj = static_cast<TYPE*>(i_object);
			dbg_object_validate(obj);
			return obj.assign_from_string(i_source, i_error_dest);
		}

	private:
		ToString m_to_string;
		ToStdStream m_to_std_stream;
		AssignFromString m_assign_from_string_function;
	};
}

