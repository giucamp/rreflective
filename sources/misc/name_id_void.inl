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
	template <class UINT>
		inline NameIdentifier<UINT, void>::NameIdentifier()
			: m_hash( empty_hash<UINT>() )
	{

	}

	// NameIdentifier::s_empty
	template <class UINT>
		const NameIdentifier<UINT, void> NameIdentifier<UINT, void>::s_empty;

	// NameIdentifier::constructor( const char * )
	template <class UINT>
		template <class STRING>
			inline NameIdentifier<UINT, void>::NameIdentifier( const STRING & string )
				: m_hash( hash<UINT>( string ) )
	{

	}

	// NameIdentifier::constructor( const char * )
	template <class UINT>
		inline NameIdentifier<UINT, void>::NameIdentifier( const char * string )
			: m_hash( hash<UINT>( string ) )
	{

	}

	// NameIdentifier::constructor( const char * )
	template <class UINT>
		inline NameIdentifier<UINT, void>::NameIdentifier( const char * string, size_t length )
			: m_hash( hash<UINT>( string, length ) )
	{

	}

	// NameIdentifier::constructor( NameIdentifier )
	template <class UINT>
		inline NameIdentifier<UINT, void>::NameIdentifier( const NameIdentifier & name )
			: m_hash( name.m_hash )
	{

	}


	template <class UINT>
		inline NameIdentifier<UINT, void> NameIdentifier<UINT, void>::from_hash( UINT i_hash )
	{
		NameIdentifier res;
		res.m_hash = i_hash;
		return res;
	}

	// NameIdentifier<true, UINT>::operator =
	template <class UINT>
		inline NameIdentifier<UINT, void> &
			NameIdentifier<UINT, void>::operator = ( const NameIdentifier & source )
	{
		m_hash = source.m_hash;
		return *this;
	}

	// NameIdentifier<UINT, void>::uint_hash
	template <class UINT>
		inline typename NameIdentifier<UINT, void>::UIntHash
			NameIdentifier<UINT, void>::uint_hash() const
	{
		return m_hash;
	}

	// NameIdentifier<UINT, void>::operator ==
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator == ( const NameIdentifier & op ) const
	{
		return m_hash == op.m_hash;
	}

	// NameIdentifier<UINT, void>::operator !=
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator != ( const NameIdentifier & op ) const
	{
		return m_hash != op.m_hash;
	}

	// NameIdentifier<UINT, void>::operator >=
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator >= ( const NameIdentifier & op ) const
	{
		return m_hash >= op.m_hash;
	}

	// NameIdentifier<UINT, void>::operator >
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator > ( const NameIdentifier & op ) const
	{
		return m_hash > op.m_hash;
	}

	// NameIdentifier<UINT, void>::operator <=
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator <= ( const NameIdentifier & op ) const
	{
		return m_hash <= op.m_hash;
	}

	// NameIdentifier<UINT, void>::operator <
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::operator < ( const NameIdentifier & op ) const
	{
		return m_hash < op.m_hash;
	}

	// NameIdentifier<UINT, void>::is_empty
	template <class UINT>
		inline bool NameIdentifier<UINT, void>::is_empty() const
	{
		return m_hash == empty_hash<uint32_t>();
	}

	// NameIdentifier<UINT, void>::to_string
	template <class UINT>
		inline void NameIdentifier<UINT, void>::to_string(
			StringOutputStream & dest_buffer,
			const Type & type, const void * object )
	{
		// cast object to a name
		const NameIdentifier<UINT, void> & name =
			*static_cast< const NameIdentifier<UINT, void> * >( object );

		name.to_string( dest_buffer );
	}

	// NameIdentifier<UINT, void>::to_string
	template <class UINT>
		void NameIdentifier<UINT, void>::to_string(
			StringOutputStream & dest_buffer ) const
	{
		// NameIdentifier with no string. The hash will be stringized
		const UINT hash = uint_hash();

		char str[ 128 ];
		sprintf_s( str, 128, "id%x", hash );
		dest_buffer.append( str );
	}

	// NameIdentifier<UINT, STRING>::from_string
	template <class UINT>
		bool NameIdentifier<UINT, void>::from_string( FromStringBuffer & i_source_buffer,
			const reflective::Type & i_type, void * i_object, StringOutputStream & o_error_buffer )
	{
		REFLECTIVE_UNUSED( i_type );

		typedef NameIdentifier<UINT, void> ThisType;

		REFLECTIVE_ASSERT( i_type >= get_type< ThisType >() );

		ThisType & name = *static_cast< ThisType * >( i_object );

		const char * chars = i_source_buffer.chars();
		if( i_source_buffer.accept( "0x", FromStringBuffer::eIgnoreLeadingSpaces | FromStringBuffer::eIgnoreLeadingSpaces ) )
		{
			bool result = parse_hash( i_source_buffer, o_error_buffer, &name.m_hash );
			return result;			
		}
		else
		{
			name.m_hash = hash<UINT>( i_source_buffer.chars(), i_source_buffer.remaining_length() );
			return true;
		}		
	}
}


namespace reflective_externals
{
	// reflection of reflective::NameIdentifier<UINT,void>
	template < typename UINT >
		void init_type( reflective::Type * volatile * o_result,
			reflective::NameIdentifier<UINT,void> * null_pointer_1,
			reflective::NameIdentifier<UINT,void> * null_pointer_2 )
	{
		using namespace ::reflective;
		typedef reflective::NameIdentifier<UINT,void> ThisClass;
		
		if( *o_result != nullptr )
			return;

		// template arguments
		const TemplateParameter template_parameters[] =
		{
			TemplateParameter( "UINT", safe_get_qualified_type<UINT>() ),
			TemplateParameter( "STRING", safe_get_qualified_type<void>() ),
		};

		Class * class_object = new_class<ThisClass>( "reflective", "NameIdentifier", template_parameters );
		
		*o_result = class_object;
	
		REFLECTIVE_ASSERT( *o_result == class_object );

		class_object->set_no_base_type();

		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorCopyAssignmentDestructor ) );

		// properties
		const Property * properties[] =
		{
			new_property<ThisClass>( "uint_hash", &ThisClass::uint_hash ),
		};

		class_object->set_string_functions( NameIdentifier<UINT, void>::to_string, NameIdentifier<UINT, void>::from_string );

		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals
