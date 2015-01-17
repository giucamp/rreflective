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
	// QualifiedType::default constructor
	inline QualifiedType::QualifiedType()
		: m_font_type( nullptr )
	{
	}
	// QualifiedType::copy constructor
	inline QualifiedType::QualifiedType( const QualifiedType & source )
		: m_font_type( source.m_font_type ), m_type_qualification( source.m_type_qualification )
	{
	}

	// QualifiedType::constructor( type, type_qualification )
	inline QualifiedType::QualifiedType( const Type & type, const TypeQualification & type_qualification )
		: m_font_type( &type ), m_type_qualification( type_qualification )
	{
	}

	// QualifiedType::constructor( type, type_qualification )
	inline QualifiedType::QualifiedType( const Type & type, unsigned number_of_indirection_levels, const reflective::Type & final_type, unsigned constness_word )
		: m_font_type( &type ), m_type_qualification( number_of_indirection_levels, final_type, constness_word )
	{
	}

	// QualifiedType::assignment
	inline QualifiedType & QualifiedType::operator = ( const QualifiedType & source )		
	{
		m_font_type = source.m_font_type;
		m_type_qualification = source.m_type_qualification;
		return *this;
	}

	// QualifiedType::constructor( type )
	inline QualifiedType::QualifiedType( const Type & type )
		: m_font_type( &type )
	{
	}

	// QualifiedType::type
	inline const Type * QualifiedType::front_type() const
	{
		return m_font_type;
	}

	// QualifiedType::type_qualification
	inline const TypeQualification & QualifiedType::qualification() const
	{
		return m_type_qualification;
	}


	// QualifiedType::can_cast_to
	inline bool QualifiedType::can_cast_to( const QualifiedType & dest_type ) const
	{
		if( m_font_type && dest_type.m_font_type )
			return m_font_type->can_cast_to( *dest_type.m_font_type );
		return true;
	}

	// QualifiedType::change_final_type
	inline void QualifiedType::change_final_type( const Type * i_new_final_type )
	{
		m_type_qualification.change_final_type( i_new_final_type );
	}

	// QualifiedType::operator ==
	inline bool QualifiedType::operator == ( const QualifiedType & other ) const
	{
		if( m_font_type == other.m_font_type && m_font_type )
		{
			REFLECTIVE_ASSERT( *m_font_type == *other.m_font_type );
		}

		return m_font_type == other.m_font_type &&
			m_type_qualification == other.m_type_qualification;
	}

	// QualifiedType::operator !=
	inline bool QualifiedType::operator != ( const QualifiedType & other ) const
	{
		return *m_font_type != *other.m_font_type ||
			m_type_qualification != other.m_type_qualification;
	}
	
} // namespace reflective


