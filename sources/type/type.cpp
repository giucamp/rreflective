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

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{
	// Type::constructor
	Type::Type( const StaticConstString & parent_namespace_path,
			const SymbolName & name, size_t size, size_t alignment )
		: Symbol( name ), 
			_equality_comparer( nullptr ), _less_than_comparer( nullptr ), 
			_type_resolver( nullptr ),
			_abstract_collection( nullptr ),
			_capabilities( eCapabilitiesNone )
	{
		set_type_id( eTypeType );

		#if REFLECTIVE_IS_DEBUG
			_dbg_base_types_set = false;
		#endif

		_single_base_type._base_type = nullptr;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_multiple_base_types = nullptr;
			_base_types_count = 0;
		#endif

		// check size and alignment
		REFLECTIVE_ASSERT( size > 0 ); // the size cannot be zero
		REFLECTIVE_ASSERT( alignment <= size ); // the alignment cannot be greater than the size
		REFLECTIVE_ASSERT( ( alignment & ( alignment >> 1 ) ) == 0 ); /* the alignment must be 
			zero or an integer power of 2 */
		REFLECTIVE_ASSERT( ( size & ( alignment - 1 ) ) == 0 ); // the size must be aligned

		_size = size;
		_alignment = alignment;
		if( _alignment == 0 )
			_alignment = 1;

		_stringizer = nullptr;

		// set the namespace
		Namespace & global_namespace = Namespace::edit_global();
		_parent_namespace = global_namespace.find_or_add_child_namespace( parent_namespace_path );
		_parent_namespace->add_child_type( *this );
	}

	// Type::constructor
	Type::Type( Namespace & parent_namespace, const SymbolName & name, size_t size, size_t alignment )
		: Symbol( name ), 
			_equality_comparer( nullptr ), _less_than_comparer( nullptr ), 
			_type_resolver( nullptr ),
			_abstract_collection( nullptr ),
			_capabilities( eCapabilitiesNone )
	{
		set_type_id( eTypeType );

		#if REFLECTIVE_IS_DEBUG
			_dbg_base_types_set = false;
		#endif

		_single_base_type._base_type = nullptr;

		#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
			_multiple_base_types = nullptr;
			_base_types_count = 0;
		#endif

		// check size and alignment
		REFLECTIVE_ASSERT( size > 0 ); // the size cannot be zero
		REFLECTIVE_ASSERT( alignment <= size ); // the alignment cannot be greater than the size
		REFLECTIVE_ASSERT( ( alignment & ( alignment >> 1 ) ) == 0 ); /* the alignment must be 
			zero or an integer power of 2 */
		REFLECTIVE_ASSERT( ( size & ( alignment - 1 ) ) == 0 ); // the size must be aligned

		_size = size;
		_alignment = alignment;
		if( _alignment == 0 )
			_alignment = 1;

		_stringizer = nullptr;

		// set the namespace		
		_parent_namespace = &parent_namespace;
		_parent_namespace->add_child_type( *this );
	}


	// Type::can_cast_to
	bool Type::can_cast_to( const Type & op ) const
	{
		const Type * curr_type = this;
		do {

			if( *curr_type == op )
				return true;

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				if( _base_types_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
 						const Type * base_type = _multiple_base_types[base_type_index]._base_type;
						if( base_type->can_cast_to( op ) )
							return true;
					} while( ++base_type_index < _base_types_count );
				}
			#endif

			curr_type = curr_type->_single_base_type._base_type;

		} while( curr_type );

		return false;
	}

	// Type::has_base_type_with_name
	bool Type::has_base_type_with_name( SymbolName name ) const
	{
		const Type * curr_type = this;
		do {
			
			if( curr_type->name() == name )
				return true;

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				if( _base_types_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = _multiple_base_types[base_type_index]._base_type;
						if( base_type->has_base_type_with_name( name ) )
							return true;
					} while( ++base_type_index < _base_types_count );
				}
			#endif

			curr_type = curr_type->_single_base_type._base_type;

		} while( curr_type );
		return false;
	}

	// Type::try_cast_pointer_to
	void * Type::try_cast_pointer_to( const Type & dest_type, void * object ) const
	{
		const Type * curr_type = this;
		do {

			if( *curr_type == dest_type )
				return object;

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				if( _base_types_count > 1 )
				{
					unsigned base_type_index = 1;
					do {	

						void * base_object = _multiple_base_types[base_type_index]._updown_caster.pointer_to_base_type( object );
						void * result = _multiple_base_types[ base_type_index ]._base_type->
							try_cast_pointer_to( dest_type, base_object );
						if( result != nullptr )
							return result;
					} while( ++base_type_index < _base_types_count );
				}
			#endif

			object = curr_type->_single_base_type._updown_caster.pointer_to_base_type( object );
			curr_type = curr_type->_single_base_type._base_type;

		} while( curr_type );

		return nullptr;
	}

	// Type::try_cast_pointer_to
	const void * Type::try_cast_pointer_to( const Type & dest_type, const void * object ) const
	{
		const Type * curr_type = this;
		do {

			if( *curr_type == dest_type )
				return object;

			#if REFLECTIVE_ALLOW_MULTIPLE_INHERITANCE
				if( _base_types_count > 1 )
				{
					unsigned base_type_index = 1;
					do {
						const Type * base_type = _multiple_base_types[base_type_index]._base_type;
						const void * result = base_type->try_cast_pointer_to( dest_type, object );
						if( result != nullptr )
							return result;
					} while( ++base_type_index < _base_types_count );
				}
			#endif

			curr_type = curr_type->_single_base_type._base_type;
			object = curr_type->_single_base_type._updown_caster.pointer_to_base_type( object );

		} while( curr_type );

		return nullptr;
	}

	// Type::set_life_functions
	void Type::set_life_functions( const LifeFunctions & new_life_functors )
	{
		_life_functors = new_life_functors;

		_set_capabilities( eHasDefaultConstructor, _life_functors.constructor_caller() != nullptr );
		_set_capabilities( eHasDestructor, _life_functors.destructor_caller() != nullptr );
		_set_capabilities( eHasCopier, _life_functors.copier() != nullptr );
		_set_capabilities( eHasMover, _life_functors.mover() != nullptr );
	}

	// Type::type_full_name_to_string
	void Type::type_full_name_to_string( ToStringBuffer & dest_buffer ) const
	{
		bool first = true;
		const Namespace * last_dumped_namespace = &Namespace::global();
		const Namespace * this_parent_namespace = &parent_namespace();
		for(;;)
		{

			const Namespace * curr_namespace = this_parent_namespace;
			if( curr_namespace != last_dumped_namespace ) 
			{
				for(;;) 
				{
					const Namespace * next_namespace = curr_namespace->parent();
					REFLECTIVE_ASSERT( next_namespace != nullptr );
					if( next_namespace == last_dumped_namespace )
						break;
					curr_namespace = curr_namespace->parent();
				}
			}

			if( last_dumped_namespace == this_parent_namespace )
				break;

			// append namespace name
			if( !first )
				dest_buffer.append_literal( "::" );
			first = false;
			const SymbolName & name = curr_namespace->name();
			name.to_string( dest_buffer );
			last_dumped_namespace = curr_namespace;
		}

		// append type name
		if( !first )
			dest_buffer.append_literal( "::" );
		const SymbolName & name = this->name();
		name.to_string( dest_buffer );
	}

	template < size_t PREFIX_LEN >
		void _string_skip_prefix( const char * & inout_string, const char (&prefix)[PREFIX_LEN] )
	{
		REFLECTIVE_COMPILETIME_ASSERT( PREFIX_LEN > 0 );
		if( strncmp( inout_string, prefix, PREFIX_LEN - 1 ) == 0 )
			inout_string += PREFIX_LEN - 1;
	}

	// separate_symbol_name
	void separate_symbol_name( const char * full_name,
		StaticConstString * out_namespace_path, SymbolName * out_symbol_name, bool remove_template_parameter )
	{
		StaticConstString name;
		separate_symbol_name( full_name, out_namespace_path, &name, remove_template_parameter );
		*out_symbol_name = SymbolName( name.start_of_chars(), name.length() );
	}
	void separate_symbol_name( const char * full_name,
		StaticConstString * out_namespace_path, StaticConstString * out_symbol_name, bool remove_template_parameter )
	{
		const char * namespace_start = full_name;
		_string_skip_prefix( namespace_start, "class " );
		_string_skip_prefix( namespace_start, "struct " );
		_string_skip_prefix( namespace_start, "enum " );

		// find last "::"
		const char * last_double_colon = nullptr;
		const char * curr_pos = namespace_start;
		int brackets_depth = 0;
		while( *curr_pos )
		{
			switch( curr_pos[0] )
			{
				case ':':
					if( brackets_depth == 0 && curr_pos[1] == ':' )
						last_double_colon = curr_pos;
					break;

				case '<':
					brackets_depth++;
					break;

				case '>':
					REFLECTIVE_ASSERT( brackets_depth > 0 );
					brackets_depth--;
					break;
			}
			curr_pos++;
		}
		
		if( last_double_colon )
		{
			if( out_namespace_path )
				*out_namespace_path = StaticConstString( namespace_start, last_double_colon - namespace_start );
			if( out_symbol_name )
				*out_symbol_name = last_double_colon + 2;
		}
		else
		{
			if( out_namespace_path )
				*out_namespace_path = StaticConstString();
			if( out_symbol_name )
				*out_symbol_name = namespace_start;
		}

		// truncate out_symbol_name at the first '<'
		if( remove_template_parameter )
		{
			const size_t curr_length = out_symbol_name->length();
			const char * chars = out_symbol_name->start_of_chars();
			for( size_t index = 0; index < curr_length; index++ )
			{			
				if( chars[index] == '<' )
				{
					*out_symbol_name = StaticConstString( chars, index );
					break;
				}
				REFLECTIVE_ASSERT( chars[index] == '_' || isalnum( chars[index] ) != 0 );
			}
		}
	}

	// Type::create_instance
	void * Type::create_instance() const
	{
		void * object = reflective_externals::mem_alloc( _alignment, _size );
		construct( object );
		return object;
	}

	// Type::destroy_instance
	void Type::destroy_instance( void * object ) const
	{		
		destroy( object );
		reflective_externals::mem_free( object );
	}

} // namespace reflective

namespace reflective_externals
{
	// reflection of reflective::Type::Capabilities
	reflective::Enum * init_type(
		reflective::Type::Capabilities * null_pointer_1,
		reflective::Type::Capabilities * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		
		static Enum * enum_object = nullptr;
		if( enum_object != nullptr )
			return enum_object;
		
		const Enum::Member * members[] = 
		{
			new_enum_member( "CapabilitiesNone", reflective::Type::eCapabilitiesNone ),
			new_enum_member( "HasDefaultConstructor", reflective::Type::eHasDefaultConstructor ),
			new_enum_member( "HasCopier", reflective::Type::eHasCopier ),
			new_enum_member( "HasMover", reflective::Type::eHasMover ),
			new_enum_member( "HasDestructor", reflective::Type::eHasDestructor ),
			new_enum_member( "HasEqualityComparer", reflective::Type::eHasEqualityComparer ),
			new_enum_member( "HasLessThanComparer", reflective::Type::eHasLessThanComparer ),
			new_enum_member( "HasToStringDumper", reflective::Type::eHasToStringDumper ),
			new_enum_member( "HasFromStringAssigner", reflective::Type::eHasFromStringAssigner ),
			new_enum_member( "HasCollectionHandler", reflective::Type::eHasCollectionHandler ),
		};
		
		enum_object = new_enum( "reflective::Type", "Capabilities" );
		enum_object->edit_members().assign( members );
		return enum_object;
	}


	// reflection of reflective::Type
	reflective::Class * init_type(
		reflective::Type * null_pointer_1,
		reflective::Type * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Type ThisClass;
		typedef Symbol BaseClass;
		
		static Class * result = nullptr;
		if( result != nullptr )
			return result;
		
		// class object
		Class * class_object = new_class<ThisClass>( "reflective", "Type" );
		result = class_object;
		
		// services
		class_object->set_base_type( BaseType::from_types<ThisClass,BaseClass>() );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "parent_namespace", &ThisClass::parent_namespace, ClassMember::BACK_REFERENCE ),
			new_property<ThisClass>( "capabilities", &ThisClass::capabilities ),
			new_property<ThisClass>( "size", &ThisClass::size ),
			new_property<ThisClass>( "alignment", &ThisClass::alignment ),
			new_property<ThisClass>( "base_types_count", &ThisClass::base_types_count ),
			new_property<ThisClass>( "first_base_type", &ThisClass::first_base_type, ClassMember::BACK_REFERENCE ),			
		};
				
		// assign members
		class_object->assign_properties( properties );
		
		// return type
		return result;
	}


} // namespace reflective_externals
