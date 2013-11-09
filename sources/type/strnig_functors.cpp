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


#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{

	// GlobalFunctionsStringizer::lifo_create
	GlobalFunctionsStringizer * GlobalFunctionsStringizer::lifo_create( 
		ToStringDumper to_string, FromStringAssigner from_string )
	{
		return REFLECTIVE_LIFO_NEW( GlobalFunctionsStringizer, to_string, from_string );
	}

	// GlobalFunctionsStringizer::to_string - gives a string representation of an object
	void GlobalFunctionsStringizer::to_string( ToStringBuffer & dest_buffer,
		const Type & type, const void * object ) const
	{
		(*_to_string)( dest_buffer, type, object );
	}

	// GlobalFunctionsStringizer::assign_from_string
	bool GlobalFunctionsStringizer::assign_from_string( FromStringBuffer & source_buffer, 
		const Type & type, void * object, 
		ToStringBuffer & error_buffer ) const
	{
		return (*_from_string)( source_buffer, type, object, error_buffer );
	}

} // namespace reflective

