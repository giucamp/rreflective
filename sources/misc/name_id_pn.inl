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
	// NameIdentifier::empty
	template <class UINT, class STRING>
		NameIdentifier<UINT, STRING, true> NameIdentifier<UINT, STRING, true>::empty;

	// NameIdentifier::default constructor
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true>::NameIdentifier()
			: _string( "" ), _hash( empty_hash<UINT>() )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true>::NameIdentifier( const STRING & string )
			: _string( string ), _hash( hash<UINT>( string ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true>::NameIdentifier( const char * string )
			: _string( string ), _hash( hash<UINT>( string ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( STRING )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true>::NameIdentifier( const char * string, size_t length )
			: _string( string, length ), _hash( hash<UINT>( string, length ) )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier::constructor( NameIdentifier )
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true>::NameIdentifier( const NameIdentifier & string )
			: _string( string._string ), _hash( string._hash )
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
	}

	// NameIdentifier<UINT, STRING, true>::operator =
	template <class UINT, class STRING>
		inline NameIdentifier<UINT, STRING, true> & 
			NameIdentifier<UINT, STRING, true>::operator = ( const NameIdentifier & source )
	{
		_string = source._string;
		_hash = source._hash;
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check();
		#endif
		return *this;
	}

	// NameIdentifier<UINT, STRING, true>::operator ==
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator == ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );			
		#endif
		return _hash == op._hash;
	}

	// NameIdentifier<UINT, STRING, true>::operator !=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator != ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return _hash != op._hash;
	}

	// NameIdentifier<UINT, STRING, true>::operator <=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator <= ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return _hash <= op._hash;
	}

	// NameIdentifier<UINT, STRING, true>::operator <
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator < ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return _hash < op._hash;
	}

	// NameIdentifier<UINT, STRING, true>::operator >=
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator >= ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return _hash >= op._hash;
	}

	// NameIdentifier<UINT, STRING, true>::operator >
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::operator > ( const NameIdentifier & op ) const
	{
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			_dbg_check_pair( *this, op );
		#endif
		return _hash > op._hash;
	}

	// NameIdentifier<UINT, STRING, false>::is_empty
	template <class UINT, class STRING>
		inline bool NameIdentifier<UINT, STRING, true>::is_empty() const
	{
		return _hash == empty_hash<uint32_t>();
	}
		
	// NameIdentifier<UINT, STRING, true>::uint_hash
	template <class UINT, class STRING>
		inline typename NameIdentifier<UINT, STRING, true>::UIntHash 
			NameIdentifier<UINT, STRING, true>::uint_hash() const
	{
		return _hash;
	}

	// NameIdentifier<UINT, STRING, true>::uint_hash
	template <class UINT, class STRING>
		inline const STRING & NameIdentifier<UINT, STRING, true>::string() const
	{
		return _string;
	}

	// NameIdentifier<UINT, STRING, true>::to_string
	template <class UINT, class STRING>
		inline void NameIdentifier<UINT, STRING, true>::to_string( 
			StringOutputStream & dest_buffer, 
			const Type & type, const void * object )
	{
		REFLECTIVE_UNUSED( type );

		typedef NameIdentifier<UINT, STRING, true> ThisType;

		REFLECTIVE_ASSERT( type >= get_type< ThisType >() );

		const ThisType & name = *static_cast< const ThisType * >( object );

		name.to_string( dest_buffer );
	}

	// NameIdentifier<UINT, STRING, true>::to_string
	template <class UINT, class STRING>
		void NameIdentifier<UINT, STRING, true>::to_string( 
			StringOutputStream & dest_buffer ) const
	{
		const STRING & name_string = string();
		return safe_get_type<STRING>().to_string( dest_buffer, &name_string );
	}

	#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
		
		// NameIdentifier<UINT, STRING, true>::_dbg_check
		template <class UINT, class STRING>
			void NameIdentifier<UINT, STRING, true>::_dbg_check() const
		{
			const UINT hash_val = hash<UINT>( _string );
			REFLECTIVE_ASSERT( hash_val == _hash );
		}

		// NameIdentifier<UINT, STRING, true>::_dbg_check_pair
		template <class UINT, class STRING>
			void NameIdentifier<UINT, STRING, true>::_dbg_check_pair( const NameIdentifier & first, const NameIdentifier & second )
		{
			first._dbg_check();
			second._dbg_check();
			const bool string_equality = first._string == second._string;
			const bool hash_equality = first._hash == second._hash;
			REFLECTIVE_ASSERT( hash_equality == string_equality );	
		}

	#endif // #if REFLECTIVE_NAME_CONSISTENCY_CHECKS
}



namespace reflective_externals
{
	// reflection of reflective::NameIdentifier<UINT,STRING,true>
	template < typename UINT , typename STRING  >
		void init_type( reflective::Type * volatile * o_result,
			reflective::NameIdentifier<UINT,STRING,true> * null_pointer_1,
			reflective::NameIdentifier<UINT,STRING,true> * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );
		using namespace ::reflective;
		typedef reflective::NameIdentifier<UINT,STRING,true> ThisClass;

		if( *o_result != nullptr )
			return;

		void * allocation = reflective_externals::mem_lifo_alloc( 
			alignment_of( Class ), sizeof( Class ), &default_destroyer<Class> );

		// template arguments
		const TemplateParameter template_parameters[] = 
		{
			TemplateParameter( "UINT", safe_get_qualified_type<UINT>() ),
			TemplateParameter( "STRING", safe_get_qualified_type<STRING>() ),
			TemplateParameter( "PRESERVE_NAME", true ),
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

		class_object->set_string_functions( NameIdentifier<UINT, STRING, true>::to_string, nullptr );
		
		// assign members
		class_object->assign_properties( properties );
	}

} // namespace reflective_externals
