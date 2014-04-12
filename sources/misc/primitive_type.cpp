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
	// PrimitiveType::constructor_func
	void PrimitiveType::constructor_func( void * start_address, void * end_address )
	{
		const size_t count = reinterpret_cast<size_t>( end_address )
			- reinterpret_cast<size_t>( start_address );
		memset( start_address, 0, count );
	}

	// PrimitiveType::destructor_func
	void PrimitiveType::destructor_func( void * start_address, void * end_address )
	{
		REFLECTIVE_UNUSED_2( start_address, end_address );
		#if REFLECTIVE_IS_DEBUG
			mem::dbg_invalidate( start_address, end_address );
		#endif
	}

	// PrimitiveType::copier
	void PrimitiveType::copier( void * dest_start, void * dest_end, const void * source_start )
	{
		const size_t count = reinterpret_cast<size_t>( dest_end )
			- reinterpret_cast<size_t>( dest_start );
		memcpy( dest_start, source_start, count );
	}

	// PrimitiveType::primitive_mover
	void PrimitiveType::mover( void * dest_start, void * dest_end, void * source_start )
	{
		const size_t count = reinterpret_cast<size_t>( dest_end )
			- reinterpret_cast<size_t>( dest_start );
		memmove( dest_start, source_start, count );
	}
} // namespace reflective

namespace reflective_externals
{

	template <>
		void primitive_to_string<bool>( reflective::StringOutputStream & dest_buffer,
			const reflective::Type & type, const void * object )
	{
		REFLECTIVE_ASSERT( type == reflective::get_type<bool>() );
		REFLECTIVE_UNUSED( type );

		const bool value = *static_cast<const bool*>( object );
		if( value )
			dest_buffer.append_literal( "true" );
		else
			dest_buffer.append_literal( "false" );
	}

	template <>
		bool primitive_assign_from_string<bool>( reflective::FromStringBuffer & source_buffer,
			const reflective::Type & type, void * object, reflective::StringOutputStream & error_buffer )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			REFLECTIVE_ASSERT( type == reflective::get_type<bool>() );
		#else
			REFLECTIVE_UNUSED( type );
		#endif

		bool & value = *static_cast<bool*>( object );

		if( source_buffer.accept_literal( "true", reflective::FromStringBuffer::eIgnoreCase ) )
		{
			value = true;
			return true;
		}

		if( source_buffer.accept_literal( "false", reflective::FromStringBuffer::eIgnoreCase ) )
		{
			value = false;
			return true;
		}

		error_buffer.append_literal( "expected \'true\' or \'false\'\n" );
		return false;
	}
}

namespace reflective_externals
{
	// reflection of reflective::reflective::PrimitiveType
	void init_type( reflective::Type * volatile * o_result,
		reflective::PrimitiveType * null_pointer_1,
		reflective::PrimitiveType * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::PrimitiveType ThisClass;
		typedef reflective::Type BaseClass;
		
		if( *o_result != nullptr )
			return;

		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "PrimitiveType" );
		*o_result = class_object;
		
		// services
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
	}

} //namespace reflective_externals

