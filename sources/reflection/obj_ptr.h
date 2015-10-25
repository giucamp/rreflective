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
	class ObjPtr
	{
	public:

		ObjPtr() : m_object(nullptr) {  }
		
		ObjPtr(void * i_object, QualifiedTypePtr i_qualified_type)
			: m_object(i_object), m_type(i_qualified_type)
		{
		}

		template <typename TYPE>
			ObjPtr(TYPE * i_object)
				: m_object(i_object), m_type(get_type<TYPE>())
		{

		}

		void * object() const					{ return m_object; }

		const QualifiedTypePtr & type() const	{ return m_type; }

		ObjPtr cast_to(QualifiedTypePtr i_dest_type) const;

		ObjPtr upcast_to(QualifiedTypePtr i_dest_type) const;

		template <typename DEST_TYPE>
			ObjPtr cast_to() const
				{ return cast_to(get_type(DEST_TYPE)); }

	private:		
		void * m_object;
		QualifiedTypePtr m_type;
	};
}