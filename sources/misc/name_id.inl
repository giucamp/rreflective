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
	// NameIdentifier::s_empty
	template <class UINT, class STRING>
		const NameIdentifier<UINT, STRING> NameIdentifier<UINT, STRING>::s_empty;

	template <class UINT, class STRING>
		const STRING NameIdentifier<UINT, STRING>::s_unknown_string("???");

	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING> NameIdentifier<UINT, STRING>::from_hash( UINT i_hash )
	{
		return NameIdentifier( eConstructFromHash, s_unknown_string );
	}

	template <class UINT> bool parse_hash( FromStringBuffer & i_source_buffer, StringOutputStream & o_error_buffer, UINT * o_hash )
	{
		bool result = true;

		UINT hash = 0;
		for( char curr_char = i_source_buffer.consume_char(); curr_char != 0; curr_char = i_source_buffer.consume_char() )
		{
			if( curr_char >= '0' && curr_char <= '9' )
			{
				hash += curr_char - '0';
			}
			else if( curr_char >= 'A' && curr_char <= 'F' )
			{
				hash += curr_char - 'A' + 10;
			}
			else
			{
				if( !isspace(curr_char) )
				{
					o_error_buffer.append_literal( "unexpected char" );
					result = false;
				}
				break;
			}
			const UINT next_val = hash << 4;
			if( next_val >> 4 != hash )
			{
				o_error_buffer.append_literal( "overflow" );
				result = false;
			}
			hash = next_val;
		}

		if( result )
		{
			*o_hash = hash;
		}		
		return result;
	}


	template <class UINT> void hash_to_string( UINT i_hash, StringOutputStream & i_dest_buffer )
	{
		REFLECTIVE_COMPILETIME_ASSERT( !std::numeric_limits<UINT>::is_signed );
		REFLECTIVE_COMPILETIME_ASSERT( std::numeric_limits<UINT>::radix == 2 );
		REFLECTIVE_COMPILETIME_ASSERT( std::numeric_limits<UINT>::digits % 4 == 0 );
		REFLECTIVE_COMPILETIME_ASSERT( std::numeric_limits<UINT>::digits >= 4 );
		
		uint32_t shift = std::numeric_limits<UINT>::digits;
		do
		{
			shift -= 4;

			char dig = (i_hash >> shift) & 15;
			if( dig < 10 )
				dig += '0';
			else
				dig += 'A';
			i_dest_buffer.append(dig);

		} while( shift > 0 );
	}

	// NameIdentifier::default constructor
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING>::NameIdentifier()
			: m_hash( empty_hash<UINT>() )
	{
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING>::NameIdentifier( const STRING & string )
			: m_string( string ), m_hash( hash<UINT>( string ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING>::NameIdentifier( const char * string )
			: m_string( string ), m_hash( hash<UINT>( string ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING>::NameIdentifier( const char * string, size_t length )
			: m_string( string, length ), m_hash( hash<UINT>( string, length ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( NameIdentifier )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING>::NameIdentifier( const NameIdentifier & string )
			: m_string( string.m_string ), m_hash( string.m_hash )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier<UINT, STRING>::operator =
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING> & 
			NameIdentifier<UINT, STRING>::operator = ( const NameIdentifier & source )
	{
		m_string = source.m_string;
		m_hash = source.m_hash;
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
		return *this;
	}

	// NameIdentifier<UINT, STRING>::operator ==
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator == ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );			
		#endif
		return m_hash == op.m_hash;
	}

	// NameIdentifier<UINT, STRING>::operator !=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator != ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return m_hash != op.m_hash;
	}

	// NameIdentifier<UINT, STRING>::operator <=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator <= ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return m_hash <= op.m_hash;
	}

	// NameIdentifier<UINT, STRING>::operator <
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator < ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return m_hash < op.m_hash;
	}

	// NameIdentifier<UINT, STRING>::operator >=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator >= ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return m_hash >= op.m_hash;
	}

	// NameIdentifier<UINT, STRING>::operator >
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::operator > ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return m_hash > op.m_hash;
	}

	// NameIdentifier<UINT, STRING, false>::is_empty
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING>::is_empty() const
	{
		return m_hash == empty_hash<uint32_t>();
	}
		
	// NameIdentifier<UINT, STRING>::uint_hash
	template <class UINT, class STRING>
		inline typename NameIdentifier<UINT, STRING>::UIntHash 
			NameIdentifier<UINT, STRING>::uint_hash() const
	{
		return m_hash;
	}

	// NameIdentifier<UINT, STRING>::uint_hash
	template <class UINT, class STRING>
		inline const STRING & NameIdentifier<UINT, STRING>::string() const
	{
		return m_string;
	}

	// NameIdentifier<UINT, STRING>::to_string
	template <class UINT, class STRING>
		void NameIdentifier<UINT, STRING>::to_string( 
			StringOutputStream & dest_buffer, 
			const Type & type, const void * object )
	{
		REFLECTIVE_UNUSED( type );

		typedef NameIdentifier<UINT, STRING> ThisType;

		REFLECTIVE_ASSERT( type.can_cast_to( get_type< ThisType >() ) );

		const ThisType & name = *static_cast< const ThisType * >( object );

		name.to_string( dest_buffer );
	}

	// NameIdentifier<UINT, STRING>::from_string
	template <class UINT, class STRING>
		bool NameIdentifier<UINT, STRING>::from_string( FromStringBuffer & i_source_buffer,
			const reflective::Type & i_type, void * i_object, StringOutputStream & o_error_buffer )
	{
		REFLECTIVE_UNUSED( i_type );

		typedef NameIdentifier<UINT, STRING> ThisType;

		REFLECTIVE_ASSERT( i_type.can_cast_to( get_type< ThisType >() ) );

		ThisType & name = *static_cast< ThisType * >( i_object );

		if( i_source_buffer.accept( "0x", FromStringBuffer::eIgnoreLeadingSpaces | FromStringBuffer::eIgnoreLeadingSpaces ) )
		{
			bool result = parse_hash( i_source_buffer, o_error_buffer, &name.m_hash );
			if( result )
			{
				name.m_string = s_unknown_string;
			}
			return result;			
		}
		else
		{
			STRING & name_string = name.m_string;
			const bool result = safe_get_type<STRING>().assign_from_string( i_source_buffer, &name_string, o_error_buffer );
			if( result )
			{
				name.m_hash = hash<UINT>( name.m_string );
			}
			return result;
		}		
	}

	// NameIdentifier<UINT, STRING>::to_string
	template <class UINT, class STRING>
		void NameIdentifier<UINT, STRING>::to_string( 
			StringOutputStream & dest_buffer ) const
	{
		const STRING & name_string = string();
		if( name_string == s_unknown_string )
		{
			dest_buffer.append_literal( "0x" );
			hash_to_string( m_hash, dest_buffer );
		}
		else
		{
			return safe_get_type<STRING>().to_string( dest_buffer, &name_string );
		}
	}

	#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
		
		// NameIdentifier<UINT, STRING>::_dbg_check
		template <class UINT, class STRING>
			void NameIdentifier<UINT, STRING>::_dbg_check() const
		{
			if( !(m_string == s_unknown_string) )
			{
				const UINT hash_val = hash<UINT>( m_string );
				REFLECTIVE_ASSERT( hash_val == m_hash );
			}
		}

		// NameIdentifier<UINT, STRING>::_dbg_check_pair
		template <class UINT, class STRING>
			void NameIdentifier<UINT, STRING>::_dbg_check_pair( const NameIdentifier & first, const NameIdentifier & second )
		{
			const bool first_string_known = !( first.m_string == s_unknown_string );
			const bool second_string_known = !( second.m_string == s_unknown_string );

			if( first_string_known )
			{
				const UINT hash_val = hash<UINT>( first.m_string );
				REFLECTIVE_ASSERT( hash_val == first.m_hash );
			}

			if( second_string_known )
			{
				const UINT hash_val = hash<UINT>( second.m_string );
				REFLECTIVE_ASSERT( hash_val == second.m_hash );
			}
			
			if( first_string_known && second_string_known )
			{
				const bool string_equality = first.m_string == second.m_string;
				const bool hash_equality = first.m_hash == second.m_hash;
				REFLECTIVE_ASSERT( hash_equality == string_equality );	
			}
		}

	#endif // #if REFLECTIVE_NAME_CONSISTENCY_CHECKS
}



namespace reflective_externals
{
	// reflection of reflective::NameIdentifier<UINT,STRING>
	template < typename UINT , typename STRING  >
		void init_type( reflective::Type * volatile * o_result,
			reflective::NameIdentifier<UINT,STRING> * null_pointer_1,
			reflective::NameIdentifier<UINT,STRING> * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );
		using namespace ::reflective;
		typedef reflective::NameIdentifier<UINT,STRING> ThisClass;

		if( *o_result != nullptr )
			return;

		void * allocation = reflective_externals::mem_lifo_alloc( 
			alignment_of( Class ), sizeof( Class ), &default_destroyer<Class> );

		// template arguments
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "UINT", safe_get_qualified_type<UINT>() ),
			TemplateParameter( "STRING", safe_get_qualified_type<STRING>() ),
		};	

		// here new_class() is not used, beacuse it would cause recursion

		const size_t template_parameter_count = sizeof( template_parameters ) / sizeof( template_parameters[0] );

		ClassTemplate * class_template = create_class_template( "reflective", "NameIdentifier", 
			template_parameters, template_parameter_count );

		const SymbolName decorated_name = decorate_class_instance_name( "NameIdentifier", template_parameters, template_parameter_count );

		Class * class_object = new( allocation ) Class( class_template, decorated_name,
			alignment_of( ThisClass ), sizeof( ThisClass ) );

		*o_result = class_object;

		REFLECTIVE_ASSERT( *o_result == class_object );

		class_object->set_no_base_type();

		class_object->set_life_functions( LifeFunctions::from_type<ThisClass>(
			eConstructorCopyAssignmentDestructor ) );

		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "string", &ThisClass::string ),
			new_property<ThisClass>( "uint_hash", &ThisClass::uint_hash ),
		};

		class_object->set_string_functions( NameIdentifier<UINT, STRING>::to_string, NameIdentifier<UINT, STRING>::from_string );
		
		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals
