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
	// new_parameter<TYPE>
	template <class TYPE>
		inline Parameter * new_parameter( SymbolName name )
	{
		return new_parameter( name, safe_get_type<TYPE>(), safe_get_type_qualification<TYPE>() );
	}

	// new_parameter<TYPE>
	inline Parameter * new_parameter( SymbolName name, const Type & type, 
		const TypeQualification & type_qualification )
	{
		Parameter * parameter = REFLECTIVE_LIFO_NEW( Parameter, name, type, type_qualification );

		return parameter;
	}

	// Parameter::constructor
	inline Parameter::Parameter( SymbolName name, const Type & type )
		: Symbol( name ), _qualified_type( type )
	{
		set_type_id( eTypeParameter );
	}

	// Parameter::constructor
	inline Parameter::Parameter( SymbolName name, const Type & type, const TypeQualification & type_qualification )
		: Symbol( name ), _qualified_type( type, type_qualification )
	{
		set_type_id( eTypeParameter );
	}

	inline Parameter::Parameter( SymbolName name, const QualifiedType & qualified_type )
		: Symbol( name ), _qualified_type( qualified_type )
	{
	}

	// Parameter::type
	inline const Type & Parameter::type() const
	{
		return *_qualified_type.front_type();
	}

	// Parameter::qualified_type
	inline const TypeQualification & Parameter::qualification() const
	{
		return _qualified_type.qualification();
	}

	// ParameterList::total_alignment
	inline size_t ParameterList::total_alignment() const
	{
		return _total_alignment;
	}

	// ParameterList::total_size
	inline size_t ParameterList::total_size() const
	{
		return _total_size;
	}

	// ParameterList::assign
	inline void ParameterList::assign( const Parameter * * items, size_t count )
	{
		SymbolList< Parameter, false >::assign( items, count );
		_compute_totals();
	}

	// ParameterList::assign
	template < size_t ITEM_COUNT >
		inline void ParameterList::assign( const Parameter * (&items_array)[ITEM_COUNT] )
	{
		SymbolList< Parameter, false >::assign( items_array );
		_compute_totals();
	}

	// ParameterList::Walker<POINTER>::constructor
	template <class POINTER>
		inline ParameterList::Walker<POINTER>::Walker(
			const ParameterList & parameter_list, POINTER buffer )
			: _parameter_list( parameter_list ), _curr_value( buffer ), _curr_parameter_index( 0 )
	{
		#if REFLECTIVE_IS_DEBUG

			// check alignment
			REFLECTIVE_ASSERT( mem::is_aligned( buffer, _parameter_list.total_alignment() ) );

		#endif
	}

	// ParameterList::Walker<POINTER>::parameter_list
	template <class POINTER>
		inline const ParameterList & ParameterList::Walker<POINTER>::parameter_list() const
	{
		return _parameter_list;
	}

	// ParameterList::Walker<POINTER>::parameter
	template <class POINTER>
		inline const Parameter & ParameterList::Walker<POINTER>::parameter() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return _parameter_list[ _curr_parameter_index ];
	}

	// ParameterList::Walker<POINTER>::value
	template <class POINTER>
		inline POINTER ParameterList::Walker<POINTER>::value() const
	{
		REFLECTIVE_ASSERT( is_not_over() );
		return _curr_value;
	}

	// ParameterList::Walker<POINTER>::is_not_over
	template <class POINTER>
		inline bool ParameterList::Walker<POINTER>::is_not_over() const
	{
		return _curr_parameter_index < _parameter_list.count();
	}

	// Walker<POINTER>::operator bool
	template <class POINTER>
		inline ParameterList::Walker<POINTER>::operator bool () const
	{
		return is_not_over();
	}

	// ParameterList::Walker<POINTER>::move_next
	template <class POINTER>
		inline bool ParameterList::Walker<POINTER>::move_next()
	{
		REFLECTIVE_ASSERT( is_not_over() );

		size_t address = (size_t)_curr_value;
		address += _parameter_list[ _curr_parameter_index ].type().size();

		_curr_parameter_index++;

		if( _curr_parameter_index >= _parameter_list.count() )
			return false;

		const size_t alignment_mask = _parameter_list[ _curr_parameter_index ].type().alignment() - 1;
		address += alignment_mask;
		address &= ~alignment_mask;
		_curr_value = reinterpret_cast<POINTER*>( address );
		return true;
	}

	// ParameterList::Walker<POINTER>::operator ++
	template <class POINTER>
		inline bool ParameterList::Walker<POINTER>::operator ++ ()
	{
		return move_next();
	}

} // namespace reflective
