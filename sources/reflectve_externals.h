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


#ifdef _DEBUG
	#define REFLECTIVE_IS_DEBUG					(0==0)
#else
	#define REFLECTIVE_IS_DEBUG					(1==0)
#endif

#ifdef _TEST
	#define REFLECTIVE_IS_TEST					(0==0)
#else
	#define REFLECTIVE_IS_TEST					(1==0)
#endif

#define REFLECTIVE_ENABLE_ASSERT				(REFLECTIVE_IS_DEBUG || REFLECTIVE_IS_TEST)

// forwards
namespace reflective		{ class StringOutputStream; }

// REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES
#define REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES	1 /* (REFLECTIVE_IS_DEBUG || CMB_ENABLE_TESTING) /* If this is true, SymbolName will
	keep both the string and the hash of the name. Otherwise SymbolName will keep only the
	hash. Having the string is useful for debug purpose or to generate user interfaces. */

#define REFLECTIVE_NAME_CONSISTENCY_CHECKS		REFLECTIVE_IS_DEBUG

// REFLECTIVE_TRACK_TYPE_HIERARCHY
#define REFLECTIVE_TRACK_TYPE_HIERARCHY		(1 == 1) /* If true, the reflective::Class will
	define an iterator to enumerate:
		- all classes with no base class
		- given a class, all its derived classes.
	The cost of REFLECTIVE_TRACK_TYPE_HIERARCHY is a linked list node-head couple in every class.
	Usually classes (like other types) are picked from their enclosing namespace by their
	SymbolName, (see reflective::Namespace), but enumerating the derived classes may be useful
	to generate user interfaces. */

#define REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS	0
#define REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS	1 /*
	Class members are sorted by their visibility (first public members, then protected members and
	finally private members). Enum members are sorted by their values in non-decreasing order.
	reflective::Class and reflective::Enum objects may be constructed with the correct sorting,
	and REFLECTIVE_WARN_ON_NONSORTED_SYMBOLS can defined as true to detect wrong sorting.
	If REFLECTIVE_RECOVERY_NONSORTED_SYMBOLS is defined as true, the memebers are automatically sorted.
	Having members not sorted may lead to impredictable results. */

#define REFLECTIVE_CHECK_FOR_DUPLICATE_SYMBOLS	REFLECTIVE_IS_DEBUG

#define REFLECTIVE_DEBUG_LIFO_DESTRUCTION		REFLECTIVE_IS_DEBUG

#define REFLECTIVE_NO_CLEANUP_ON_EXIT			(0==1) /* if true, the allocation of
	reflective objects is irreversible, otherwise they are destroyed during
	the destruction of the global objects in reverse order. Having this option to
	TRUE reduces the memory consuption. */

#define REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE   (1==1) /* Determines if types can have multiple
	base types (or base classes). Setting this to TRUE has impacts on performance. */

#if REFLECTIVE_IS_DEBUG
	#define REFLECTIVE_UPDOWN_CASTER_DEFAULT_IMPL	UpDownCasterOffsetting_Diagnostic // see UpDownCasterImplementation
#else
	#define REFLECTIVE_UPDOWN_CASTER_DEFAULT_IMPL	UpDownCasterOffsetting // see UpDownCasterImplementation
#endif

namespace reflective
{
	/* DestructorCallback - pointer to a function delegated to destroy an object before it is deallocated. */
	typedef void (*DestructorCallback)( void * i_object );

	template <typename TYPE> 
		void default_destructor_callback( void * i_memory_block )
			{ static_cast<TYPE*>( i_memory_block )->~TYPE(); }
}

namespace reflective_externals
{	
	void assert_failure( const char * expression );

	void * mem_alloc( size_t i_alignment, size_t i_size );

	void mem_free( void * i_allocation );

	void * mem_lifo_alloc( size_t i_alignment, size_t i_size, reflective::DestructorCallback i_destroyer = nullptr );

	void mem_lifo_free( void * i_allocation );

	void on_implicit_type_instance( const char * name,
		bool has_default_constructor, bool has_copy_constructor, 
		bool has_assignment, bool has_destructor );

} // namespace reflective_externals







