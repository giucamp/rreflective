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
	// Namespace::edit_global
	inline Namespace & Namespace::edit_global()
	{		
		static Namespace * global = _create_namespace( "global" );
		return *global;		
	}

	// Namespace::global
	inline const Namespace & Namespace::global()
	{
		return edit_global();
	}

	// Namespace::type_count
	inline size_t Namespace::type_count() const
	{
		return _child_types.size();
	}

	// Namespace::namespace_count
	inline size_t Namespace::namespace_count() const
	{
		return _child_namespaces.size();
	}

	// Namespace::constructor
	inline Namespace::Namespace( const SymbolName & name )
		: Symbol( name ), _parent( nullptr )
	{
		set_type_id( eTypeNamespace );
	}

	// Namespace::destructor
	inline Namespace::~Namespace()
	{
	}

	// Namespace::remove_child_type
	inline void Namespace::remove_child_type( const Type & type )
	{
		_child_types.erase( type.name().uint_hash() );
	}

	// Namespace::parent
	inline Namespace * Namespace::parent()
	{
		return _parent;
	}

	// Namespace::parent (const)
	inline const Namespace * Namespace::parent() const
	{
		return _parent;
	}

	// Namespace::find_child_namespace
	inline const Namespace * Namespace::find_child_namespace( const StaticConstString & path ) const
	{
		return const_cast<Namespace*>( this )->find_child_namespace( path );
	}

	// Namespace::child_types
	inline const std::map< SymbolNameHash, const Type * > & Namespace::child_types() const
	{
		return _child_types; 
	}

	// Namespace::child_namespaces
	inline const std::map< SymbolNameHash, Namespace * > & Namespace::child_namespaces() const
	{
		return _child_namespaces;
	}


			/**** Namespace::TypeIterator ****/

	// Namespace::TypeIterator::constructor
	inline Namespace::TypeIterator::TypeIterator( const Namespace & namespace_object )
		: _namespace_object( namespace_object ),
		  _inner_iterator( _namespace_object._child_types.begin() )
	{
		
	}

	// Namespace::TypeIterator::operator *
	inline const Type & Namespace::TypeIterator::operator * () const
	{
		return *_inner_iterator->second;
	}

	// Namespace::TypeIterator::operator ->
	inline const Type * Namespace::TypeIterator::operator -> () const
	{
		return _inner_iterator->second;
	}

	// Namespace::TypeIterator::operator ++
	inline bool Namespace::TypeIterator::operator ++ ()
	{
		_inner_iterator++;
		return _inner_iterator != _namespace_object._child_types.end();
	}

	// Namespace::TypeIterator::operator bool
	inline Namespace::TypeIterator::operator bool () const
	{
		return _inner_iterator != _namespace_object._child_types.end();
	}


			/**** Namespace::NamespaceIterator ****/

	// Namespace::NamespaceIterator::constructor
	inline Namespace::NamespaceIterator::NamespaceIterator( const Namespace & namespace_object )
		: _namespace_object( namespace_object ),
		  _inner_iterator( _namespace_object._child_namespaces.begin() )
	{
	}

	// Namespace::NamespaceIterator::operator *
	inline const Namespace & Namespace::NamespaceIterator::operator * () const
	{
		return *_inner_iterator->second;
	}

	// Namespace::NamespaceIterator::operator ->
	inline const Namespace * Namespace::NamespaceIterator::operator -> () const
	{
		return _inner_iterator->second;
	}

	// Namespace::NamespaceIterator::operator ++
	inline bool Namespace::NamespaceIterator::operator ++ ()
	{
		_inner_iterator++;
		return _inner_iterator != _namespace_object._child_namespaces.end();
	}

	// Namespace::NamespaceIterator::operator bool
	inline Namespace::NamespaceIterator::operator bool () const
	{
		return _inner_iterator != _namespace_object._child_namespaces.end();
	}


}
