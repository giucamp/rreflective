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
	// QualifiedType
	class QualifiedType
	{
	public:

		// constructor
		QualifiedType();
		QualifiedType( const Type & type );
		QualifiedType( const Type & type, const TypeQualification & type_qualification );
		QualifiedType( const QualifiedType & source );

		// assignment
		QualifiedType & operator = ( const QualifiedType & source );

		// properties
		const Type * type() const;
		const TypeQualification & qualification() const;

		// can_cast_to
		bool can_cast_to( const QualifiedType & dest_type ) const;

		// comparaison
		bool operator == ( const QualifiedType & other ) const;
		bool operator != ( const QualifiedType & other ) const;

		// final_type
		const Type * final_type() const;

		// stringizer
		void to_string( ToStringBuffer & dest_buffer ) const;

	private: // data members
		const Type * _type;
		TypeQualification _type_qualification;
	};

} // namespace reflective

namespace reflective_externals
{

	// reflection of reflective::QualifiedType
	reflective::Class * init_type(
		reflective::QualifiedType * null_pointer_1,
		reflective::QualifiedType * null_pointer_2 );	

} // namespace reflective_externals
