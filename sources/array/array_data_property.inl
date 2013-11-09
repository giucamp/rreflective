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
	// new_property( name, &Class::data, [attributes] )
	template <class ROOT_OBJECT, class OBJECT, class TYPE, size_t SIZE>
		Property * new_property( SymbolName name, 
			TYPE (OBJECT::*data)[ SIZE ], 
			ClassMember::Attributes attributes )
	{
		ROOT_OBJECT * just_a_type_check = (OBJECT*)null;
		REFLECTIVE_UNUSED( just_a_type_check );

		typedef TYPE (ROOT_OBJECT::*BasePointer )[ SIZE ];
		BasePointer base_data = static_cast<BasePointer>( data );
			
		typedef ArrayDataProperty<ROOT_OBJECT, TYPE, SIZE> ThisClass;
		
		ArrayDataProperty<ROOT_OBJECT, TYPE, SIZE> * result =
			REFLECTIVE_LIFO_NEW( ThisClass, name, base_data, attributes );

		return result;
	}

					/**** ArrayDataProperty *****/

	// ArrayDataProperty::constructor
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline ArrayDataProperty<ROOT_OBJECT, TYPE, SIZE>::ArrayDataProperty(
			SymbolName name, 
			TYPE (ROOT_OBJECT::*data)[ SIZE ], 
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<ArrayType>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_data( data )
	{
	}

	// ArrayDataProperty::on_set_value
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline bool ArrayDataProperty<ROOT_OBJECT, TYPE, SIZE>::on_set_value( void * object, const void * value ) const
	{
		ROOT_OBJECT & obj = *static_cast<ROOT_OBJECT*>( object );
		const TYPE * source = static_cast<const TYPE *>( value );
		for( size_t index = 0; index < SIZE; index++, source++ )
			(obj.*_data)[index] = *static_cast<const TYPE*>( source );
		return true;
	}

	// ArrayDataProperty::on_get_value
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline bool ArrayDataProperty<ROOT_OBJECT, TYPE, SIZE>::on_get_value( const void * object, void * value ) const
	{
		const ROOT_OBJECT & obj = *static_cast<const ROOT_OBJECT*>( object );
		TYPE * dest = static_cast<TYPE *>( value );
		for( size_t index = 0; index < SIZE; index++, dest++ )
			new( dest ) TYPE( (obj.*_data)[index] );
		return true;
	}



				/**** ArrayDataProperty (const member) *****/

	// ArrayDataProperty::constructor
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline ArrayDataProperty<ROOT_OBJECT, const TYPE, SIZE>::ArrayDataProperty(
			SymbolName name, 
			const TYPE (ROOT_OBJECT::*data)[ SIZE ], 
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<ArrayType>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_data( data )
	{
	}

	// ArrayDataProperty::on_set_value
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline bool ArrayDataProperty<ROOT_OBJECT, const TYPE, SIZE>::on_set_value( void * object, const void * value ) const
	{
		return false;
	}

	// ArrayDataProperty::on_get_value
	template <class ROOT_OBJECT, class TYPE, size_t SIZE>
		inline bool ArrayDataProperty<ROOT_OBJECT, const TYPE, SIZE>::on_get_value( const void * object, void * value ) const
	{
		const ROOT_OBJECT & obj = *static_cast<const ROOT_OBJECT*>( object );
		TYPE * dest = static_cast<TYPE *>( value );
		for( size_t index = 0; index < SIZE; index++, dest++ )
			new( dest ) TYPE( (obj.*_data)[index] );
		return true;
	}


} //namespace reflective
