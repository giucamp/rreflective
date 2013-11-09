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

#ifndef _REFLECTIVE_INCLUDED
#define _REFLECTIVE_INCLUDED
#define _INCLUDING_REFLECTIVE

// reflectve_externals
#include "reflectve_externals.h" /* this is an user-supplied header which specifies compile-time 
	options. You can use default_reflectve_externals.h as template and edit it for your needs. */

#include "reflective_common.h"

namespace reflective
{
	// forwards
	class Namespace;
	class Allocator;
	class CollectionHandler;
	class Type;

	// null
	extern class Null
	{
	public:
		template <class TYPE> operator TYPE * () const		{ return 0; }
	} null;

	// SymbolTypeId
	enum SymbolTypeId
	{
		eTypeSymbol = 0,
		eTypeNamespace,
		eTypeType,
		eTypeClass,
		eTypePrimitiveType,
		eTypeEnum,
		eTypeEnumMember,
		eTypeProperty,
		eTypeEvent,
		eTypeAction,
		eTypeParameter,
		eTypeClassTemplate,
		eFirstUserTypeId,
		eMaxTypeIdCount = 16,
	};
}



// headers
#include "mem\memory.h"
#include "misc\unreferencer.h"
#include "misc\static_const_string.h"
#include "misc\to_string_buffer.h"
#include "misc\from_string_buffer.h"
#include "misc\lexical_parser.h"
#include "misc\name_id.h"
#include "symbol\symbol.h"
#include "symbol\symbol_list.h"
#include "up_down_caster\up_down_caster.h"
#include "type\default_life_functions.h"
#include "type\life_functions.h"
#include "type\strnig_functors.h"
#include "type\type.h"
#include "type\type_qualification.h"
#include "type\qualified_type.h"
#include "type\namespace.h"
#include "misc\primitive_type.h"
#include "misc\enum.h"
#include "misc\event.h"
#include "misc\handler.h"
#include "misc\auto_string_buffer.h"
#include "class\class_member.h"
#include "class\property.h"
#include "class\data_property.h"
#include "class\method_property.h"
#include "class\parameter.h"
#include "class\action.h"
#include "class\parameter_list_stringizer.h"
#include "class\method_action.h"
#include "class\template_argument.h"
#include "class\event_descriptor.h"
#include "class\class.h"
#include "class\class_template.h"
#include "class\type_inspector.h"
#include "collections\abstract_iterator.h"
#include "collections\watch.h"
#include "collections\watch_chain.h"
#include "collections\collection_handler.h"
#include "iterators\iterators.h"
#include "array\array_type.h"
#include "array\array_data_property.h"
#include "type\type_traits.h"
#include "functions\functions.h"

// inlines
#include "mem\memory.inl"
#include "misc\static_const_string.inl"
#include "misc\to_string_buffer.inl"
#include "misc\from_string_buffer.inl"
#include "misc\lexical_parser.inl"
#include "misc\auto_string_buffer.inl"
#include "misc\name_id.inl"
#include "misc\name_id_pn.inl"
#include "symbol\symbol.inl"
#include "symbol\symbol_list_collection.h"
#include "symbol\symbol_list.inl"
#include "symbol\symbol_list_collection.inl"
#include "up_down_caster\up_down_caster.inl"
#include "type\default_life_functions.inl"
#include "type\life_functions.inl"
#include "type\strnig_functors.inl"
#include "type\type.inl"
#include "type\type_qualification.inl"
#include "type\qualified_type.inl"
#include "type\namespace.inl"
#include "misc\primitive_type.inl"
#include "misc\enum.inl"
#include "misc\event.inl"
#include "misc\handler.inl"
#include "class\class_member.inl"
#include "class\property.inl"
#include "class\data_property.inl"
#include "class\method_property.inl"
#include "class\parameter.inl"
#include "class\action.inl"
#include "class\method_action_0.inl"
#include "class\method_action_1.inl"
#include "class\method_action_2.inl"
#include "class\method_action_3.inl"
#include "class\method_action_4.inl"
#include "class\method_action_5.inl"
#include "class\method_action_6.inl"
#include "class\method_action_7.inl"
#include "class\method_action_8.inl"
#include "class\event_descriptor.inl"
#include "class\template_argument.inl"
#include "class\type_inspector.inl"
#include "class\class.inl"
#include "class\class_template.inl"
#include "collections\abstract_iterator.inl"
#include "collections\watch.inl"
#include "collections\watch_chain.inl"
#include "iterators\iterators.inl"
#include "array\array_type.inl"
#include "array\array_data_property.inl"
#include "type\type_traits.inl"
#include "functions\functions.inl"

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#undef _INCLUDING_REFLECTIVE
#endif // #ifndef _REFLECTIVE_INCLUDED

