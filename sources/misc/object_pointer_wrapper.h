
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

#ifndef _INCLUDING_REFLECTIVE
	#error include "reflective.h" instead of including this file
#endif

namespace reflective
{
	/** Pair of void object pointer and qualified type.
		This class is useful to handle objects whose type is know only at runtime.
	*/
	class ObjectPointerWrapper
	{
	public:

		ObjectPointerWrapper()
			: m_object( nullptr ) { }

		ObjectPointerWrapper( void * i_object, const QualifiedType & i_type )
			: m_object( i_object ), m_type( i_type ) { }

		ObjectPointerWrapper( void * i_object, const Type & i_type )
			: m_object( i_object ), m_type( QualifiedType( i_type ) ) { }

		ObjectPointerWrapper( const ObjectPointerWrapper & i_source )
			: m_object( i_source.m_object ), m_type( i_source.m_type ) {}

		ObjectPointerWrapper & operator = ( const ObjectPointerWrapper & i_source )
		{
			m_object = i_source.m_object;
			m_type = i_source.m_type;
			return *this;
		}

		void * object() const					{ return m_object; }

		const QualifiedType & type() const		{ return m_type; }

		template< typename TYPE>
			static ObjectPointerWrapper from_pointer( const TYPE * i_object )
				{ return ObjectPointerWrapper( &i_object, qualified_type_of(i_object) ); }
	
	private:
		void * m_object;
		QualifiedType m_type;
	};

}