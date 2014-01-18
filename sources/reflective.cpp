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

#include "reflective.h"

#define _IMPLEMENTING_REFLECTIVE

#include "mem\memory.cpp"
#include "misc\static_const_string.cpp"
#include "misc\name_id.cpp"
#include "misc\from_string_buffer.cpp"
#include "misc\to_string_buffer.cpp"
#include "misc\auto_string_buffer.cpp"
#include "symbol\symbol.cpp"
#include "type\life_functions.cpp"
#include "type\strnig_functors.cpp"
#include "type\type.cpp"
#include "type\namespace.cpp"
#include "type\namespace_reflection.cpp"
#include "type\type_qualification.cpp"
#include "type\qualified_type.cpp"
#include "misc\primitive_type.cpp"
#include "misc\enum.cpp"
#include "class\class_member.cpp"
#include "class\property.cpp"
#include "class\action.cpp"
#include "class\event_descriptor.cpp"
#include "class\class.cpp"
#include "class\class_template.cpp"
#include "class\parameter.cpp"
#include "class\parameter_list_stringizer.cpp"
#include "class\template_argument.cpp"
#include "class\type_inspector.cpp"
#include "collections\watch.cpp"
#include "collections\watch_chain.cpp"
#include "misc\event.cpp"
#include "functions\functions.cpp"
#include "array\array_type.cpp"


#undef _IMPLEMENTING_REFLECTIVE

