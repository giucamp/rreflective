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
						/**** LifeFunctions ****/

	// LifeFunctions::from_type
	template < class TYPE > inline LifeFunctions LifeFunctions::from_type( 
			ConstructorCopyAssignmentDestructor functions )
	{
		REFLECTIVE_ASSERT( functions == eConstructorCopyAssignmentDestructor );
		REFLECTIVE_UNUSED( functions );

		return LifeFunctions( 
			&objects_construct_void<TYPE>,
			&objects_destroy_void<TYPE>,
			&objects_copy_void<TYPE>,
			&grabbed_objects_move_void<TYPE> );
	}

	// LifeFunctions::from_type
	template < class TYPE > inline LifeFunctions LifeFunctions::from_type( 
			UngrabbedConstructorCopyAssignmentDestructor functions )
	{
		/* this forces grabbed_objects_move_void to be instantiated for TYPE. The linker
			will strip this function, but if the assignment operator is not accessible
			the compiler will report an error. */
		//template <>	void grabbed_objects_move_void<TYPE>( void * dest_start, void * dest_end, void * source_start );

		REFLECTIVE_ASSERT( functions == eUngrabbedConstructorCopyAssignmentDestructor );
		REFLECTIVE_UNUSED( functions );

		return LifeFunctions( 
			&objects_construct_void<TYPE>,
			&objects_destroy_void<TYPE>,
			&objects_copy_void<TYPE>,
			&_binary_move );
	}

	// LifeFunctions::from_type
	template < class TYPE > inline LifeFunctions LifeFunctions::from_type( 
		ConstructorDestructor functions )
	{		
		REFLECTIVE_ASSERT( functions == eConstructorDestructor );
		REFLECTIVE_UNUSED( functions );
		return LifeFunctions( 
			&objects_construct_void<TYPE>,
			&objects_destroy_void<TYPE>,
			nullptr,
			nullptr );
	}

	// LifeFunctions::from_type
	template < class TYPE > inline LifeFunctions LifeFunctions::from_type( 
		CopyAssignmentDestructor functions )
	{
		REFLECTIVE_ASSERT( functions == eCopyAssignmentDestructor );
		REFLECTIVE_UNUSED( functions );
		return LifeFunctions( 
			nullptr,
			&objects_destroy_void<TYPE>,
			&objects_copy_void<TYPE>,
			&objects_move_void<TYPE> );
	}

	// LifeFunctions::from_type
	template < class TYPE > inline LifeFunctions LifeFunctions::from_type( 
		NoLifeFunctions functions )
	{
		REFLECTIVE_UNUSED( functions );
		REFLECTIVE_ASSERT( functions == eNoLifeFunctions );
		return LifeFunctions();
	}

	// LifeFunctions::is_abstract
	inline bool LifeFunctions::is_abstract() const
	{
		REFLECTIVE_ASSERT( (!_constructor_caller) == (!_destructor_caller) &&
			(!_destructor_caller) == (!_mover) &&
			(!_mover) == (!_copier) );
		return _constructor_caller == nullptr;
	}

	// LifeFunctions::default_constructor
	inline LifeFunctions::LifeFunctions()
		: _constructor_caller( nullptr ), _destructor_caller( nullptr ),
		  _mover( nullptr ), _copier( nullptr )
	{
		// the functions
		REFLECTIVE_ASSERT( (!_constructor_caller) == (!_destructor_caller) &&
			(!_destructor_caller) == (!_mover) &&
			(!_mover) == (!_copier) );
	}

		// LifeFunctions::constructor
	inline LifeFunctions::LifeFunctions( CtorCaller constructor_caller, CtorCaller destructor_caller,
		Copier copier, Mover mover )
		: _constructor_caller( constructor_caller ), _destructor_caller( destructor_caller ),
		  _copier( copier ), _mover( mover )
	{
		#if REFLECTIVE_ENABLE_ASSERT
			if( _constructor_caller )
			{
				REFLECTIVE_ASSERT( _destructor_caller != nullptr );
				if( _mover != nullptr )
					REFLECTIVE_ASSERT( _copier != nullptr );
				else
					REFLECTIVE_ASSERT( !_copier );
			}
			else if( !_destructor_caller )
			{
				REFLECTIVE_ASSERT( !_destructor_caller );
				REFLECTIVE_ASSERT( !_mover );
				REFLECTIVE_ASSERT( !_copier );
			}
		#endif // #if REFLECTIVE_ENABLE_ASSERT
	}

	// LifeFunctions::constructor_caller
	inline LifeFunctions::CtorCaller LifeFunctions::constructor_caller() const
		{ return _constructor_caller; }

	// LifeFunctions::destructor_caller
	inline LifeFunctions::CtorCaller LifeFunctions::destructor_caller() const
		{ return _destructor_caller; }

	// LifeFunctions::mover
	inline LifeFunctions::Mover LifeFunctions::mover() const
		{ return _mover; }

	// LifeFunctions::copier
	inline LifeFunctions::Copier LifeFunctions::copier() const
		{ return _copier; }

	// LifeFunctions::call_constructor
	inline void LifeFunctions::call_constructor( void * start_address, void * end_address ) const
	{
		(*_constructor_caller)( start_address, end_address );
	}

	// LifeFunctions::call_destructor
	inline void LifeFunctions::call_destructor( void * start_address, void * end_address ) const
	{
		(*_destructor_caller)( start_address, end_address );
	}

	// LifeFunctions::move_objects
	inline void LifeFunctions::move( void * dest_start, void * dest_end, void * source_start ) const
	{
		(*_mover)( dest_start, dest_end, source_start );
	}

	// LifeFunctions::copy
	inline void LifeFunctions::copy( void * dest_start, void * dest_end, const void * source_start ) const
	{
		(*_copier)( dest_start, dest_end, source_start );
	}


} // namespace reflective
