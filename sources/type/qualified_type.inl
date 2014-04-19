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
		: _type( nullptr )
	{
	}

	// QualifiedType::copy constructor
	inline QualifiedType::QualifiedType( const QualifiedType & source )
		: _type( source._type ), _type_qualification( source._type_qualification )
	{
	}

	// QualifiedType::constructor( type, type_qualification )
	inline QualifiedType::QualifiedType( const Type & type, const TypeQualification & type_qualification )
		: _type( &type ), _type_qualification( type_qualification )
	{
	}

	// QualifiedType::constructor( type, type_qualification )
	inline QualifiedType::QualifiedType( const Type & type, unsigned number_of_indirection_levels, const reflective::Type & final_type, unsigned constness_word )
		: _type( &type ), _type_qualification( number_of_indirection_levels, final_type, constness_word )
	{
	}

	// QualifiedType::assignment
	inline QualifiedType & QualifiedType::operator = ( const QualifiedType & source )		
	{
		_type = source._type;
		_type_qualification = source._type_qualification;
		return *this;
	}

	// QualifiedType::constructor( type )
	inline QualifiedType::QualifiedType( const Type & type )
		: _type( &type )
	{
	}

	// QualifiedType::type
	inline const Type * QualifiedType::type() const
	{
		return _type;
	}

	// QualifiedType::type_qualification
	inline const TypeQualification & QualifiedType::qualification() const
	{
		return _type_qualification;
	}


	// QualifiedType::can_cast_to
	inline bool QualifiedType::can_cast_to( const QualifiedType & dest_type ) const
	{
		if( _type && dest_type._type )
			return _type->can_cast_to( *dest_type._type );
		return true;
	}

	// QualifiedType::operator ==
	inline bool QualifiedType::operator == ( const QualifiedType & other ) const
	{
		if( _type == other._type && _type )
		{
			REFLECTIVE_ASSERT( *_type == *other._type );
		}

		return _type == other._type &&
			_type_qualification == other._type_qualification;
	}

	// QualifiedType::operator !=
	inline bool QualifiedType::operator != ( const QualifiedType & other ) const
	{
		return *_type != *other._type ||
			_type_qualification != other._type_qualification;
	}

	// FullName
	class QualifiedType::FullName
	{
	public:

		typedef FullName ThisClass;

		static void init_class( reflective::Class & class_obj )
		{
			class_obj.set_string_functions( &ThisClass::to_string );
		}

		FullName() {}

		FullName( const QualifiedType  & i_qualified_type )
			: m_qualified_type( i_qualified_type ) {}

		void to_string( StringOutputStream & dest_buffer ) const
		{
			m_qualified_type.to_string( dest_buffer );
		}

	private:
		QualifiedType m_qualified_type;
	};

	// QualifiedType::full_name
	inline QualifiedType::FullName QualifiedType::full_name() const
		{ return FullName( *this ); }

} // namespace reflective


