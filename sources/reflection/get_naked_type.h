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
	namespace details
	{
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
				std::is_rvalue_reference<TYPE>::value, "Type not supported" );

			static const SymbolTypeId s_type_id = std::is_class<TYPE>::value ? SymbolTypeId::is_class :
				(std::is_enum<TYPE>::value ? SymbolTypeId::is_enum : SymbolTypeId::is_type );
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
				Type * new_type = new Type("void", 0, 1 );
				return new_type;
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


	template <typename TYPE>
	using ReflectingType = typename details::SymbolTraits<
		details::CleanType<TYPE>, details::GetSymbolTypeId< details::CleanType<TYPE> >::s_type_id >::ReflectedType;

	template <typename TYPE>
		const ReflectingType<TYPE> & get_naked_type();


	template <typename TYPE>
		class TypeSetupContext
	{
	public:

		TypeSetupContext(typename ReflectingType<TYPE> * i_type)
			: m_type(i_type)
		{

		}

		typename ReflectingType<TYPE> * type() const { return m_type; }

	private:
		typename ReflectingType<TYPE> * m_type;
	};

	template <typename TYPE>
		void setup_type(TypeSetupContext<TYPE> & i_context)
	{
		i_context.type()->set_implicit_reflection(true);
	}

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

		template <typename REFLECTING_TYPE>
		REFLECTING_TYPE * __create_builtin_type(StringView i_complete_name, size_t i_size, size_t i_alignment)
		{
			static_assert(std::is_base_of<REFLECTING_TYPE, Type>::value, "REFLECTING_TYPE must derive from reflective::Type");

			i_complete_name.remove_prefix_literal("class ");
			i_complete_name.remove_prefix_literal("::");

			auto identifier = i_complete_name.remove_prefix_identifier();

				// todo
		}

		// SymbolTraits::create for primitive types
		template <typename TYPE>
			inline const Type * SymbolTraits< TYPE, SymbolTypeId::is_type>::create()
		{
			Type * new_type = new Type(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value);
			new_type->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(new_type);
			InternalSetupType<HasSetupClassStaticFunc< TYPE >::value, TYPE>::setup(context);
			return new_type;
		}

		// SymbolTraits::create for class types
		template <typename TYPE>
			inline const Class * SymbolTraits< TYPE, SymbolTypeId::is_class>::create()
		{
			Class * class_obj = new Class(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value);
			class_obj->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(class_obj);
			InternalSetupType<HasSetupClassStaticFunc< TYPE >::value, TYPE>::setup(context);
			return class_obj;
		}

		// SymbolTraits::create for enum types
		template <typename TYPE>
			inline const typename reflective::Enum<std::underlying_type<TYPE>> * SymbolTraits< TYPE, SymbolTypeId::is_enum >::create()
		{
			Enum< std::underlying_type<TYPE> > * enum_obj = new Enum< std::underlying_type<TYPE> >(get_type_full_name<TYPE>());
			enum_obj->set_special_functions(SpecialFunctions::from_type<TYPE>());
			TypeSetupContext<TYPE> context(enum_obj);
			InternalSetupType<HasSetupClassStaticFunc< TYPE >::value, TYPE>::setup(context);
			return enum_obj;
		}
	}
}
