
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
	class SpecialFunctions
	{
	public:

		template <typename TYPE>
			static SpecialFunctions from_type();

		using ScalarTor = void (*)(void * i_objects_start, void * i_objects_end );

		using CopyConstructor = void (*)(void * i_objects_start, void * i_objects_end, const void * i_source_start );

		using MoveConstructor = void(*)(void * i_objects_start, void * i_objects_end, void * i_source_start );
		
		SpecialFunctions()
			: m_scalar_default_constructor(nullptr), m_scalar_copy_constructor(nullptr), m_scalar_move_constructor(nullptr), m_scalar_destructor(nullptr) {}

		SpecialFunctions(ScalarTor i_scalar_default_constructor, CopyConstructor i_scalar_copy_constructor,
			MoveConstructor const i_scalar_move_constructor, ScalarTor const i_scalar_destructor);

		ScalarTor const scalar_default_constructor() const			{ return m_scalar_default_constructor; }
		CopyConstructor const scalar_copy_constructor() const		{ return m_scalar_copy_constructor; }
		MoveConstructor const scalar_move_constructor() const		{ return m_scalar_move_constructor; }
		ScalarTor const scalar_destructor() const					{ return m_scalar_destructor;  }

	private:
		ScalarTor m_scalar_default_constructor;
		CopyConstructor m_scalar_copy_constructor;
		MoveConstructor m_scalar_move_constructor;
		ScalarTor m_scalar_destructor;
	};
}
