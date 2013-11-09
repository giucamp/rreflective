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

namespace reflective_externals
{
	// reflection of reflective::ArrayType
	reflective::Class * init_type(
		reflective::ArrayType * null_pointer_1,
		reflective::ArrayType * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::ArrayType ThisClass;
		
		static Class * class_object = null;
		if( class_object != null )
			return class_object;
		
		// class object
		class_object = new_class<ThisClass>( "reflective", "ArrayType" );
		class_object->set_no_base_type();
		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eCopyAssignmentDestructor ) );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "type", &ThisClass::type ),
			new_property<ThisClass>( "count", &ThisClass::count ),
		};
		
		// assign members
		class_object->assign_properties( properties );
		
		// return type
		return class_object;
	}

} // namespace reflective_externals


namespace reflective
{
	// ArrayType::_name_to_string
	void ArrayType::_name_to_string( ToStringBuffer & dest, const QualifiedType & type, size_t count )
	{
		reflective::to_string( dest, type );
		dest.append( '[' );
		reflective::to_string( dest, count );
		dest.append( ']' );
	}

	// ArrayType::_construct_name
	SymbolName ArrayType::_construct_name( const QualifiedType & type, size_t count )
	{
		// compute count

		ToStringBuffer buffer;

		_name_to_string( buffer, type, count );

		const size_t length = buffer.needed_length();

		// allocate the name

		void * allocation = reflective_externals::mem_lifo_alloc( alignment_of( char ), length * sizeof( char ) );

		char * name = new( allocation ) char[ length ];

		// fill with chars

		buffer.init( name, length );

		_name_to_string( buffer, type, count );

		SymbolName result( name, length );

		#if !REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES
			reflective_externals::mem_lifo_free( allocation );
		#endif

		return result;
	}

} // namespace reflective
