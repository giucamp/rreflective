
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

#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif


namespace reflective
{


	namespace details
	{
		/** HasSetupClassStaticFunc< TYPE >::value is non-zero if the type has a static function with the signature:
			static void setup_class( reflective::Class & class_obj );
			implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error */
		struct HasSetupClassStaticFunc_Base
		{
			template <class TYPE>
			static std::true_type func(int, decltype(TYPE::setup_class(*(Class*)nullptr)) par = 0);

			template <class TYPE>
			static std::false_type func(...);
		};
		template <class TYPE>
			struct HasSetupClassStaticFunc : HasSetupClassStaticFunc_Base
		{
			typedef decltype(func<TYPE>(0)) B;
			static const bool value = B::value;
		};



		/** HasUnitTestFunc< TYPE >::value */
		struct HasUnitTestFunc_Base
		{
			template <class TYPE>
			static std::true_type func(int, decltype(unit_test((TYPE**)nullptr, *(CorrectnessTestContext*)0)) par = 0);

			template <class TYPE>
			static std::false_type func(...);
		};
		template <class TYPE>
			struct HasUnitTestFunc : HasUnitTestFunc_Base
		{
			typedef decltype(func<TYPE>(0)) B;
			static const bool value = B::value;
		};


		enum class SymbolTypeId
		{
			is_type,
			is_class,
			is_enum,
		};

		// defines s_type_id for TYPE
		template <typename TYPE> struct GetSymbolTypeId
		{
			static_assert(std::is_class<TYPE>::value || std::is_enum<TYPE>::value ||
				std::is_fundamental<TYPE>::value || std::is_pointer<TYPE>::value || std::is_reference<TYPE>::value ||
				std::is_rvalue_reference<TYPE>::value, "Type not supported");

			static const SymbolTypeId s_type_id = std::is_class<TYPE>::value ? SymbolTypeId::is_class :
				(std::is_enum<TYPE>::value ? SymbolTypeId::is_enum : SymbolTypeId::is_type);
		};

		// defines ReflectedType (that is Class, Type, Enum, etc..) and static const ReflectedType * create()
		template <typename TYPE, SymbolTypeId TYPE_ID> struct SymbolTraits;

		// SymbolTraits for void
		template <>
		struct SymbolTraits< void, SymbolTypeId::is_type>
		{
			using ReflectedType = reflective::Type;

			static const Type * create()
			{
				return GlobalRegistry::instance().add_primitive_type("void", 0, 1, typeid(void));
			}
		};

		// SymbolTraits for primitive types
		template <typename TYPE>
		struct SymbolTraits< TYPE, SymbolTypeId::is_type>
		{
			using ReflectedType = reflective::Type;

			static const Type * create();
		};

		// SymbolTraits for classes
		template <typename TYPE>
		struct SymbolTraits< TYPE, SymbolTypeId::is_class>
		{
			using ReflectedType = reflective::Class;

			static const Class * create();
		};

		template <typename TYPE>
		struct SymbolTraits< TYPE, SymbolTypeId::is_enum >
		{
			using ReflectedType = reflective::Enum<std::underlying_type<TYPE>>;

			static const reflective::Enum<std::underlying_type<TYPE>> * create();
		};

		template <typename TYPE>
		using CleanType = std::conditional_t< std::is_pointer<TYPE>::value || std::is_reference<TYPE>::value,
			void*, std::decay_t<TYPE > >;
	}
}