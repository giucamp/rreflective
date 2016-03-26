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
	template <typename TYPE>
	using ReflectingType = typename details::SymbolTraits<
		details::CleanType<TYPE>, details::GetSymbolTypeId< details::CleanType<TYPE> >::s_type_id >::ReflectedType;

	template <typename TYPE>
	const ReflectingType<TYPE> & get_naked_type();

	template <typename TYPE>
	class TypeSetupContext
	{
	public:

		TypeSetupContext(ReflectingType<TYPE> * i_type)
			: m_type(i_type)
		{

		}

		ReflectingType<TYPE> * type() const { return m_type; }

	private:
		ReflectingType<TYPE> * m_type;
	};

	namespace details
	{
		template <bool HAS_SETUP_CLASS_FUNC, typename TYPE>
		struct InternalSetupType;
		template <typename TYPE>
		struct InternalSetupType< false, TYPE >
		{
			static void setup(TypeSetupContext<TYPE> & i_context)
			{
				reflective::setup_type(i_context);
			}
		};
		template <typename TYPE>
		struct InternalSetupType< true, TYPE >
		{
			static void setup(TypeSetupContext<TYPE> & i_context)
			{
				TYPE::setup_class(*static_cast<Class*>(i_context.type()));
			}
		};

		template <bool HAS_UNIT_TEST_FUNC, typename TYPE>
		struct InternalAddUnitTest;
		template <typename TYPE>
		struct InternalAddUnitTest< false, TYPE >
		{
			static void add_unit_test()
			{
			}
		};
		template <typename TYPE>
		struct InternalAddUnitTest< true, TYPE >
		{
			static void add_unit_test()
			{
				UnitTestingManager::instance().add_correctness_test(get_type_full_name<TYPE>(), [](CorrectnessTestContext & i_context) {
					unit_test(static_cast<TYPE**>(nullptr), i_context);
				});
			}
		};
	}



	template <typename TYPE>
		void setup_type(TypeSetupContext<TYPE> & i_context)
	{
		i_context.type()->set_implicit_reflection(true);
	}

	namespace details
	{
		// SymbolTraits::create for primitive types
		template <typename TYPE>
			inline const Type * SymbolTraits< TYPE, SymbolTypeId::is_type>::create()
		{
			Type * new_type = GlobalRegistry::instance().add_primitive_type(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, typeid(TYPE));
			new_type->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(new_type);
			InternalSetupType<HasSetupClassStaticFunc< TYPE >::value, TYPE>::setup(context);
			return new_type;
		}

		// SymbolTraits::create for class types
		template <typename TYPE>
			inline const Class * SymbolTraits< TYPE, SymbolTypeId::is_class>::create()
		{
			Class * class_obj = GlobalRegistry::instance().add_class(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, typeid(TYPE));
			class_obj->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(class_obj);
			InternalSetupType<HasSetupClassStaticFunc<TYPE>::value, TYPE>::setup(context);
			InternalAddUnitTest<HasUnitTestFunc<TYPE>::value, TYPE>::add_unit_test();
			return class_obj;
		}

		// SymbolTraits::create for enum types
		template <typename TYPE>
			inline const typename reflective::Enum<std::underlying_type<TYPE>> * SymbolTraits< TYPE, SymbolTypeId::is_enum >::create()
		{
			auto enum_obj = GlobalRegistry::instance().add_enum<std::underlying_type<TYPE>>(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, typeid(TYPE));
			enum_obj->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(enum_obj);
			InternalSetupType<HasSetupClassStaticFunc< TYPE >::value, TYPE>::setup(context);
			return enum_obj;
		}
	}
}
