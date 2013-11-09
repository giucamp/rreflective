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
	// BaseType::constructor
	inline BaseType::BaseType( const Type & _base_type, const UpDownCaster<> & _updown_caster )
		: base_type( _base_type ), updown_caster( _updown_caster )
	{
	}

	// BaseType<THIS_CLASS, BASE_CLASS>::from_types_only_to_base
	template <class THIS_CLASS, class BASE_CLASS>
		inline BaseType BaseType::from_types_only_to_base()
	{
		return BaseType( safe_get_type<BASE_CLASS>(),
			UpDownCaster<>::from_types_only_to_base<BASE_CLASS, THIS_CLASS>() );
	}

	// BaseType<THIS_CLASS, BASE_CLASS>::from_types
	template <class THIS_CLASS, class BASE_CLASS>
		inline BaseType BaseType::from_types()
	{
		return BaseType( safe_get_type<BASE_CLASS>(),
			UpDownCaster<>::from_types<BASE_CLASS, THIS_CLASS>() );
	}

	// Type::size
	inline size_t Type::size() const
	{
		return _size;
	}

	// Type::alignment
	inline unsigned Type::alignment() const
	{
		return _alignment;
	}

	// Type::is_aligned
	inline bool Type::is_aligned( const void * address ) const
	{
		return mem::is_aligned( address, _alignment );
	}

	// Type::upper_align
	template <class ADDRESS> inline ADDRESS Type::upper_align( ADDRESS address ) const
	{
		return mem::upper_align( address, _alignment );
	}

	// Type::lower_align
	template <class ADDRESS> inline ADDRESS Type::lower_align( ADDRESS address ) const
	{
		return mem::lower_align( address, _alignment );
	}

	// Type::parent_namespace
	inline const Namespace & Type::parent_namespace() const
	{
		return *_parent_namespace;
	}

	// Type::parent_namespace
	inline Namespace & Type::edit_parent_namespace()
	{
		return *_parent_namespace;
	}

	// Type::is_related_to
	inline bool Type::is_related_to( const Type & op ) const
	{
		return can_cast_to( op ) || op.can_cast_to( *this );
	}

	// Type::first_base_type
	inline const Type * Type::first_base_type() const
	{
		return base_type(0);
	}

	// Type::base_type
	inline const Type * Type::base_type( unsigned index ) const
	{
		#if !REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			//REFLECTIVE_ASSERT( index == 0 && _single_base_type._base_type != null );
			return _single_base_type._base_type;
		#else
			if( index < _base_types_count )
				return _multiple_base_types[ index ]._base_type;
			else
				return null;
		#endif
	}

	// Type::base_types_count
	inline unsigned Type::base_types_count() const
	{
		#if !REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			if( _single_base_type._base_type != null )
				return 1;
			return 0;
		#else
			return _base_types_count;
		#endif
	}

	// Type::updown_caster
	inline const UpDownCaster<> & Type::updown_caster( unsigned base_type_index ) const
	{
		#if !REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			REFLECTIVE_ASSERT( base_type_index <= 1 );
			REFLECTIVE_ASSERT( _single_base_type._base_type != null );			
			return _single_base_type._updown_caster;
		#else
			REFLECTIVE_ASSERT( base_type_index < _base_types_count );
			return _multiple_base_types[ base_type_index ]._updown_caster;
		#endif
	}

	// Type::set_no_base_type
	inline void Type::set_no_base_type()
	{
		// the base types can be set only once
		#if REFLECTIVE_IS_DEBUG
			REFLECTIVE_ASSERT( !_dbg_base_types_set );
			_dbg_base_types_set = true;
		#endif

		#if REFLECTIVE_TRACK_TYPE_HIERARCHY
			_derived_chain_root().add_child( *this );
		#endif

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_base_types_count = 0;
		#endif
	}


	// Type::set_base_type
	inline void Type::set_base_type( BaseType base_type )
	{
		// the base types can be set only once
		#if REFLECTIVE_IS_DEBUG
			REFLECTIVE_ASSERT( !_dbg_base_types_set );
			_dbg_base_types_set = true;
		#endif

		_single_base_type._base_type = &base_type.base_type;
		_single_base_type._updown_caster = base_type.updown_caster;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_base_types_count = 1;					
			_multiple_base_types = &_single_base_type;
		#endif

		if( !_type_resolver )
			set_type_resolver( base_type.base_type._type_resolver );

		if( !_abstract_collection )
			set_collection_handler( base_type.base_type._abstract_collection );

		#if REFLECTIVE_TRACK_TYPE_HIERARCHY
			base_type.base_type._derived_child_member.add_child( *this );
		#endif
	}

	// Type::set_base_type
	inline void Type::set_base_type( const Type & base_type, const UpDownCaster<> & updown_caster )
	{
		// the base types can be set only once
		#if REFLECTIVE_IS_DEBUG
			REFLECTIVE_ASSERT( !_dbg_base_types_set );
			_dbg_base_types_set = true;
		#endif

		_single_base_type._base_type = &base_type;
		_single_base_type._updown_caster = updown_caster;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_base_types_count = 1;
			_multiple_base_types = &_single_base_type;
		#endif

		if( !_type_resolver )
			set_type_resolver( base_type._type_resolver );

		if( !_abstract_collection )
			set_collection_handler( base_type._abstract_collection );

		#if REFLECTIVE_TRACK_TYPE_HIERARCHY
			base_type._derived_child_member.add_child( *this );
		#endif
	}

	// Type::set_base_types
	template <unsigned COUNT>
		void Type::set_base_types( const BaseType (&base_types)[ COUNT ] )
	{
		REFLECTIVE_COMPILETIME_ASSERT( REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE || COUNT <= 1 );

		if( COUNT == 0 )
			set_no_base_type();
		else if( COUNT == 1 )
			set_base_type( base_types[0] );
		else
		{
			// the base types can be set only once
			#if REFLECTIVE_IS_DEBUG
				REFLECTIVE_ASSERT( !_dbg_base_types_set );
				_dbg_base_types_set = true;
			#endif

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

				_base_types_count = COUNT;

				_multiple_base_types = static_cast<_BaseTypeSlot*>(
					reflective_externals::mem_lifo_alloc(
						alignment_of( _BaseTypeSlot ), sizeof( _BaseTypeSlot ) * COUNT ) );

				unsigned index = 0; do {
					_multiple_base_types[ index ]._base_type = &base_types[index].base_type;
					_multiple_base_types[ index ]._updown_caster = base_types[index].updown_caster;

					if( !_type_resolver )
						set_type_resolver( _multiple_base_types[ index ]._base_type->_type_resolver );

					if( !_abstract_collection )
						set_collection_handler( _multiple_base_types[ index ]._base_type->_abstract_collection );

					#if REFLECTIVE_TRACK_TYPE_HIERARCHY
						_multiple_base_types[ index ]._base_type->_derived_child_member.add_child( *this );
					#endif

				} while( ++index < COUNT );
				_base_types_count = COUNT;
			#endif

		}
	}

	// Type::life_functions
	inline const LifeFunctions & Type::life_functions() const
	{
		return _life_functors;
	}

	// Type::type_resolver
	inline TypeResolver Type::type_resolver() const
	{
		return _type_resolver;
	}

	// Type::resolve_type
	inline const Type & Type::resolve_type( const void * object ) const
	{
		REFLECTIVE_ASSERT( is_aligned( object ) );
		REFLECTIVE_ASSERT( check_capabilities( eHasTypeResolver ) );

		return (*_type_resolver)( *this, object );
	}

	// Type::set_string_functions with global functions
	inline void Type::set_string_functions( 
		GlobalFunctionsStringizer::ToStringDumper to_string,
		GlobalFunctionsStringizer::FromStringAssigner from_string )
	{
		_stringizer = GlobalFunctionsStringizer::lifo_create( to_string, from_string );
		_set_capabilities( eHasFromStringAssigner, from_string != 0 );
		_set_capabilities( eHasToStringDumper, to_string != 0 );
	}

	// Type::set_string_functions with methods
	template <class TYPE> inline void Type::set_string_functions( 
		void (TYPE::*to_string)( ToStringBuffer & dest_buffer ) const,
		bool (TYPE::*from_string)( FromStringBuffer & source_buffer,
			ToStringBuffer & error_buffer ) )
	{
		_stringizer = MethodsStringizer<TYPE>::lifo_create( to_string, from_string );
		_set_capabilities( eHasFromStringAssigner, from_string != 0 );
		_set_capabilities( eHasToStringDumper, to_string != 0 );
	}

	// Type::set_string_functions with methods
	template <class TYPE> inline void Type::set_string_functions( 
		void (TYPE::*to_string)( ToStringBuffer & dest_buffer ) const )
	{
		_stringizer = MethodsStringizer<TYPE>::lifo_create( to_string, 0 );
		_set_capabilities( eHasToStringDumper, to_string != 0 );
	}

	// Type::assign_from_string
	inline bool Type::assign_from_string( FromStringBuffer & source_buffer, void * object, ToStringBuffer & error_buffer ) const
	{
		REFLECTIVE_ASSERT( is_aligned( object ) );
		REFLECTIVE_ASSERT( check_capabilities( eHasFromStringAssigner ) );

		return _stringizer->assign_from_string( source_buffer, *this, object, error_buffer );
	}

	// Type::to_string
	inline void Type::to_string( ToStringBuffer & dest_buffer, const void * object ) const
	{
		REFLECTIVE_ASSERT( is_aligned( object ) );
		REFLECTIVE_ASSERT( check_capabilities( eHasToStringDumper ) );

		return _stringizer->to_string( dest_buffer, *this, object );
	}

	// Type::compute_to_string_required_length
	inline size_t Type::compute_to_string_required_length( const void * object ) const
	{
		ToStringBuffer empty_buffer;
		to_string( empty_buffer, object );
		return empty_buffer.needed_length();
	}

	// Type::to_string
	inline size_t Type::to_string( char * dest_buffer, size_t dest_buffer_size, const void * object ) const
	{
		ToStringBuffer buffer( dest_buffer, dest_buffer_size );
		to_string( buffer, object );
		return buffer.needed_length();
	}

	// Type::set_comparers
	inline void Type::set_comparers( EqualsComparer equality_comparer, LessThanComparer less_than_comparer )
	{
		_equality_comparer = equality_comparer;
		_less_than_comparer = less_than_comparer;
		_set_capabilities( eHasEqualityComparer, equality_comparer != null );
		_set_capabilities( eHasLessThanComparer, less_than_comparer != null );
	}

	// Type::set_default_equality_comparers
	template <class TYPE> 
		inline void Type::set_default_equality_comparers()
	{
		_equality_comparer = &_default_equality_comparer<TYPE>;
		_set_capabilities( eHasEqualityComparer, true );	
	}

	// Type::_default_equality_comparer
	template <class TYPE> 
		inline bool Type::_default_equality_comparer( const void * first_operand, const void * second_operand )
	{
		const TYPE & first = *static_cast<const TYPE*>( first_operand );
		const TYPE & second = *static_cast<const TYPE*>( second_operand );
		return first == second;
	}

	// Type::equality_comparer
	inline EqualsComparer Type::equality_comparer() const
	{
		return _equality_comparer;
	}

	// Type::less_than_comparer
	inline LessThanComparer Type::less_than_comparer() const
	{
		return _less_than_comparer;
	}

	// Type::object_equals
	inline bool Type::object_equals( const void * first_operand, const void * second_operand ) const
	{
		REFLECTIVE_ASSERT( is_aligned( first_operand ) );
		REFLECTIVE_ASSERT( is_aligned( second_operand ) );
		return (*_equality_comparer)( first_operand, second_operand );
	}

	// Type::object_less_than
	inline bool Type::object_less_than( const void * first_operand, const void * second_operand ) const
	{
		REFLECTIVE_ASSERT( is_aligned( first_operand ) );
		REFLECTIVE_ASSERT( is_aligned( second_operand ) );
		return (*_less_than_comparer)( first_operand, second_operand );
	}

	// Type::set_collection_handler
	inline void Type::set_collection_handler( const CollectionHandler * collection_interface )
	{
		REFLECTIVE_ASSERT( _abstract_collection == null ); // the collection interface can be set only once
		_abstract_collection = collection_interface; 
		_set_capabilities( eHasCollectionHandler, collection_interface != null );
	}

	// Type::set_type_resolver
	inline void Type::set_type_resolver( TypeResolver type_resolver )
	{
		_type_resolver = type_resolver;
		_set_capabilities( eHasTypeResolver, type_resolver != null );
	}

	// Type::collection_handler
	inline const CollectionHandler * Type::collection_handler() const
	{
		return _abstract_collection;
	}

	// Type::construct
	inline void Type::construct( void * object ) const
	{
		#if REFLECTIVE_IS_DEBUG
			REFLECTIVE_ASSERT( check_capabilities( eHasDefaultConstructor ) );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( object ) );

		#endif
		const LifeFunctions::CtorCaller caller = _life_functors.constructor_caller();
		void * end_of_object = (void *)( size_t( object ) + _size );
		(*caller)( object, end_of_object );
	}

	// Type::construct
	inline void Type::construct( void * start_address, void * end_address ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasDefaultConstructor ) );

			REFLECTIVE_ASSERT( start_address <= end_address );

			const size_t object_size = size();
			const size_t array_size = size_t( end_address ) - size_t( start_address );
			REFLECTIVE_ASSERT( (array_size % object_size) == 0 );
			REFLECTIVE_UNUSED( object_size );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( start_address ) );
			REFLECTIVE_ASSERT( is_aligned( end_address ) );

		#endif
		const LifeFunctions::CtorCaller caller = _life_functors.constructor_caller();
		(*caller)( start_address, end_address );
	}

	// Type::destroy
	inline void Type::destroy( void * object ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasDestructor ) );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( object ) );

		#endif

		const LifeFunctions::CtorCaller caller = _life_functors.destructor_caller();
		void * end_of_object = (void *)( size_t( object ) + _size );
		(*caller)( object, end_of_object );
	}

	// Type::destroy
	inline void Type::destroy( void * start_address, void * end_address ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasDestructor ) );

			REFLECTIVE_ASSERT( start_address <= end_address );

			const size_t object_size = size();
			const size_t array_size = size_t( end_address ) - size_t( start_address );
			REFLECTIVE_ASSERT( (array_size % object_size) == 0 );
			REFLECTIVE_UNUSED( object_size );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( start_address ) );
			REFLECTIVE_ASSERT( is_aligned( end_address ) );

		#endif

		const LifeFunctions::CtorCaller caller = _life_functors.destructor_caller();
		(*caller)( start_address, end_address );
	}

	// Type::copy_construct
	inline void Type::copy_construct( void * object, const void * source_object ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasCopier ) );

			REFLECTIVE_ASSERT( object != source_object );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( object ) );
			REFLECTIVE_ASSERT( is_aligned( source_object ) );

		#endif

		const LifeFunctions::Copier copier = _life_functors.copier();
		void * end_of_object = (void *)( size_t( object ) + _size );
		(*copier)( object, end_of_object, source_object );
	}

	// Type::multiple_copy_construct
	inline void Type::multiple_copy_construct( void * start_address, void * end_address, const void * source_object ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasCopier ) );

			REFLECTIVE_ASSERT( start_address <= end_address );

			const size_t object_size = size();
			const size_t array_size = size_t( end_address ) - size_t( start_address );
			REFLECTIVE_ASSERT( (array_size % object_size) == 0 );
			REFLECTIVE_UNUSED( array_size );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( start_address ) );
			REFLECTIVE_ASSERT( is_aligned( end_address ) );
			REFLECTIVE_ASSERT( is_aligned( source_object ) );

			REFLECTIVE_ASSERT( source_object < start_address || source_object >= end_address );	

		#endif

		if( start_address == end_address )
			return;

		const LifeFunctions::Copier copier = _life_functors.copier();

		void * curr = start_address, * next;
		do {

			next = mem::address_add( curr, _size );

			(*copier)( curr, next, source_object );

			curr = next;

		} while( next != end_address );		
	}


	// Type::move
	inline void Type::move( void * dest_start, void * dest_end, void * source_start ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasMover ) );

			REFLECTIVE_ASSERT( dest_start <= dest_end );

			// check dest size
			const size_t object_size = size();
			const size_t array_size = size_t( dest_end ) - size_t( dest_start );
			REFLECTIVE_ASSERT( (array_size % object_size) == 0 );
			REFLECTIVE_UNUSED( array_size );
			REFLECTIVE_UNUSED( object_size );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( dest_start ) );
			REFLECTIVE_ASSERT( is_aligned( dest_end ) );
			REFLECTIVE_ASSERT( is_aligned( source_start ) );

		#endif

		const LifeFunctions::Mover mover = _life_functors.mover();
		(*mover)( dest_start, dest_end, source_start );
	}

	// Type::copy
	inline void Type::copy( void * dest_start, void * dest_end, const void * source_start ) const
	{
		#if REFLECTIVE_IS_DEBUG

			REFLECTIVE_ASSERT( check_capabilities( eHasCopier ) );

			REFLECTIVE_ASSERT( dest_start <= dest_end );

			const size_t object_size = size();

			const size_t array_size = size_t( dest_end ) - size_t( dest_start );
			REFLECTIVE_ASSERT( (array_size % object_size) == 0 );

			// check for overlapping
			const size_t uint_dest_start = size_t( dest_start );
			const size_t uint_dest_end = size_t( dest_end );
			const size_t uint_source_start = size_t( source_start );
			const size_t uint_source_end = uint_source_start + array_size;
			REFLECTIVE_ASSERT( uint_dest_start < uint_source_start || uint_dest_start >= uint_source_end );
			REFLECTIVE_ASSERT( uint_dest_end < uint_source_start || uint_dest_end >= uint_source_end );

			// check alignment
			REFLECTIVE_ASSERT( is_aligned( dest_start ) );
			REFLECTIVE_ASSERT( is_aligned( dest_end ) );
			REFLECTIVE_ASSERT( is_aligned( source_start ) );

		#endif

		const LifeFunctions::Copier copier = _life_functors.copier();
		(*copier)( dest_start, dest_end, source_start );
	}

	// Type::_set_capabilities
	inline void Type::_set_capabilities( Capabilities capability, bool value )
	{
		if( value )
			_capabilities = _capabilities | capability;
		else
			_capabilities = static_cast<Capabilities>( _capabilities & ~capability );
	}

	// Type::capabilities
	inline Type::Capabilities Type::capabilities() const
	{
		return _capabilities;
	}

	// Type::check_capabilities
	inline bool Type::check_capabilities( Type::Capabilities capabilities_to_chech_for ) const
	{
		return (_capabilities & capabilities_to_chech_for) == capabilities_to_chech_for;
	}

	// Type::indicize
	inline void * Type::indicize( void * start_address, size_t index ) const
	{
		REFLECTIVE_ASSERT( is_aligned( start_address ) );
		size_t address = index * _size;
		address += reinterpret_cast<size_t>( start_address );
		return reinterpret_cast<void *>( address );
	}

	// Type::indicize
	inline const void * Type::indicize( const void * start_address, size_t index ) const
	{
		REFLECTIVE_ASSERT( is_aligned( start_address ) );
		size_t address = index * _size;
		address += reinterpret_cast<size_t>( start_address );
		return reinterpret_cast<const void *>( address );
	}

	// operator | ( Type::Capabilities op1, Type::Capabilities op2 )
	inline Type::Capabilities operator | ( Type::Capabilities op1, Type::Capabilities op2 )
	{
		return static_cast<Type::Capabilities>(
			static_cast<int>( op1 ) | static_cast<int>( op2 ) );
	}

	// operator & ( Type::Capabilities op1, Type::Capabilities op2 )
	inline Type::Capabilities operator & ( Type::Capabilities op1, Type::Capabilities op2 )
	{
		return static_cast<Type::Capabilities>(
			static_cast<int>( op1 ) & static_cast<int>( op2 ) );
	}

	// Type::pointer_to_base_type
	inline void * Type::pointer_to_base_type( void * pointer, unsigned base_type_index ) const
	{
		REFLECTIVE_ASSERT( base_type_index < base_types_count() );
		const _BaseTypeSlot * base_type_slot;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

			if( _single_base_type._base_type != null )
				base_type_slot = &_single_base_type;
			else
				base_type_slot = _multiple_base_types + base_type_index;

		#else
			
			base_type_slot = &_single_base_type;

		#endif

		return base_type_slot->_updown_caster.pointer_to_base_type( pointer );
	}

	// Type::pointer_from_base_type
	inline void * Type::pointer_from_base_type( void * pointer, unsigned base_type_index ) const
	{
		REFLECTIVE_ASSERT( base_type_index < base_types_count() );
		const _BaseTypeSlot * base_type_slot;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

			if( _single_base_type._base_type != null )
				base_type_slot = &_single_base_type;
			else
				base_type_slot = _multiple_base_types + base_type_index;

		#else
			
			base_type_slot = &_single_base_type;

		#endif

		return base_type_slot->_updown_caster.pointer_from_base_type( pointer );
	}

	// Type::pointer_to_base_type
	inline const void * Type::pointer_to_base_type( const void * pointer, unsigned base_type_index ) const
	{
		REFLECTIVE_ASSERT( base_type_index < base_types_count() );
		const _BaseTypeSlot * base_type_slot;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

			if( _single_base_type._base_type != null )
				base_type_slot = &_single_base_type;
			else
				base_type_slot = _multiple_base_types + base_type_index;

		#else
			
			base_type_slot = &_single_base_type;

		#endif

		return base_type_slot->_updown_caster.pointer_to_base_type( pointer );
	}

	// Type::pointer_from_base_type
	inline const void * Type::pointer_from_base_type( const void * pointer, unsigned base_type_index ) const
	{
		REFLECTIVE_ASSERT( base_type_index < base_types_count() );
		const _BaseTypeSlot * base_type_slot;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE

			if( _single_base_type._base_type != null )
				base_type_slot = &_single_base_type;
			else
				base_type_slot = _multiple_base_types + base_type_index;

		#else
			
			base_type_slot = &_single_base_type;

		#endif

		return base_type_slot->_updown_caster.pointer_from_base_type( pointer );
	}

	#if REFLECTIVE_TRACK_TYPE_HIERARCHY

		// Type::HierarchyIterator::constructor()
		inline Type::HierarchyIterator::HierarchyIterator()
			: _current( _derived_chain_root().first_derived_class() )
				{ }

		// Type::HierarchyIterator::constructor( base_class )
		inline Type::HierarchyIterator::HierarchyIterator( const Type & base_class )
			: _current( base_class._derived_child_member.first_derived_class() )
		{
		}

		// Type::HierarchyIterator::operator bool
		inline Type::HierarchyIterator::operator bool () const
		{
			return _current != null;
		}

		// Type::HierarchyIterator::operator *
		inline const Type & Type::HierarchyIterator::operator * () const
		{
			REFLECTIVE_ASSERT( _current != null ); //reached end of iteration
			return *_current;
		}

		// Type::HierarchyIterator::operator ->
		inline const Type * Type::HierarchyIterator::operator -> () const
		{
			REFLECTIVE_ASSERT( _current != null ); //reached end of iteration
			return _current;
		}

		// Type::HierarchyIterator::postfix increment
		inline void Type::HierarchyIterator::operator ++ ( int )
		{
			_current = _current->_derived_child_member.next();
		}

		// Type::HierarchyIterator::prefix increment
		inline Type::HierarchyIterator & Type::HierarchyIterator::operator ++ ()
		{
			_current = _current->_derived_child_member.next();
			return *this;
		}

		// Type::_DerivedChainItem::cosntructor
		inline Type::_DerivedChainItem::_DerivedChainItem()
			: _next_derived_brother( null ), _first_derived( null ), _last_derived( null )
		{
		}

		// Type::_DerivedChainItem::add_child
		inline void Type::_DerivedChainItem::add_child( const Type & child )
		{
			if( _last_derived )
			{
				REFLECTIVE_ASSERT( _first_derived != null );
				REFLECTIVE_ASSERT( _last_derived->_derived_child_member._next_derived_brother == null );
				_last_derived->_derived_child_member._next_derived_brother = &child;
			}
			else
			{
				REFLECTIVE_ASSERT( _first_derived == null );
				_first_derived = &child;
			}

			_last_derived = &child;
		}

		// Type::_DerivedChainItem::next
		inline const Type * Type::_DerivedChainItem::next() const
		{
			return _next_derived_brother;
		}

		// Type::_DerivedChainItem::first_derived_class
		inline const Type * Type::_DerivedChainItem::first_derived_class() const
		{
			return _first_derived;
		}

	#endif // #if REFLECTIVE_TRACK_TYPE_HIERARCHY

	// Type::destructor
	inline Type::~Type()
	{

	}

} // namespace reflective

