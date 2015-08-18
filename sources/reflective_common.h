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

#ifdef _MSC_VER
	#ifndef _CPPUNWIND
		//#define _HAS_EXCEPTIONS 0
	#endif
#endif
#include <map>
#include <vector>
#ifdef _MSC_VER
	#ifdef _HAS_EXCEPTIONS
		//#undef _HAS_EXCEPTIONS
	#endif
	#pragma warning( push )
	#pragma warning( disable: 4127 ) // conditional expression is constant
#endif
#include <ctype.h>
#include <stdint.h>
#include <type_traits> 
#include <limits>
#include <string>
#include <inttypes.h>

#ifndef REFLECTIVE_IS_DEBUG
	#error REFLECTIVE_IS_DEBUG must be defined
#endif
#ifndef REFLECTIVE_ENABLE_ASSERT
	#error REFLECTIVE_ENABLE_ASSERT must be defined
#endif

// alignment_of - returns the alignment of a type, or 1 if the type in an abstract class
#if defined( _MSC_VER )
	template <bool IsAbstract, typename CLASS > struct _MSC_AlignmentOfImpl;
	template <typename CLASS> struct _MSC_AlignmentOfImpl<true, CLASS> { static const size_t alignment = 1; };
	template <typename CLASS> struct _MSC_AlignmentOfImpl<false, CLASS> { static const size_t alignment = __alignof( CLASS ); };
	#define alignment_of( ... )		( _MSC_AlignmentOfImpl<__is_abstract( __VA_ARGS__ ), __VA_ARGS__>::alignment )
#elif defined( __GNUG__ )
	#define alignment_of( ... )		__alignof__( __VA_ARGS__ )
#endif

#define REFLECTIVE_UNUSED( var )							((void)(var))
#define REFLECTIVE_UNUSED_2( var1, var2 )					((void)(var1), (void)(var2))
#define REFLECTIVE_UNUSED_3( var1, var2, var3 )				((void)(var1), (void)(var2), (void)(var3))
#define REFLECTIVE_UNUSED_4( var1, var2, var3, var4 )		((void)(var1), (void)(var2), (void)(var3), (void)(var4))

#define DX_ENUM_GLOBAL_BITWISE_OR( EnumName )								\
	inline EnumName operator | ( EnumName op1, EnumName op2 )				\
	{																		\
		REFLECTIVE_COMPILETIME_ASSERT( sizeof( EnumName ) == sizeof( int ) );		\
		return static_cast<EnumName>( static_cast<int>( op1 ) | static_cast<int>( op2 ) );	\
	}

#ifdef __GNUG__
    #include <stddef.h>
    #include <stdint.h>
#endif

// REFLECTIVE_COMPILETIME_ASSERT
#define __TOKEN_PASTE_1( first, second )		first##second
#define __TOKEN_PASTE( first, second )			__TOKEN_PASTE_1( first, second )
#define REFLECTIVE_COMPILETIME_ASSERT( bool_expr ) struct __TOKEN_PASTE( __compile_time_assert_, __COUNTER__ ) { int s[ (bool_expr) ? 1 : -1 ]; };

// REFLECTIVE_ASSERT( expr )
#if REFLECTIVE_ENABLE_ASSERT
	#define REFLECTIVE_ASSERT( expr )		if(!(expr)) reflective_externals::assert_failure( #expr ); else (void)0
#else
	#define REFLECTIVE_ASSERT( expr )		(void)0
#endif



