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
	// hash<uint32_t>
	template <> uint32_t hash<uint32_t>( const char * name, size_t length )
	{
		/*	djb2 - http://www.cse.yorku.ca/~oz/hash.html
		"this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. another version of
		 this algorithm (now favored by bernstein) uses xor: hash(i) = hash(i - 1) * 33 ^ str[i]; the magic of number
		 33 (why it works better than many other constants, prime or not) has never been adequately explained." */

		/*Hash hash = 5381;
		int c;
		while (c = *str++)
			hash = ((hash << 5) + hash) + c; // hash * 33 + c
		REFLECTIVE_ASSERT( hash == result );*/

		uint32_t result = empty_hash<uint32_t>();
		const char * str = name;
		if( length >= 4 ) do {
			result = (result << 5) + ( result + str[0] );
			result = (result << 5) + ( result + str[1] );
			result = (result << 5) + ( result + str[2] );
			result = (result << 5) + ( result + str[3] );
			str += 4;
			length -= 4;
		} while( length >= 4 );

		if( length ) do {
			result = (result << 5) + ( result + *str++ );
		} while( --length );

		return result;
	}

	// empty_hash<uint32_t>
	template <> uint32_t empty_hash<uint32_t>()
	{
		return 5381;
	}
}
