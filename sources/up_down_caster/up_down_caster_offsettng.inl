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

	// UpDownCaster<UpDownCasterOffsetting>::constructor
	inline UpDownCaster<UpDownCasterOffsetting>::UpDownCaster()
		: _offset( 0 )
	{
	}

	// UpDownCaster<UpDownCasterOffsetting>::constructor
	inline UpDownCaster<UpDownCasterOffsetting>::UpDownCaster( int offset )
		: _offset( offset )
	{
	}

	// UpDownCaster<UpDownCasterOffsetting>::from_types
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterOffsetting>
			UpDownCaster<UpDownCasterOffsetting>::from_types()
	{
		const THIS_CLASS * volatile pointer_to_this = reinterpret_cast<const THIS_CLASS* const>( sizeof( THIS_CLASS ) );
		const BASE_CLASS * volatile pointer_to_base = pointer_to_this;

		const void * const void_pointer_to_this = pointer_to_this;
		const void * const void_pointer_to_base = pointer_to_base;

		size_t const uint_pointer_to_this = reinterpret_cast<size_t const>( void_pointer_to_this );
		size_t const uint_pointer_to_base = reinterpret_cast<size_t const>( void_pointer_to_base );

		int offset;

		if( uint_pointer_to_this > uint_pointer_to_base )
		{
			size_t const uoffset = uint_pointer_to_this - uint_pointer_to_base;
			offset = static_cast<int>( uoffset );
			REFLECTIVE_ASSERT( uoffset == static_cast<size_t>( offset ) );
			offset = -offset;
		}
		else
		{
			size_t const uoffset = uint_pointer_to_base - uint_pointer_to_this;
			offset = static_cast<int>( uoffset );
			REFLECTIVE_ASSERT( uoffset == static_cast<size_t>( offset ) );
		}

		return UpDownCaster<UpDownCasterOffsetting>( offset );
	}

	// UpDownCaster<UpDownCasterOffsetting>::from_types_only_to_base
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterOffsetting>
			UpDownCaster<UpDownCasterOffsetting>::from_types_only_to_base()
	{
		const THIS_CLASS * volatile pointer_to_this = reinterpret_cast<const THIS_CLASS* const>( sizeof( THIS_CLASS ) );
		const BASE_CLASS * volatile pointer_to_base = pointer_to_this;

		const void * const void_pointer_to_this = pointer_to_this;
		const void * const void_pointer_to_base = pointer_to_base;

		size_t const uint_pointer_to_this = reinterpret_cast<size_t const>( void_pointer_to_this );
		size_t const uint_pointer_to_base = reinterpret_cast<size_t const>( void_pointer_to_base );

		int offset;

		if( uint_pointer_to_this > uint_pointer_to_base )
		{
			size_t const uoffset = uint_pointer_to_this - uint_pointer_to_base;
			offset = static_cast<int>( uoffset );
			REFLECTIVE_ASSERT( uoffset == static_cast<size_t>( offset ) );
			offset = -offset;
		}
		else
		{
			size_t const uoffset = uint_pointer_to_base - uint_pointer_to_this;
			offset = static_cast<int>( uoffset );
			REFLECTIVE_ASSERT( uoffset == static_cast<size_t>( offset ) );
		}

		return UpDownCaster<UpDownCasterOffsetting>( offset );
	}

	// UpDownCaster<UpDownCasterOffsetting>::pointer_to_base_type
	inline void * UpDownCaster<UpDownCasterOffsetting>::pointer_to_base_type( void * pointer ) const
	{
		size_t uint_pointer = reinterpret_cast<size_t>( pointer );
		uint_pointer += _offset;
		return reinterpret_cast<void *>( uint_pointer );
	}

	// UpDownCaster<UpDownCasterOffsetting>::pointer_from_base_type
	inline void * UpDownCaster<UpDownCasterOffsetting>::pointer_from_base_type( void * pointer ) const
	{
		size_t uint_pointer = reinterpret_cast<size_t>( pointer );
		uint_pointer -= _offset;
		return reinterpret_cast<void *>( uint_pointer );
	}

	// UpDownCaster<UpDownCasterOffsetting>::pointer_to_base_type
	inline const void * UpDownCaster<UpDownCasterOffsetting>::pointer_to_base_type( const void * pointer ) const
	{
		size_t uint_pointer = reinterpret_cast<size_t>( pointer );
		uint_pointer += _offset;
		return reinterpret_cast<const void *>( uint_pointer );
	}

	// UpDownCaster<UpDownCasterOffsetting>::pointer_from_base_type
	inline const void * UpDownCaster<UpDownCasterOffsetting>::pointer_from_base_type( const void * pointer ) const
	{
		size_t uint_pointer = reinterpret_cast<size_t>( pointer );
		uint_pointer -= _offset;
		return reinterpret_cast<const void *>( uint_pointer );
	}

} // namespace reflective
