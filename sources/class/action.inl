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
	// Action::constructor
	inline Action::Action( SymbolName name, Attributes attributes, const Type & return_type )
		: ClassMember( name, attributes ), _return_type( return_type )
	{
		set_type_id( eTypeAction );
	}

	// Action::constructor
	inline Action::Action( SymbolName name, Attributes attributes, const Type & return_type,
			const TypeQualification & return_type_qualification )
		: ClassMember( name, attributes ), _return_type( return_type ),
		  _return_type_qualification( return_type_qualification )
	{
		set_type_id( eTypeAction );
	}

	// Action::return_type
	inline const Type & Action::return_type() const
	{
		return _return_type;
	}

	// Action::return_type_qualification
	inline const TypeQualification & Action::return_type_qualification() const
	{
		return _return_type_qualification;
	}

	// Action::invoke
	inline bool Action::invoke( void * object, const void * actual_parameters, void * return_value ) const
	{
		ToStringBuffer empty_reason_text;
		if( !can_invoke( object, empty_reason_text ) )
			return false;

		REFLECTIVE_ASSERT( mem::is_aligned( return_value, _return_type.alignment() ) );

		REFLECTIVE_ASSERT( mem::is_aligned( actual_parameters, _parameter_list.total_alignment() ) );

		on_invoke( object, actual_parameters, return_value );

		return true;
	}

	// Parameter::edit_parameter_list
	inline ParameterList & Action::edit_parameter_list()
	{
		return _parameter_list;
	}

	// Parameter::parameter_list
	inline const ParameterList & Action::parameter_list() const
	{
		return _parameter_list;
	}

} // namespace reflective
