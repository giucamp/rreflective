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

#pragma once
#ifndef INCLUDING_REFLECTIVE
	#error "cant't include this header directly, include reflective.h instead"
#endif

namespace reflective
{
	template < typename HASHER, typename STRING >
		class Identifier
	{
	public:

		using Hasher = typename HASHER;
		using String = typename STRING;
		using HashType = typename HASHER::ResultType;

				// constructors

		/** default constructor */
		Identifier() 
			: m_hash(Hasher::s_empty_hash)
		{

		}

		Identifier(const char * i_null_terminated_string)
			: m_hash(Hasher()(i_null_terminated_string)), m_string(i_null_terminated_string)
		{

		}

		Identifier(StringView i_string)
			: m_hash(Hasher()(i_string)), m_string(i_string)
		{

		}


		template < typename OTHER_STRING >
			Identifier(const Identifier<HASHER, OTHER_STRING> & i_source )
				: m_hash(i_source.hash()), m_string(i_source.string())
		{

		}
		

				// getters

		HashType hash() const
		{
			return m_hash;
		}

		const String & string() const
		{
			return m_string;
		}


					// comparison

		bool operator == (const Identifier & i_source) const
		{
			return m_hash == i_source.m_hash;
		}

		bool operator < (const Identifier & i_source) const
		{
			return m_hash < i_source.m_hash;
		}

		bool operator <= (const Identifier & i_source) const
		{
			return m_hash <= i_source.m_hash;
		}

		bool operator > (const Identifier & i_source) const
		{
			return m_hash > i_source.m_hash;
		}

		bool operator >= (const Identifier & i_source) const
		{
			return m_hash >= i_source.m_hash;
		}

	private: // data members
		HashType m_hash;
		String m_string;
	};

	template < typename HASHER >
		class Identifier <HASHER, void >
	{
	public:

		using Hasher = typename HASHER;
		using String = void;
		using HashType = typename HASHER::ResultType;


				// constructors

		/** default constructor */
		Identifier() 
			: m_hash(Hasher::s_empty_hash)
		{

		}

		Identifier(const char * i_null_terminated_string)
			: m_hash(Hasher()(i_null_terminated_string))
		{

		}

		Identifier(const char * i_string, size_t i_length)
			: m_hash(Hasher()(i_string, i_length))
		{

		}

		template < typename OTHER_STRING >
			Identifier(const Identifier<HASHER, OTHER_STRING> & i_source)
				: m_hash(i_source.hash())
		{

		}
		
					// getters

		HashType hash() const
		{
			return m_hash;
		}


					// comparison

		bool operator == (const Identifier & i_source) const
		{
			return m_hash == i_source.m_hash;
		}

		bool operator < (const Identifier & i_source) const
		{
			return m_hash < i_source.m_hash;
		}

		bool operator <= (const Identifier & i_source) const
		{
			return m_hash <= i_source.m_hash;
		}

		bool operator > (const Identifier & i_source) const
		{
			return m_hash > i_source.m_hash;
		}

		bool operator >= (const Identifier & i_source) const
		{
			return m_hash >= i_source.m_hash;
		}
		
	private: // data members
		HashType m_hash;
	};

	template < typename HASHER, typename STRING >
		inline std::ostream & operator << (std::ostream & i_dest, const Identifier<HASHER, STRING> & i_identifier)
	{
		const auto & string = i_identifier.string();
		i_dest.write(string.data(), string.length());
		return i_dest;
	}

	template < typename HASHER, typename STRING >
		inline OutStringBuffer & operator << (OutStringBuffer & i_dest, const Identifier<HASHER, STRING> & i_identifier)
	{
		const auto & string = i_identifier.string();
		i_dest.write_nstr(string.data(), string.length());
		return i_dest;
	}

	template < typename HASHER >
		inline std::ostream & operator << (std::ostream & i_dest, const Identifier<HASHER, void> & i_identifier)
	{
		i_dest << '%' << i_identifier.hash();
		return i_dest;
	}

	template < typename HASHER >
		inline OutStringBuffer & operator << (OutStringBuffer & i_dest, const Identifier<HASHER, void> & i_identifier)
	{
		i_dest << '%' << i_identifier.hash();
		return i_dest;
	}
}

