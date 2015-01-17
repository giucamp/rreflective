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
	template <class OBJECT, class TYPE>
		Property * new_property( SymbolName name, 
			TYPE (OBJECT::*data), 
			ClassMember::Attributes attributes )
	{
		typedef DataProperty<OBJECT, TYPE> PropertyType;

		DataProperty<OBJECT, TYPE> * result = REFLECTIVE_LIFO_NEW( PropertyType, name, data, attributes );

		return result;
	}

					/**** DataProperty *****/

	// DataProperty::constructor
	template <class ROOT_OBJECT, class TYPE>
		inline DataProperty<ROOT_OBJECT, TYPE>::DataProperty(
			SymbolName name, 
			TYPE (ROOT_OBJECT::*data), 
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<TYPE>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_data( data )
	{
	}

	// DataProperty::on_get_value_inplace
	template <class ROOT_OBJECT, class TYPE>
		void * DataProperty<ROOT_OBJECT, TYPE>::on_get_value_inplace( void * object ) const
	{
		ROOT_OBJECT & obj = *static_cast<ROOT_OBJECT*>( object );
		return &( obj.*_data );
	}
		

				/**** DataProperty (const member) *****/

	// DataProperty::constructor
	template <class ROOT_OBJECT, class TYPE>
		inline DataProperty<ROOT_OBJECT, const TYPE>::DataProperty(
			SymbolName name, 
			const TYPE (ROOT_OBJECT::*data), 
			ClassMember::Attributes attributes )
				: Property( name, safe_get_type<TYPE>(), safe_get_type_qualification<TYPE>(), attributes ), 
					_data( data )
	{
	}
		
	// DataProperty::on_get_value_inplace
	template <class ROOT_OBJECT, class TYPE>
		void * DataProperty<ROOT_OBJECT, const TYPE>::on_get_value_inplace( void * object ) const
	{
		ROOT_OBJECT & obj = *static_cast<ROOT_OBJECT*>( object );
		return &( obj.*_data );
	}
		
} //namespace reflective
