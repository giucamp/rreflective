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
	void to_string(OutStringBuffer & i_dest, int8_t i_value );
	void to_string(OutStringBuffer & i_dest, int16_t i_value);
	void to_string(OutStringBuffer & i_dest, int32_t i_value);
	void to_string(OutStringBuffer & i_dest, int64_t i_value);

	void to_string(OutStringBuffer & i_dest, uint8_t i_value);
	void to_string(OutStringBuffer & i_dest, uint16_t i_value);
	void to_string(OutStringBuffer & i_dest, uint32_t i_value);
	void to_string(OutStringBuffer & i_dest, uint64_t i_value);

	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, int8_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, int16_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, int32_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, int64_t & o_dest);

	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, uint8_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, uint16_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, uint32_t & o_dest);
	bool assign_from_string(InStringBuffer & i_source, OutStringBuffer & i_error_dest, uint64_t & o_dest);
}
