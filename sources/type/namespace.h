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
	// Namespace
	class Namespace : public Symbol
	{
	public:

		Namespace( const SymbolName & name );
		~Namespace();

		// global
		static const Namespace & global();
		static Namespace & edit_global();

		// counts
		size_t type_count() const;
		size_t namespace_count() const;

		// find by SymbolName
		const Type * find_child_type( SymbolName name ) const;
		const Namespace * find_child_namespace( SymbolName name ) const;	

		// find by SymbolNameHash
		const Type * find_child_type( SymbolNameHash hash ) const;
		const Namespace * find_child_namespace( SymbolNameHash hash ) const;

		// find by path
		const Type * find_child_type( const StaticConstString & path ) const;
		const Namespace * find_child_namespace( const StaticConstString & path ) const;

		// non const
		Namespace * find_child_namespace( SymbolName name );
		Namespace * find_child_namespace( const StaticConstString & path );		
		Namespace * find_or_add_child_namespace( SymbolName name );
		Namespace * find_or_add_child_namespace( const StaticConstString & path );

		// parent
		Namespace * parent();
		const Namespace * parent() const;

		// adds
		void add_child_type( const Type & type );
		void remove_child_type( const Type & type );
		void add_child_namespace( Namespace & namespace_obj );

		// TypeIterator
		class TypeIterator
		{
		public:
			TypeIterator( const Namespace & namespace_object );

			const Type & operator * () const;
			const Type * operator -> () const;

			bool operator ++ ();
			operator bool () const;

		private:
			TypeIterator & operator = ( const TypeIterator & ); // not supported

		private: // data members
			const Namespace & _namespace_object;
			std::map< SymbolNameHash, const Type * >::const_iterator _inner_iterator;
		};

		// NamespaceIterator
		class NamespaceIterator
		{
		public:
			NamespaceIterator( const Namespace & namespace_object );

			const Namespace & operator * () const;
			const Namespace * operator -> () const;

			bool operator ++ ();
			operator bool () const;

		private:
			NamespaceIterator & operator = ( const NamespaceIterator & ); // not supported

		private: // data members
			const Namespace & _namespace_object;
			std::map< SymbolNameHash, Namespace * >::const_iterator _inner_iterator;
		};		

		const std::map< SymbolNameHash, const Type * > & child_types() const;
		const std::map< SymbolNameHash, Namespace * > & child_namespaces() const;

		// collection handler
		class ICollectionHandler;
		class ReflectiveIterator;
		class ReflectiveWatch;
		static ICollectionHandler & access_collection_handler();

	private: // internal services		
		friend class Type;
		static Namespace * _create_namespace( SymbolName name );
		static const char * _find_double_colon( const char * src, size_t src_len );

	private: // data members
		std::map< SymbolNameHash, const Type * > _child_types;
		std::map< SymbolNameHash, Namespace * > _child_namespaces;
		Namespace * _parent;
		static ICollectionHandler _collection_handler;
	};

}

namespace reflective_externals
{
	// reflection of reflective::Namespace
	void init_type( reflective::Type * volatile * o_result,
		reflective::Namespace * null_pointer_1,
		reflective::Namespace * null_pointer_2 );	

} // namespace reflective_externals
