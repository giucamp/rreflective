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
	// Namespace::_create_namespace
	Namespace * Namespace::_create_namespace( SymbolName name )
	{
		return REFLECTIVE_LIFO_NEW( Namespace, name );
	}

	// Namespace::find_child_type
	const Type * Namespace::find_child_type( const StaticConstString & path ) const
	{
		const char * separator;
		const char * curr_path = path.start_of_chars();
		const Namespace * curr_namespace = this;
		const char * end_of_chars = path.end_of_chars();
		for(;;)
		{
			if( curr_path < end_of_chars )
				separator = _find_double_colon( curr_path, end_of_chars - curr_path );
			else
				separator = nullptr;

			if( !separator )
			{
				return curr_namespace->find_child_type( SymbolName( curr_path, end_of_chars - curr_path ) );
			}
			else
			{
				const SymbolName token_id( curr_path, separator - curr_path );
				curr_namespace = curr_namespace->find_child_namespace( token_id );
				if( !curr_namespace )
					return nullptr;
			}

			curr_path = separator + 2;
		}
	}

	// Namespace::find_child_namespace
	Namespace * Namespace::find_child_namespace( const StaticConstString & path )
	{
		const char * separator;
		const char * curr_path = path.start_of_chars();
		Namespace * curr_namespace = this;
		const char * end_of_chars = path.end_of_chars();

		if( curr_path == end_of_chars )
			return this;

		for(;;)
		{
			if( curr_path < end_of_chars )
				separator = _find_double_colon( curr_path, end_of_chars - curr_path );
			else
				separator = nullptr;

			if( !separator )
			{
				return curr_namespace->find_child_namespace( SymbolName( curr_path, end_of_chars - curr_path ) );
			}
			else
			{
				const SymbolName token_id( curr_path, separator - curr_path );
				curr_namespace = curr_namespace->find_child_namespace( token_id );
				if( !curr_namespace )
					return nullptr;
			}

			curr_path = separator + 2;
		}
	}

	// Namespace::find_or_add_child_namespace
	Namespace * Namespace::find_or_add_child_namespace( SymbolName name )
	{
		#if REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES && REFLECTIVE_ENABLE_ASSERT
			Symbol::dbg_check_sumbol_name( name.string() );
		#endif

		Namespace * result = find_child_namespace( name );
		if( result )
			return result;

		result = _create_namespace( name );
		add_child_namespace( *result );
		return result;
	}

	// Namespace::find_or_add_child_namespace
	Namespace * Namespace::find_or_add_child_namespace( const StaticConstString & path )
	{
		const char * separator;
		const char * curr_path = path.start_of_chars();
		Namespace * curr_namespace = this;
		const char * end_of_chars = path.end_of_chars();
		for(;;)
		{
			if( curr_path < end_of_chars )
				separator = _find_double_colon( curr_path, end_of_chars - curr_path );
			else
				separator = nullptr;

			if( !separator )
			{
				const SymbolName token_id( curr_path, end_of_chars - curr_path );
				if( token_id.is_empty() )
					return curr_namespace;
				Namespace * child_namespace = curr_namespace->find_child_namespace( token_id );
				if( child_namespace )
					return child_namespace;
				else
				{
					Namespace * new_namespace = _create_namespace( token_id );
					curr_namespace->add_child_namespace( *new_namespace );
					return new_namespace;
				}
			}
			else
			{
				const SymbolName token_id( curr_path, separator - curr_path );
				Namespace * child_namespace = curr_namespace->find_child_namespace( token_id );
				if( child_namespace )
					curr_namespace = child_namespace;
				else
				{
					Namespace * new_namespace = _create_namespace( token_id );
					curr_namespace->add_child_namespace( *new_namespace );
					curr_namespace = new_namespace;
				}
			}

			curr_path = separator + 2;
		}
	}


	// Namespace::_find_double_colon
	const char * Namespace::_find_double_colon( const char * src, size_t src_len )
	{
		int bracket_depth = 0;
		if( src_len >= 2 ) do {
			if( src[0] == '<' )
				bracket_depth++;
			else if( src[0] == '>' )
			{
				REFLECTIVE_ASSERT( bracket_depth > 0 );
				bracket_depth--;
			}
			if( bracket_depth == 0 )
			{
				if( src[0] == ':' && src[1] == ':' )
					return src;
			}
			src_len--;
			src++;
		} while( src_len >= 2 ); 

		return nullptr;
	}

	// Namespace::find_child_type
	const Type * Namespace::find_child_type( SymbolName name ) const
	{
		#if REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES && REFLECTIVE_ENABLE_ASSERT
			Symbol::dbg_check_sumbol_name( name.string() );
		#endif

		std::map< SymbolNameHash, const Type * >::const_iterator it = _child_types.find( name.uint_hash() );
		if( it != _child_types.end() )
			return it->second;
		return nullptr;
	}

	// Namespace::find_child_namespace
	const Namespace * Namespace::find_child_namespace( SymbolName name ) const
	{
		std::map< SymbolNameHash, Namespace * >::const_iterator namespace_it = _child_namespaces.find( name.uint_hash() );
		if( namespace_it != _child_namespaces.end() )
			return namespace_it->second;
		return nullptr;
	}

	// Namespace::find_child_type
	const Type * Namespace::find_child_type( SymbolNameHash hash ) const
	{
		std::map< SymbolNameHash, const Type * >::const_iterator it = _child_types.find( hash );
		if( it != _child_types.end() )
			return it->second;
		return nullptr;
	}

	// Namespace::find_child_namespace
	const Namespace * Namespace::find_child_namespace( SymbolNameHash hash ) const
	{
		std::map< SymbolNameHash, Namespace * >::const_iterator namespace_it = _child_namespaces.find( hash );
		if( namespace_it != _child_namespaces.end() )
			return namespace_it->second;
		return nullptr;
	}

	// Namespace::find_child_namespace (non const)
	Namespace * Namespace::find_child_namespace( SymbolName name )
	{
		#if REFLECTIVE_PRESERVE_SYMBOL_PLAIN_NAMES && REFLECTIVE_ENABLE_ASSERT
			Symbol::dbg_check_sumbol_name( name.string() );
		#endif

		std::map< SymbolNameHash, Namespace * >::iterator namespace_it = _child_namespaces.find( name.uint_hash() );
		if( namespace_it != _child_namespaces.end() )
			return namespace_it->second;
		return nullptr;
	}

	// Namespace::add_child_type
	void Namespace::add_child_type( const Type & type )
	{
		#if REFLECTIVE_IS_DEBUG && REFLECTIVE_ENABLE_ASSERT
			const Type * existing_type = find_child_type( type.name() );
			REFLECTIVE_ASSERT( existing_type == nullptr );
		#endif
		_child_types[ type.name().uint_hash() ] = &type;
	}

	// add_child_namespace
	void Namespace::add_child_namespace( Namespace & namespace_obj )
	{
		#if REFLECTIVE_IS_DEBUG && REFLECTIVE_ENABLE_ASSERT
			const Namespace * existing_namespace = find_child_namespace( namespace_obj.name() );
			REFLECTIVE_ASSERT( this != &namespace_obj ); // self inclusion
			REFLECTIVE_ASSERT( existing_namespace == nullptr );
		#endif

		REFLECTIVE_ASSERT( namespace_obj._parent == nullptr );
		namespace_obj._parent = this;
		_child_namespaces[ namespace_obj.name().uint_hash() ] = &namespace_obj;
	}
}

