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

#define INCLUDING_REFLECTIVE

// forward declarations
namespace reflective
{
	class Symbol; // basic reflective object, with a name (of type SymbolName) - defined in core\Symbol.h
		class Type; // provides informations and services related to a type - defined in type\Type.h
			class Class; // a class or struct, with properties, actions and events
			class Enum; // provides informations and services related to an enumeration type
		class ClassMember; // provides informations about a member of a class (such visibility)
			class Property; // generalization of a typed data member (maybe a getter-setter pair)
				template <typename OWNER_CLASS, typename PROPERTY_TYPE> class DataMemberProperty; // a data member
			class Action; // a (non-const) function callable on a class instance
			class Event;
		class Namespace;

	template < typename HASHER, typename STRING >
		class Identifier; // coherent string-hash pair, or just an hash if STRING is void

	template < typename UINT>
		class StringHasher;

	class PtrString;	
}

#include "reflective_settings.h"
#include "reflective_common.h"

// headers

#include "core\Identifier.h"
#include "core\PtrString.h"
#include "core\StringHash.h"
#include "type\type_traits.h"
#include "core\TextInBuffer.h"
#include "core\TextOutBuffer.h"
#include "core\Symbol.h"
#include "core\SymbolList.h"

#include "type\SpecialFunctions.h"
#include "type\Type.h"
#include "class\ClassMember.h"
#include "type\QualifiedTypeRef.h"
#include "class\property\Property.h"
#include "class\Class.h"
#include "GetType.h"
#include "Namespace.h"
#include "type\StaticQualification.h"

// inlines
#include "type\SpecialFunctions.inl"
#include "type\Type.inl"
#include "type\QualifiedTypeRef.inl"
#include "GetType.inl"

#undef INCLUDING_REFLECTIVE
