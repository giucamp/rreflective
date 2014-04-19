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
	// Property::constructor
	inline Property::Property( SymbolName name, const Type & type,
		const TypeQualification & type_qualification, ClassMember::Attributes attributes )
			: ClassMember( name, attributes ), _qualified_type( type, type_qualification )
	{
		set_type_id( eTypeProperty );
	}

	// Property::qualification
	inline const TypeQualification & Property::qualification() const
	{
		return _qualified_type.qualification();
	}

	// Property::type
	inline const Type & Property::type() const
	{
		return *_qualified_type.type();
	}

	// Property::qualified_type
	inline const QualifiedType & Property::qualified_type() const
	{
		return _qualified_type;
	}

	// Property::set_value
	inline bool Property::set_value( void * object, const void * value ) const
	{
		return on_set_value( object, value );
	}

	// Property::get_value_inplace
	inline void * Property::get_value_inplace( void * object ) const
	{
		return on_get_value_inplace( object );
	}

	// Property::get_value_inplace - const
	inline const void * Property::get_value_inplace( const void * object ) const
	{
		return on_get_value_inplace( const_cast<void*>( object ) );
	}

	// Property::get_value
	inline bool Property::get_value( const void * object, void * value ) const
	{
		return on_get_value( object, value );
	}

	// Property::type_has_tostring
	inline bool Property::type_has_tostring() const
	{
		const Type * type = _qualified_type.final_type();
		return type->check_capabilities( Type::eHasToString );
	}
}
