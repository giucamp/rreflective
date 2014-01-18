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
	enum NoLifeFunctions { eNoLifeFunctions };
	enum ConstructorDestructor { eConstructorDestructor };
	enum ConstructorCopyAssignmentDestructor { eConstructorCopyAssignmentDestructor };
	enum CopyAssignmentDestructor { eCopyAssignmentDestructor };
	enum UngrabbedConstructorCopyAssignmentDestructor { eUngrabbedConstructorCopyAssignmentDestructor };

	// LifeFunctions
	class LifeFunctions
	{
	public:

		// from_type< TYPE, FUNCTIONS >
		template <class TYPE> static LifeFunctions from_type( 
			ConstructorCopyAssignmentDestructor functions = eConstructorCopyAssignmentDestructor );
		template <class TYPE> static LifeFunctions from_type( UngrabbedConstructorCopyAssignmentDestructor );
		template <class TYPE> static LifeFunctions from_type( ConstructorDestructor );		
		template <class TYPE> static LifeFunctions from_type( CopyAssignmentDestructor );
		template <class TYPE> static LifeFunctions from_type( NoLifeFunctions );

			/* returns a LifeFunctions with the functions for the  type TYPE. The specified functions 
				are subject to the following statements: 			
				- A copy constructor implies an assignemnt operator, and vice-versa.
				- Any constructor implies a destructor, and vice-versa.

				LifeFunctions::from_type<TYPE>( UngrabbedConstructorCopyAssignmentDestructor )
					can be used for types which proivides public default constructor, destructor, 
					copy constructor and assignment operator.

				LifeFunctions::from_type<TYPE>( eConstructorCopyAssignmentDestructor ) (or simply
					LifeFunctions::from_type<TYPE>() ) can be used for types which proivides public 
					default constructor, destructor, copy constructor and assignment operator.

				LifeFunctions::from_type<TYPE>( eConstructorDestructor ) can be used for types 
					which proivides public default constructor and destructor.

				LifeFunctions::from_type<TYPE>( eCopyAssignmentDestructor ) can be used for types 
					which proivides public copy constructor, assignment and destructor.	*/

		// typedefs
		typedef void (*CtorCaller)( void * start_address, void * end_address ); // constructor\destructor caller function pointer
		typedef void (*Mover)( void * dest_start, void * dest_end, void * source_start ); // mover function pointer
		typedef void (*Copier)( void * dest_start, void * dest_end, const void * source_start ); // mover function pointer

		// constructors
		LifeFunctions();
		LifeFunctions( CtorCaller constructor_caller, CtorCaller destructor_caller, 
			Copier copier, Mover mover );

		// properties
		bool is_abstract() const; // returns true if any of the functions is nullptr
		CtorCaller constructor_caller() const; // retrieves the function delegated to construct an array of objects
		CtorCaller destructor_caller() const; // retrieves the function delegated to construct an array of objects
		Mover mover() const; /* retrieves the function delegated to move an array of objects. The objects are destroyed 
			from the source and copy-constructed in the destination. The source and the destination can overlap. */
		Copier copier() const; /* retrieves the function delegated to copy an array of objects to a destination array. 
			The destination array must be an uninitialized buffer, and cannot overlap with the source array. */

		// methods
		void call_constructor( void * start_address, void * end_address ) const;
		void call_destructor( void * start_address, void * end_address ) const;		
		void move( void * dest_start, void * dest_end, void * source_start ) const;
		void copy( void * dest_start, void * dest_end, const void * source_start ) const;

	private: // internal services
		static void _binary_move( void * dest_start, void * dest_end, void * source_start );

	private: // data members
		CtorCaller _constructor_caller, _destructor_caller;		
		Copier _copier;
		Mover _mover;
	};

} // namespace reflective

