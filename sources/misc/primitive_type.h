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
	// PrimitiveType
	class PrimitiveType : public Type
	{
	public:

		// constructor
		PrimitiveType( SymbolName name, size_t size, size_t alignment );

		// primitive life functors
		static LifeFunctions life_functions();
		static void constructor_func( void * start_address, void * end_address );
		static void destructor_func( void * start_address, void * end_address );
		static void copier( void * dest_start, void * dest_end, const void * source_start );
		static void mover( void * dest_start, void * dest_end, void * source_start );
	};

	// TypedPrimitiveType
	template <class TYPE>
		class TypedPrimitiveType : public reflective::PrimitiveType
	{
	public:
		TypedPrimitiveType( const char * name );

		static bool equals_comparer( const void * first_operand, const void * second_operand );
		static bool less_than_comparer( const void * first_operand, const void * second_operand );
	};

	// pointer_type()
	const Type & pointer_type();

	// get_void_type
	const Type & void_type();

} // namespace reflective


namespace reflective_externals
{
	using namespace reflective;

	template <class TYPE>
		bool primitive_assign_from_string( FromStringBuffer & source_buffer, 
			const reflective::Type & type, void * object, StringOutputStream & error_buffer );

	template <class TYPE>
		void primitive_to_string( 
			reflective::StringOutputStream & dest_buffer, 
				const reflective::Type & type, const void * object );


	#define IMPLEMENT_PRIMITIVE_TYPE( PRIMITIVE_TYPE ) \
		inline reflective::PrimitiveType * init_type( \
			PRIMITIVE_TYPE * null_pointer_1, PRIMITIVE_TYPE * null_pointer_2 ) \
				{ static reflective::TypedPrimitiveType<PRIMITIVE_TYPE> type( #PRIMITIVE_TYPE ); \
					REFLECTIVE_UNUSED( null_pointer_1); REFLECTIVE_UNUSED( null_pointer_2 ); return &type; } \
		template <> void primitive_to_string<PRIMITIVE_TYPE>( reflective::StringOutputStream & dest_buffer,  \
				const reflective::Type & type, const void * object ); \
		template <> bool primitive_assign_from_string<PRIMITIVE_TYPE>( reflective::FromStringBuffer & source_buffer, \
				const reflective::Type & type, void * object, StringOutputStream & error_buffer );

	IMPLEMENT_PRIMITIVE_TYPE( bool )
	IMPLEMENT_PRIMITIVE_TYPE( char )
	IMPLEMENT_PRIMITIVE_TYPE( wchar_t )
	IMPLEMENT_PRIMITIVE_TYPE( long )
	IMPLEMENT_PRIMITIVE_TYPE( int )
	IMPLEMENT_PRIMITIVE_TYPE( uint8_t )
	IMPLEMENT_PRIMITIVE_TYPE( uint16_t )
	IMPLEMENT_PRIMITIVE_TYPE( uint32_t )
	IMPLEMENT_PRIMITIVE_TYPE( uint64_t )
	IMPLEMENT_PRIMITIVE_TYPE( float )
	IMPLEMENT_PRIMITIVE_TYPE( double )
	IMPLEMENT_PRIMITIVE_TYPE( unsigned long )

	#undef IMPLEMENT_PRIMITIVE_TYPE

} // namespace reflective_externals

namespace reflective_externals
{
	// reflection of reflective::reflective::PrimitiveType
	reflective::Class * init_type(
		reflective::PrimitiveType * null_pointer_1,
		reflective::PrimitiveType * null_pointer_2 );
}
