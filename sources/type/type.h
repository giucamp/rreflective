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
	// comparers
	typedef bool (*EqualsComparer)( const void * first_operand, const void * second_operand );
	typedef bool (*LessThanComparer)( const void * first_operand, const void * second_operand );

	// type resolver
	typedef const Type & (*TypeResolver)( const Type & base_type, const void * object );

	// BaseType
	struct BaseType
	{
		BaseType( const Type & base_type, const UpDownCaster<> & updown_caster );
		template <class THIS_CLASS, class BASE_CLASS> static BaseType from_types();
		template <class THIS_CLASS, class BASE_CLASS> static BaseType from_types_only_to_base();

		const Type & base_type;
		const UpDownCaster<> updown_caster;

	private:
		BaseType & operator = ( const BaseType & ); // not supported
	};

	/* separate_symbol_name - extract the namespace path and the name from a fullname
		"nmspc1::nmspc1::MyClass<int>" -> "nmspc1::nmspc1" an "MyClass<int>". */
	void separate_symbol_name( const char * full_name,
		StaticConstString * out_namespace_path, SymbolName * out_symbol_name, bool remove_template_parameter );
	void separate_symbol_name( const char * full_name,
		StaticConstString * out_namespace_path, StaticConstString * out_symbol_name, bool remove_template_parameter );

	// Type
	class Type : public Symbol
	{	
	public:

		// parent_namespace - the namespace that contains this type
		Namespace & edit_parent_namespace();
		const Namespace & parent_namespace() const;		

		/* Capabilities - enum of flags used to indicate which services are available with 
			this type. The bitwise 'or' and and operators (|, &) are defined for this enum
			to allow mixing them. */
		enum Capabilities
		{
			eCapabilitiesNone		= 0,
			eHasDefaultConstructor	= 1 << 0,
			eHasCopier				= 1 << 1,
			eHasMover				= 1 << 2,
			eHasDestructor			= 1 << 3,
			eHasEqualityComparer	= 1 << 4,
			eHasLessThanComparer	= 1 << 5,
			eHasToStringDumper		= 1 << 6,
			eHasFromStringAssigner	= 1 << 7,
			eHasTypeResolver		= 1 << 8,
			eHasCollectionHandler	= 1 << 9
		};

		// capabilities
		Capabilities capabilities() const;
		bool check_capabilities( Capabilities capabilities_to_check ) const; /* returns
			false if any of the specified capabilities is not supported. */

		/* size and alignment, as returned by sizeof() and alignment_of(). 
			They are subject to 4 constraints:
			- The size and the alignment are always greater than zero
			- The size is always greater or equal to the alignment
			- The alignment is always an integer power of 2
			- The size itself must be aligned according to the alignment
			When the memory for an object is allocated, the alignment 
			must be respected, otherwise the side effects are unpredictable. */
		size_t size() const;
		size_t alignment() const;
		bool is_aligned( const void * address ) const; /* returns true if the parameter is
			aligned according to the value returned by alignment(). If the address is nullptr 
			or zero, the return is always true. */

		// upper_align \ lower_align
		template <class ADDRESS> ADDRESS upper_align( ADDRESS address ) const; /* returns
			the lowest aligned address greater than or equal to the parameter. ADDRESS can
			be a pointer or an size_t. If the address is nullptr\zero the return value is  
			nullptr\zero too. */
		template <class ADDRESS> ADDRESS lower_align( ADDRESS address ) const; /* returns
			the highest aligned address less than or equal to the parameter. ADDRESS can
			be a pointer or an size_t. If the address is nullptr\zero the return value is 
			nullptr\zero too.*/

		// construct
		void construct( void * object ) const;
		void construct( void * start_address, void * end_address ) const; /* calls the
			default constructor on an object or an array of contiguous objects. The
			length of the array must be multiple of the size of the Type. Before
			calling this function the user must check that the type supports it by
			calling check_capabilities( eHasDefaultConstructor ). start_address is 
			included in the memory range, whereas end_address is excluded. If the length 
			of the memory between start_address and end_address is not multiple of the 
			size  of the type, or if the type has no constructor and construct() is 
			called, or if start_address is not correctly aligned, a debug-only error 
			is reported and a memory corruption or fault may occur.
			For primitive types, construct initializes the objects with zeros.
			The object are constructed starting from the first and incrementing the 
			pointer by the size of the type untill end_address is reached. 
			Implementation note: construct() just redirects the call to the 
			LifeFunctions object of the type (see life_functions() ). */

		// copy_construct
		void copy_construct( void * object, const void * source_object ) const; /* calls the
			copy constructor on an object. Before calling this function the user must check 
			that the type supports it by calling check_capabilities( eHasCopier ). If the type 
			has no copy constructor and copy_construct() is called, or is object has not the
			required alignment, a debug-only error is reported and a memory corruption or fault 
			may occur. The two pointers must be distinct.
			Implementation note: copy_construct() just redirects the call to the 
			LifeFunctions object of the type (see life_functions() ). */

		// multiple_copy_construct
		void multiple_copy_construct( void * start_address, void * end_address, const void * source_object ) const; 
			/* call copy_construct for each object between start_address and end_address, using the
				object pointed by source_object as source. */

		// destroy
		void destroy( void * object ) const;		
		void destroy( void * start_address, void * end_address ) const;	/* calls the
			destructor on an object or an array of contiguous objects. The size of 
			the destination buffer must be multiple of the size of the Type. Before 
			calling this function the user must check that the type supports it by calling 
			check_capabilities( eHasDestructor ). start_address is included in the 
			memory range, whereas end_address is excluded. If the length of the memory
			between start_address and end_address is not multiple of the size of the 
			type, or if the type has no destructor and destruct() is called, a debug-only 
			error is be reported and a memory corruption or fault may occur.
			In debug mode (see REFLECTIVE_IS_DEBUG macro), after the destruction the memory is
			filled with random values.
			The object are destroyed starting from the last and decrementing the 
			pointer by the size of the type untill start_address is reached. 
			Implementation note: destroy() just redirects the call to the 
			LifeFunctions object of the type (see life_functions() ). */

		// copy
		void copy( void * dest_start, void * dest_end, const void * source_start ) const;
			/* copies an array of objects to a destination location. The source an destination 
				arrays cannot overlap. Before calling this function the user must check that 
				the type support it by calling check_capabilities( eHasCopier ). The length of 
				the destination array must be multiple of the size of the Type. If the length 
				of the memory between dest_start and dest_end is not multiple of the size of 
				the type, or if the type has no copier and copy() is called, or if the source
				and the destination array overlap, a debug-only error may be reported and a 
				memory corruption or fault may occur.
				Udts usually uses the copy constructor to implement the copy, whereas primitive 
				types uses a memcpy. 
				Implementation note: copy() just redirects the call to the LifeFunctions object 
				of the type (see life_functions() ).  */

		// move
		void move( void * dest_start, void * dest_end, void * source_start ) const;
			/* moves an array of objects to a destination location. First the objects are 
				copied into the destination, and then the source objects are destroyed.
				The case of overlapping arrays is correctly handled. Before calling this 
				function the user must check that the type support it by calling 
				check_capabilities( eHasMover ). The length of the destination array must 
				be multiple of the size of the Type. If the length of the memory between
				dest_start and dest_end is not multiple of the size of the type, or if the
				type has no copier and copy() is called, a debug-only error may be reported
				and a memory corruption or fault may occur.				
				Udts usually uses the copy constructor and the destructor to move, whereas
				primitive types uses a memmove. In debug mode, the memory of destroyed objects 
				is randomly filled. 
				Implementation note: move() just redirects the call to the LifeFunctions object 
				of the type (see life_functions() ). */

		void move( void * dest_start, void * source_start ) const
			{ move( dest_start, mem::address_add( dest_start, _size  ), source_start ); }

		// life_functions		
		const LifeFunctions & life_functions() const;

		void * create_instance() const;

		void destroy_instance( void * object ) const;

		// to_string - gets a string representation of an object of this type. 			
		void to_string( ToStringBuffer & dest_buffer, const void * object ) const;
			/* before calling to_string, an array of chars must be assigned to
				dest_buffer. If the buffer is not big enough, ToStringBuffer reports
				the needed size (see ToStringBuffer). 
				If to_string() is called on an object which has not the eHasToStringDumper
				capability, a memory fault will occur. Use check_capabilities to check if
				it is supported by this type. */

		size_t compute_to_string_required_length( const void * object ) const;

		size_t to_string( char * dest_buffer, size_t dest_buffer_size, const void * object ) const;

		// type_full_name_to_string
		void type_full_name_to_string( ToStringBuffer & dest_buffer ) const;

		// assign_from_string
		bool assign_from_string( FromStringBuffer & source_buffer, void * object, ToStringBuffer & error_buffer ) const; 
		
		// comparers
		bool object_equals( const void * first_operand, const void * second_operand ) const;
		bool object_less_than( const void * first_operand, const void * second_operand ) const;
		EqualsComparer equality_comparer() const;
		LessThanComparer less_than_comparer() const;


							/* Types Hierarchy */

		/* try_cast_pointer_to - tries to cast a pointer from a type to another type. The cast
			is possible only if dest_type is an indirect base class of this type (or if they are
			the same type). Otherwise nullptr is returned. If the parameter object is nullptr, the 
			result is nullptr. */
		void * try_cast_pointer_to( const Type & dest_type, void * object ) const;
		const void * try_cast_pointer_to( const Type & dest_type, const void * object ) const;

		// query for relation
		bool can_cast_to( const Type & dest_type ) const;
		bool is_related_to( const Type & dest_type ) const;
		bool has_base_type_with_name( SymbolName name ) const;

		// comparison - A <= B if and only if B is A or can be implicitly casted to A
		bool operator <= ( const Type & op ) const			{ return op.can_cast_to( *this ); }
		bool operator >= ( const Type & op ) const			{ return can_cast_to( op ); }

		// base types
		unsigned base_types_count() const;
		const Type * first_base_type() const;
		const Type * base_type( unsigned base_type_index ) const;
		const UpDownCaster<> & updown_caster( unsigned base_type_index ) const;

		// pointer_to_base_type \ pointer_from_base_type
		void * pointer_to_base_type( void * pointer, unsigned base_type_index ) const;
		void * pointer_from_base_type( void * pointer, unsigned base_type_index ) const;
		const void * pointer_to_base_type( const void * pointer, unsigned base_type_index ) const;
		const void * pointer_from_base_type( const void * pointer, unsigned base_type_index ) const;
			/* pointer_to_base_type casts a pointer to an object of this type to a pointer 
				to an object of the base type. The pointer cannot be nullptr, and base_type_index must
				be less than base_types_count(). */

		/* type resolution - a type may be able to retrieve the complete derived
			type of an object. Before calling this function the user must check 
			that the type supports it by calling check_capabilities( eHasTypeResolver ).*/
		TypeResolver type_resolver() const;
		const Type & resolve_type( const void * object ) const;

		// HierarchyIterator
		#if REFLECTIVE_TRACK_TYPE_HIERARCHY
			
			class HierarchyIterator
			{
			public:
				HierarchyIterator(); // iterates root classes
				HierarchyIterator( const Type & base_class ); // iterates classes deriving from base_class

				operator bool () const; // test for end of iteration

				const Type & operator * () const; // access by indirection
				const Type * operator -> () const; // access to members

				void operator ++ ( int ); // prefix increment - next class
				HierarchyIterator & operator ++ (); // postfix increment - next class

			private: // data members
				const Type * _current;
			};

		#endif // #if REFLECTIVE_TRACK_TYPE_HIERARCHY



					/* collection handler */

		// collection interface		
		const CollectionHandler * collection_handler() const;

		// indicize
		void * indicize( void * start_address, size_t index ) const;
		const void * indicize( const void * start_address, size_t index ) const; /* provides
			an easy way to access a ([const] void *) buffer as an array of objects of this
			type. */

	public: // editing		

		void set_life_functions( const LifeFunctions & new_life_functors );

		void set_comparers( EqualsComparer equality_comparer, LessThanComparer less_than_comparer );

		template <class TYPE> void set_default_equality_comparers();

		// set_string_functions with global functions
		void set_string_functions( 
			GlobalFunctionsStringizer::ToStringDumper to_string,
			GlobalFunctionsStringizer::FromStringAssigner from_string = 0 );

		// set_string_functions with methods
		template <class TYPE> void set_string_functions( 
			void (TYPE::*to_string)( ToStringBuffer & dest_buffer ) const );
		template <class TYPE> void set_string_functions( 
			void (TYPE::*to_string)( ToStringBuffer & dest_buffer ) const,
			bool (TYPE::*from_string)( FromStringBuffer & source_buffer,
				ToStringBuffer & error_buffer ) );

		void set_collection_handler( const CollectionHandler * collection_handler );

		// set_base_type - set_base_types
		void set_no_base_type();
		void set_base_type( BaseType base_type );
		void set_base_type( const Type & base_type, const UpDownCaster<> & updown_caster ); 
		template <unsigned COUNT> void set_base_types( const BaseType (&base_types)[ COUNT ] );
		/* the base types can be set only once. The updown_caster is an object which can 
			cast a void pointer from this type to the base type and back. */

		// set_type_resolver
		void set_type_resolver( TypeResolver type_resolver );

	protected:

		// constructors		
		Type( Namespace & parent_namespace, const SymbolName & name, size_t size, size_t alignment );
		Type( const StaticConstString & parent_namespace_path, const SymbolName & name, size_t size, size_t alignment );
			/* 'namespace_path' is a SymbolNam that specifies the scope the type is declared within. 
				The parts of the scope are separated by a double colon ("::"). For example "outer::inner" 
				tells that the class is inside the namespace inner, the latter nested inside the namespace outer.
				The Namespace objects are created the first time they are encountered (see 
				Namespace::find_or_add_child_namespace method), so there is no need to 
				pre-declare them.
				parent_namespace_path can be nullptr. In this case the type is added to the global namespace.
				The size and alignment must respect the 4 constraints listed previously 
				(see size() and alignments() functions). */


		// destructor
		~Type();

	private: // internal services
		void _set_capabilities( Capabilities capability, bool value );

		struct _BaseTypeSlot
		{
			const Type * _base_type;
			UpDownCaster<> _updown_caster;
		};

		// _default_equality_comparer
		template <class TYPE> 
			static bool _default_equality_comparer( const void * first_operand, const void * second_operand );

		#if REFLECTIVE_TRACK_TYPE_HIERARCHY

			// _DerivedChainItem
			class _DerivedChainItem
			{
			public:
				_DerivedChainItem();
				void add_child( const Type & child );
				const Type * next() const;
				const Type * first_derived_class() const;				

			private: // data members
				const Type * _next_derived_brother;
				const Type * _first_derived;
				const Type * _last_derived;
			};

		#endif

	private: // data members

		// size - alignment
		size_t _size;
		size_t _alignment;		

		// base types
		_BaseTypeSlot _single_base_type;
		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_BaseTypeSlot * _multiple_base_types;
			unsigned _base_types_count;
		#endif
		#if REFLECTIVE_IS_DEBUG
			bool _dbg_base_types_set;			
		#endif

		// service functions
		LifeFunctions _life_functors;
		Stringizer * _stringizer;
		EqualsComparer _equality_comparer;
		LessThanComparer _less_than_comparer;
		TypeResolver _type_resolver;

		const CollectionHandler * _abstract_collection;
		Namespace * _parent_namespace;
		Capabilities _capabilities;

		#if REFLECTIVE_TRACK_TYPE_HIERARCHY

			static _DerivedChainItem & _derived_chain_root()
				{ static _DerivedChainItem res; return res; }

			mutable _DerivedChainItem _derived_child_member;
				
		#endif
	};

	Type::Capabilities operator | ( Type::Capabilities op1, Type::Capabilities op2 );
	Type::Capabilities operator & ( Type::Capabilities op1, Type::Capabilities op2 );

	class Enum;

} // namespace reflective

namespace reflective_externals
{

	// reflection of reflective::Type
	reflective::Class * init_type(
		reflective::Type * null_pointer_1,
		reflective::Type * null_pointer_2 );
	
	// reflection of reflective::Type::Capabilities
	reflective::Enum * init_type(
		reflective::Type::Capabilities * null_pointer_1,
		reflective::Type::Capabilities * null_pointer_2 );

} // namespace reflective_externals
