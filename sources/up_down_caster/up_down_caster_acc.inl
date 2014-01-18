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

	// UpDownCaster<UpDownCasterAllowConstCast>::constructor
	inline UpDownCaster<UpDownCasterAllowConstCast>::UpDownCaster()
		: _to_base_caster( nullptr ),
		  _from_base_caster( nullptr )
	{
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::constructor
	inline UpDownCaster<UpDownCasterAllowConstCast>::UpDownCaster(
			UpDownCaster<UpDownCasterAllowConstCast>::ToBaseCaster to_base_caster,
			UpDownCaster<UpDownCasterAllowConstCast>::FromBaseCaster from_base_caster )
		: _to_base_caster( to_base_caster ),
		  _from_base_caster( from_base_caster )
	{
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::from_types
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterAllowConstCast>
			UpDownCaster<UpDownCasterAllowConstCast>::from_types()
	{
		return UpDownCaster<UpDownCasterAllowConstCast>(
			&_typed_to_base_caster< BASE_CLASS, THIS_CLASS >,
			&_typed_from_base_caster< BASE_CLASS, THIS_CLASS > );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::from_types_only_to_base
	template < class BASE_CLASS, class THIS_CLASS >
		inline UpDownCaster<UpDownCasterAllowConstCast>
			UpDownCaster<UpDownCasterAllowConstCast>::from_types_only_to_base()
	{
		return UpDownCaster<UpDownCasterAllowConstCast>(
			&_typed_to_base_caster< BASE_CLASS, THIS_CLASS >,
			0 );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::pointer_to_base_type
	inline void * UpDownCaster<UpDownCasterAllowConstCast>::pointer_to_base_type( void * pointer ) const
	{
		REFLECTIVE_ASSERT( _to_base_caster != nullptr );
		return (*_to_base_caster)( pointer );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::pointer_from_base_type
	inline void * UpDownCaster<UpDownCasterAllowConstCast>::pointer_from_base_type( void * pointer ) const
	{
		REFLECTIVE_ASSERT( _from_base_caster != nullptr );
		return (*_from_base_caster)( pointer );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::pointer_to_base_type
	inline const void * UpDownCaster<UpDownCasterAllowConstCast>::pointer_to_base_type( const void * pointer ) const
	{
		void * non_const_pointer = const_cast<void *>( pointer );
		return pointer_to_base_type( non_const_pointer );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::pointer_from_base_type
	inline const void * UpDownCaster<UpDownCasterAllowConstCast>::pointer_from_base_type( const void * pointer ) const
	{
		void * non_const_pointer = const_cast<void *>( pointer );
		return pointer_from_base_type( non_const_pointer );
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::_typed_to_base_caster
	template < class BASE_CLASS, class THIS_CLASS >
		void * UpDownCaster<UpDownCasterAllowConstCast>::_typed_to_base_caster( void * pointer )
	{
		// the pointer cannot be nullptr
		REFLECTIVE_ASSERT( pointer != nullptr );

		// this cast is just a type reinterpretation - the compiler shouldn't generate any code
		THIS_CLASS * this_object = static_cast<THIS_CLASS*>( pointer );

		// this cast may require some code
		BASE_CLASS * base_object = this_object;

		// this cast is just a type reinterpretation - the compiler shouldn't generate any code
		void * void_base_object = base_object;

		// done
		return void_base_object;
	}

	// UpDownCaster<UpDownCasterAllowConstCast>::_typed_from_base_caster
	template < class BASE_CLASS, class THIS_CLASS >
		void * UpDownCaster<UpDownCasterAllowConstCast>::_typed_from_base_caster( void * pointer )
	{
		// the pointer cannot be nullptr
		REFLECTIVE_ASSERT( pointer != nullptr );

		// this cast is just a type reinterpretation - the compiler shouldn't generate any code
		BASE_CLASS * base_object = static_cast<BASE_CLASS*>( pointer );

		// this cast may require some code
		THIS_CLASS * this_object = static_cast<THIS_CLASS *>( base_object );

		// this cast is just a type reinterpretation - the compiler shouldn't generate any code
		void * void_this_object = this_object;

		// done
		return void_this_object;
	}	

} // namespace reflective
