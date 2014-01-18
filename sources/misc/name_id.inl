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
	// hash( string )
	template <class UINT>
		inline UINT hash( const char * string )
	{
		const size_t length = strlen( string );
		return hash<UINT>( string, length );
	}

	template <class UINT>
		inline UINT hash( const StaticConstString & string )
	{
		return hash<UINT>( string.start_of_chars(), string.length() );
	}

	// NameIdentifier::default constructor
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false>::NameIdentifier()
			: _hash( empty_hash<UINT>() )
	{

	}

	// NameIdentifier::empty
	template <class UINT, class STRING>
		NameIdentifier<UINT, STRING, false> NameIdentifier<UINT, STRING, false>::empty;

	// NameIdentifier::constructor( const char * )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false>::NameIdentifier( const STRING & string )
			: _hash( hash<UINT>( string ) )
	{

	}

	// NameIdentifier::constructor( const char * )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false>::NameIdentifier( const char * string )
			: _hash( hash<UINT>( string ) )
	{

	}

	// NameIdentifier::constructor( const char * )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false>::NameIdentifier( const char * string, size_t length )
			: _hash( hash<UINT>( string, length ) )
	{

	}

	// NameIdentifier::constructor( NameIdentifier )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false>::NameIdentifier( const NameIdentifier & name )
			: _hash( name._hash )
	{

	}

	// NameIdentifier<true, UINT>::operator =
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, false> &
			NameIdentifier<UINT, STRING, false>::operator = ( const NameIdentifier & source )
	{
		_hash = source._hash;
		return *this;
	}

	// NameIdentifier<UINT, STRING, false>::uint_hash
	template <class UINT, class STRING>
		inline typename NameIdentifier<UINT, STRING, false>::UIntHash
			NameIdentifier<UINT, STRING, false>::uint_hash() const
	{
		return _hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator ==
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator == ( const NameIdentifier & op ) const
	{
		return _hash == op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator !=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator != ( const NameIdentifier & op ) const
	{
		return _hash != op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator >=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator >= ( const NameIdentifier & op ) const
	{
		return _hash >= op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator >
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator > ( const NameIdentifier & op ) const
	{
		return _hash > op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator <=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator <= ( const NameIdentifier & op ) const
	{
		return _hash <= op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::operator <
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::operator < ( const NameIdentifier & op ) const
	{
		return _hash < op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::is_empty
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, false>::is_empty() const
	{
		return _hash == empty_hash<uint32_t>();
	}

	// NameIdentifier<UINT, STRING, false>::to_string
	template <class UINT, class STRING>
		inline void NameIdentifier<UINT, STRING, false>::to_string(
			ToStringBuffer & dest_buffer,
			const Type & type, const void * object )
	{
		// cast object to a name
		const NameIdentifier<UINT, STRING, false> & name =
			*static_cast< const NameIdentifier<UINT, STRING, false> * >( object );

		name.to_string( dest_buffer );
	}

	// NameIdentifier<UINT, STRING, false>::to_string
	template <class UINT, class STRING>
		void NameIdentifier<UINT, STRING, false>::to_string(
			ToStringBuffer & dest_buffer ) const
	{
		// NameIdentifier with no string. The hash will be stringized
		const UINT hash = uint_hash();

		char str[ 128 ];
		sprintf_s( str, 128, "id%x", hash );
		dest_buffer.append( str );
	}
}


namespace reflective_externals
{
	// reflection of reflective::NameIdentifier<UINT,STRING,false>
	template < typename UINT , typename STRING  >
		reflective::Class * init_type(
			reflective::NameIdentifier<UINT,STRING,false> * null_pointer_1,
			reflective::NameIdentifier<UINT,STRING,false> * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef reflective::NameIdentifier<UINT,STRING,false> ThisClass;

		static Class * result = nullptr;
		if( result != nullptr )
			return result;

		// template arguments
		const TemplateParameter template_parameters[] =
		{
			TemplateParameter( "UINT", safe_get_qualified_type<UINT>() ),
			TemplateParameter( "STRING", safe_get_qualified_type<STRING>() ),
			TemplateParameter( "PRESERVE_NAME", false ),
		};

		Class * class_object = new_class<ThisClass>( "reflective", "NameIdentifier", template_parameters );
		result = class_object;


		// properties
		const Property * properties[] =
		{
			new_property<ThisClass>( "uint_hash", &ThisClass::uint_hash ),
		};

		class_object->set_string_functions( NameIdentifier<UINT, STRING, false>::to_string, nullptr );

		// assign members
		class_object->assign_properties( properties );

		// assign and return type
		return class_object;
	}

} // namespace reflective_externals
