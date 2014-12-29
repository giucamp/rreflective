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
	/* Unreferencer - just translates a reference type to a const pointer type, which 
		is useful when a reference or pointer to a template type is necessary (the C++ does not 
		allow pointer to references nor reference to references, but they are equivalent to const
		pointers; indeed as a reference can be converted to a const pointer with the address-of 
		operator '&'). If the template argument is not a reference, it is not altered. */
	template <class TYPE> class Unreferencer
	{ 
		public:
			typedef TYPE Type; 

			static TYPE & void_indirection( void * address )
			{
				return *static_cast<TYPE*>( address );
			}

			static const TYPE & void_indirection( const void * address )
			{
				return *static_cast<const TYPE*>( address );
			}

			static void * address_of( const TYPE & )
			{
				return *static_cast<const TYPE*>( address );
			}

			static void copy_construct( void * destination, TYPE source )
			{
				new( destination ) TYPE( source );
			}
	};
	template <class TYPE> class Unreferencer<TYPE&>
	{
		public:
			typedef TYPE * const Type;

			static TYPE & void_indirection( void * address )
			{
				return **static_cast<TYPE**>( address );
			}

			static TYPE & void_indirection( const void * address )
			{
				return **static_cast<TYPE*const *>( address );
			}

			static void copy_construct( void * destination, const TYPE & source )
			{
				new( destination ) ( const TYPE*)( &source );
			}
	};

} // namespace reflective
