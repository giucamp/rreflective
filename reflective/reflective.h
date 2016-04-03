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

// dependencies
#include <stddef.h>
#include <stdint.h>
#include <memory>
#include <string>
#include <string.h>
#include <type_traits>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <typeindex>

#define INCLUDING_REFLECTIVE


// forward declarations
namespace reflective
{
	class Symbol; // basic reflective object, with a name (of type SymbolName) - defined in core\Symbol.h
		class NamespaceMember;
			class Type; // provides informations and services related to a type - defined in reflection\Type.h
				class Class; // a class or struct, with properties, actions and events
				template <typename UNDERLYING_TYPE> class Enum; // provides informations and services related to an enumeration type
			class Namespace;
			class ClassTemplate;
		class ClassMember; // provides informations about a member of a class (such visibility)
			class Property; // generalization of a typed data member (maybe a getter-setter pair)
				template <typename OWNER_CLASS, typename PROPERTY_TYPE> class DataMemberProperty; // a data member
			class Action; // a (non-const) function callable on a class instance
			class Event;



	class ObjPtr;
	class QualifiedTypePtr;
	
	template < typename UINT>
		class StringHasher;
	
	class PtrString;
	
}

#include "reflective_settings.h"

// core headers
#include "density\density.h"

namespace reflective
{
	template <typename ELEMENT>
		using List = DenseList<ELEMENT>;

	template <typename TYPE>
		inline void dbg_object_validate( const TYPE & /*i_object*/ )
	{

	}
				
	template <typename TYPE>
		using unique_ptr = std::unique_ptr< TYPE >;

	using std::make_unique;
	
	template <typename TYPE>
		using vector = std::vector< TYPE > ;

	template <typename KEY, typename TYPE, typename HASHER = std::hash<KEY>, typename KEY_EQUAL_PRED = std::equal_to<KEY> >
		using unordered_map = std::unordered_map< KEY, TYPE, HASHER, KEY_EQUAL_PRED >;

			

	class Ext
	{
	public:
		
		template <typename CONTAINER, typename ELEMENT>
			static auto find(CONTAINER & i_container, ELEMENT && i_value) -> decltype(std::find(i_container.begin(), i_container.end(), i_value))
		{
			return std::find(i_container.begin(), i_container.end(), i_value);
		}

		template <typename CONTAINER, typename PREDICATE>
			static auto find_if(CONTAINER & i_container, PREDICATE && i_predicate) -> decltype(std::find_if(i_container.begin(), i_container.end(), i_predicate))
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate);
		}

		template <typename CONTAINER, typename ELEMENT>
			static bool contains(CONTAINER & i_container, ELEMENT && i_value)
		{
			return std::find(i_container.begin(), i_container.end(), i_value) != i_container.end();
		}

		template <typename CONTAINER, typename PREDICATE>
			static bool contains_if(CONTAINER & i_container, PREDICATE && i_predicate)
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate) != i_container.end();
		}
	};
}

// type headers
#include "reflection\symbol.h"
#include "reflection\base_type.h"
#include "reflection\qualified_type_ptr.h"
#include "reflection\special_functions.h"
#include "reflection\string_functions.h"
#include "reflection\namespace_member.h"
#include "reflection\type.h"
#include "reflection\primitive_types.h"
#include "reflection\enum_member.h"
#include "reflection\enum.h"
#include "reflection\class_member.h"
#include "reflection\property\property.h"
#include "reflection\property\data_member_property.h"
#include "reflection\class.h"
#include "reflection\obj_ptr.h"
#include "reflection\parameter.h"
#include "reflection\class_template.h"
#include "reflection\namespace.h"
#include "reflection\global_registry.h"
#include "reflection\get_naked_type_traits.h"
#include "reflection\get_naked_type.h"

// inlines
#include "density\density.inl"
#include "reflection\qualified_type_ptr.inl"
#include "reflection\special_functions.inl"
#include "reflection\type.inl"
#include "reflection\enum.inl"
#include "reflection\namespace.inl"
#include "reflection\get_naked_type.inl"

#undef INCLUDING_REFLECTIVE
