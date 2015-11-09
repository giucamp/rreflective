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

	template < typename HASHER, typename STRING >
		class Identifier; // coherent string-hash pair, or just an hash if STRING is void

	class ObjPtr;
	class QualifiedTypePtr;
	

	template < typename UINT>
		class StringHasher;

	class OutStringBuffer;
	class InStringBuffer;
	class PtrString;
	
	enum class InheritanceSupport
	{
		Functions,
		OffsettingAndFunctions,
		Offsetting,
	};
}

#include "core\fut_std.h"


#include "reflective_settings.h"
#include "reflective_common.h"

// core headers
#include "core\traits.h"
#include "core\in_string_buffer.h"
#include "core\out_string_buffer.h"
#include "core\ptr_string.h"
#include "core\identifier.h"
#include "core\string_hash.h"
#include "core\list.h"

// type headers
#include "reflection\symbol.h"
#include "reflection\updown_caster.h"
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
#include "reflection\get_naked_type.h"
#include "reflection\obj_ptr.h"
#include "reflection\parameter.h"
#include "reflection\class_template.h"
#include "reflection\namespace.h"
#include "reflection\global_registry.h"

// inlines
#include "core\in_string_buffer.inl"
#include "reflection\qualified_type_ptr.inl"
#include "reflection\special_functions.inl"
#include "reflection\type.inl"
#include "reflection\enum.inl"
#include "reflection\namespace.inl"
#include "reflection\get_naked_type.inl"

#undef INCLUDING_REFLECTIVE
