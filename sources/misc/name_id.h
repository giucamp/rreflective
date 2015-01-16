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
	// hash
	template <class UINT> UINT hash( const char * string );
	template <class UINT> UINT hash( const char * string, size_t length );
	template <class UINT> UINT hash( const StaticConstString & string );
	
	template <class UINT, typename ALLOCATOR> 
		inline UINT hash( const std::basic_string< char, std::char_traits<char>, ALLOCATOR > & i_string )
			{ return hash<UINT>( i_string.c_str(), i_string.length() ); }

	template <class UINT> UINT hash( const StaticConstString & string );
	template <class UINT> UINT empty_hash();

	template <> uint32_t hash<uint32_t>( const char * string, size_t length );
	
	template <> uint32_t empty_hash<uint32_t>();
	
	/** A NameIdentifier stores a string and its hash. */
	template <class UINT, class STRING>
		class NameIdentifier
	{
	public:

		typedef UINT UIntHash;
		typedef STRING String;

		NameIdentifier();
		NameIdentifier( const char * string );
		NameIdentifier( const char * string, size_t length );
		NameIdentifier( const STRING & string );
		NameIdentifier( const NameIdentifier & name );
		NameIdentifier & operator = ( const NameIdentifier & name );

		NameIdentifier from_hash( UINT i_hash );

		// comparison
		bool operator == ( const NameIdentifier & op ) const;
		bool operator != ( const NameIdentifier & op ) const;
		bool operator < ( const NameIdentifier & op ) const;
		bool operator <= ( const NameIdentifier & op ) const;
		bool operator > ( const NameIdentifier & op ) const;
		bool operator >= ( const NameIdentifier & op ) const;

		const STRING & string() const;
		UIntHash uint_hash() const;

		bool is_empty() const;

		// to_string
		void to_string( StringOutputStream & dest_buffer ) const;
		static void to_string( StringOutputStream & dest_buffer, const Type & type, const void * object );	

		// from_string
		static bool from_string( FromStringBuffer & i_source_buffer,
			const reflective::Type & i_type, void * i_object, StringOutputStream & o_error_buffer );

		static const NameIdentifier<UINT, STRING> s_empty;

	private: // internal services
		
		#if REFLECTIVE_NAME_CONSISTENCY_CHECKS
			void _dbg_check() const;
			static void _dbg_check_pair( const NameIdentifier & first, const NameIdentifier & second );
		#endif

		enum ConstructFromHash { eConstructFromHash };
		
		NameIdentifier( ConstructFromHash, UINT i_hash )
			: m_hash( i_hash ), m_string( s_unknown_string ) {}

	private: // data members
		UINT m_hash;
		STRING m_string;

		static const STRING s_unknown_string;
	};

	template <class UINT>
		class NameIdentifier<UINT, void>
	{
	public:
		typedef UINT UIntHash;
		
		NameIdentifier();
		NameIdentifier( const char * string );
		NameIdentifier( const char * string, size_t length );
		template <class STRING> NameIdentifier( const STRING & string );
		NameIdentifier( const NameIdentifier & name );
		NameIdentifier & operator = ( const NameIdentifier & string );

		NameIdentifier from_hash( UINT i_hash );

		// comparison
		bool operator == ( const NameIdentifier & op ) const;
		bool operator != ( const NameIdentifier & op ) const;
		bool operator < ( const NameIdentifier & op ) const;
		bool operator <= ( const NameIdentifier & op ) const;
		bool operator > ( const NameIdentifier & op ) const;
		bool operator >= ( const NameIdentifier & op ) const;

		UIntHash uint_hash() const;

		bool is_empty() const;

		// to_string
		void to_string( StringOutputStream & dest_buffer ) const;
		static void to_string( StringOutputStream & dest_buffer, const Type & type, const void * object );

		// from_string
		static bool from_string( FromStringBuffer & i_source_buffer,
			const reflective::Type & i_type, void * i_object, StringOutputStream & o_error_buffer );

		static const NameIdentifier<UINT, void> s_empty;

	private: // data members
		UINT m_hash;
	};
}

namespace reflective_externals
{
	template < typename UINT, typename STRING >
		void init_type( reflective::Type * volatile * o_result,
			reflective::NameIdentifier<UINT,STRING> * null_pointer_1,
			reflective::NameIdentifier<UINT,STRING> * null_pointer_2 );

	template < typename UINT >
		void init_type( reflective::Type * volatile * o_result,
			reflective::NameIdentifier<UINT,void> * null_pointer_1,
			reflective::NameIdentifier<UINT,void> * null_pointer_2 );

} // namespace reflective_externals

