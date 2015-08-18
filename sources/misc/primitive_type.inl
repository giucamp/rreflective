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
	// PrimitiveType::constructor
	inline PrimitiveType::PrimitiveType( SymbolName name, size_t size, size_t alignment )
		: Type( Namespace::edit_global(), name, size, alignment )
	{
		set_type_id( eTypePrimitiveType );		
	}

	// PrimitiveType::life_functions
	inline LifeFunctions PrimitiveType::life_functions()
	{
		return LifeFunctions( constructor_func, destructor_func, copier, mover );
	}

	// TypedPrimitiveType::constructor
	template <class TYPE>
		inline TypedPrimitiveType<TYPE>::TypedPrimitiveType( const char * name )
			: PrimitiveType( name, sizeof( TYPE ), alignment_of( TYPE ) )
	{
		set_life_functions( life_functions() );
		set_comparers( equals_comparer, less_than_comparer );
		set_string_functions( 
			&reflective_externals::primitive_to_string<TYPE>,
			&reflective_externals::primitive_assign_from_string<TYPE> );
	}


	// TypedPrimitiveType::equals_comparer
	template <class TYPE>
		bool TypedPrimitiveType<TYPE>::equals_comparer( const void * first_operand, const void * second_operand )
	{
		const TYPE & op1 = *static_cast<const TYPE *>( first_operand );
		const TYPE & op2 = *static_cast<const TYPE *>( second_operand );
		return op1 == op2;
	}

	// TypedPrimitiveType::less_than_comparer
	template <class TYPE>
		bool TypedPrimitiveType<TYPE>::less_than_comparer( const void * first_operand, const void * second_operand )
	{
		const TYPE & op1 = *static_cast<const TYPE *>( first_operand );
		const TYPE & op2 = *static_cast<const TYPE *>( second_operand );
		return op1 < op2;
	}	

	// pointer_type()
	inline const Type & pointer_type()
	{
		static TypedPrimitiveType<void*> pointer_type( "pointer" );
		return pointer_type;
	}

	// get_void_type
	inline const Type & void_type()
	{
		static PrimitiveType void_type( "void", 1, 1 );
		return void_type;
	}

} // namespace reflective

namespace reflective_externals
{
#ifdef _MSC_VER
#define _PRIMITIVE_STRING_FUNCTORS_SPRINTF(FORMAT_SPEC) const int result = sprintf_s( str, 256, FORMAT_SPEC, var )
#define _PRIMITIVE_STRING_FUNCTORS_SCANF(FORMAT_SPEC)	const int result = sscanf_s( source_buffer.chars(), FORMAT_SPEC, typed_object, &read_size )
#else
#define _PRIMITIVE_STRING_FUNCTORS_SPRINTF(FORMAT_SPEC) const int result = sprintf( str, FORMAT_SPEC, var )
#define _PRIMITIVE_STRING_FUNCTORS_SCANF(FORMAT_SPEC)	const int result = sscanf( source_buffer.chars(), FORMAT_SPEC, typed_object, &read_size )
#endif

	// _PRIMITIVE_STRING_FUNCTORS
#define _PRIMITIVE_STRING_FUNCTORS( TYPE, PR_FORMAT_SPEC, SCN_FORMAT_SPEC )	\
		template <> \
			inline void primitive_to_string<TYPE>( reflective::StringOutputStream & dest_buffer,  \
				const reflective::Type & front_type, const void * object ) \
			{ \
				REFLECTIVE_UNUSED( front_type ); \
				const TYPE & var = *(const TYPE*)( object ); \
				char str[ 256 ]; \
				_PRIMITIVE_STRING_FUNCTORS_SPRINTF(PR_FORMAT_SPEC); \
				REFLECTIVE_UNUSED( result ); \
				dest_buffer.append( str ); \
			} \
		template <> \
			inline bool primitive_assign_from_string<TYPE>( reflective::FromStringBuffer & source_buffer, \
				const reflective::Type & front_type, void * object, StringOutputStream & error_buffer ) \
		{ \
			TYPE * typed_object = static_cast<TYPE*>(object); \
			REFLECTIVE_UNUSED( front_type ); size_t read_size = 0; \
			_PRIMITIVE_STRING_FUNCTORS_SCANF(SCN_FORMAT_SPEC "%n"); \
			if( result != 1 ) { error_buffer.append_literal( "not a number" ); return false; } \
			if( read_size < source_buffer.remaining_length() ) { error_buffer.append_literal( "spurious number" ); return false; } \
			return true; \
		} \


	_PRIMITIVE_STRING_FUNCTORS( float, "%f", "%f" );
	_PRIMITIVE_STRING_FUNCTORS( double, "%lf", "%lf" );
	_PRIMITIVE_STRING_FUNCTORS( long double, "%Lf", "%Lf" );
	_PRIMITIVE_STRING_FUNCTORS( int, "%d", "%d" );
	_PRIMITIVE_STRING_FUNCTORS( long, "%d", "%d" );
	_PRIMITIVE_STRING_FUNCTORS( unsigned long, "%d", "%d" );
	_PRIMITIVE_STRING_FUNCTORS( uint8_t, "%"PRId8, "%"SCNd8 );
	_PRIMITIVE_STRING_FUNCTORS( uint16_t, "%"PRId16, "%"SCNd16);
	_PRIMITIVE_STRING_FUNCTORS( uint32_t, "%"PRId32, "%"SCNd32);
	_PRIMITIVE_STRING_FUNCTORS( uint64_t, "%"PRId64, "%"SCNd64);
	_PRIMITIVE_STRING_FUNCTORS( void*, "0x%p", "0x%p" );
	_PRIMITIVE_STRING_FUNCTORS( char, "%d", "%d" );
	_PRIMITIVE_STRING_FUNCTORS( wchar_t, "%d", "%d" );

} // namespace reflective_externals

