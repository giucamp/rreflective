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

			/**** GlobalFunctionsStringizer ****/

	// GlobalFunctionsStringizer::constructor
	inline GlobalFunctionsStringizer::GlobalFunctionsStringizer( 
		ToStringDumper to_string, FromStringAssigner from_string )
			: _from_string( from_string ), _to_string( to_string )
	{

	}

	// GlobalFunctionsStringizer::from_string
	inline GlobalFunctionsStringizer::FromStringAssigner GlobalFunctionsStringizer::from_string() const
	{
		return _from_string;
	}

	// GlobalFunctionsStringizer::to_string
	inline GlobalFunctionsStringizer::ToStringDumper GlobalFunctionsStringizer::to_string() const
	{
		return _to_string;
	}




				/**** MethodsStringizer ****/

	// MethodsStringizer<TYPE>::lifo_create
	template <class TYPE>	
		MethodsStringizer<TYPE> * MethodsStringizer<TYPE>::lifo_create( 
			ToStringDumper to_string, FromStringAssigner from_string )
	{
		return REFLECTIVE_LIFO_NEW( MethodsStringizer, to_string, from_string );
	}

	// MethodsStringizer<TYPE>::constructor
	template <class TYPE>
		inline MethodsStringizer<TYPE>::MethodsStringizer( 
			ToStringDumper to_string, FromStringAssigner from_string )
				: _to_string( to_string ), _from_string( from_string )
	{

	}

	// MethodsStringizer<TYPE>::to_string - gives a string representation of an object
	template <class TYPE>
		void MethodsStringizer<TYPE>::to_string( ToStringBuffer & dest_buffer,
			const Type & type, const void * object ) const
	{
		REFLECTIVE_UNUSED( type );

		const TYPE & obj = *static_cast< const TYPE * >( object );
		REFLECTIVE_ASSERT( type <= get_type<TYPE>() );
		(obj.*_to_string)( dest_buffer );
	}

	// MethodsStringizer<TYPE>::assign_from_string
	template <class TYPE>
		bool MethodsStringizer<TYPE>::assign_from_string( FromStringBuffer & source_buffer, 
			const Type & type, void * object, 
			ToStringBuffer & error_buffer ) const
	{
		REFLECTIVE_UNUSED( type );

		TYPE & obj = *static_cast< TYPE * >( object );
		REFLECTIVE_ASSERT( type <= get_type<TYPE>() );
		return (obj.*_from_string)( source_buffer, error_buffer );
	}
	
	// MethodsStringizer<TYPE>::from_string
	template <class TYPE>
		inline typename MethodsStringizer<TYPE>::FromStringAssigner
			MethodsStringizer<TYPE>::from_string() const
	{
		return _from_string;
	}

	// MethodsStringizer<TYPE>::to_string
	template <class TYPE>
		inline typename MethodsStringizer<TYPE>::ToStringDumper
			MethodsStringizer<TYPE>::to_string() const
	{
		return _to_string;
	}



} // namespace reflective
