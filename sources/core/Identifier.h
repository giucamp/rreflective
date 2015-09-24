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

		/** copy constructor */
		Identifier(const Identifier & i_source)
			: m_hash(i_source.m_hash),
			  m_string(i_source.m_string)
		{

		}

		/** move constructor */
		Identifier(Identifier && i_from)
			: m_hash(std::move(i_from.m_hash)),
			  m_string(std::move(i_from.m_string))
		{

		}

		struct Make {};

		/** forwarding constructor */
		template<typename... PARAMETERS>
			Identifier(Make i_make, PARAMETERS &&... i_parameters)
				: m_string(i_parameters...)
		{
			m_hash = Hasher()(std::forward<PARAMETERS>(i_parameters)...);
		}

		template<typename FIRST_PARAMETER>
			Identifier(const FIRST_PARAMETER & i_first_parameter)
				: m_string(i_first_parameter)
		{
			m_hash = Hasher()(i_first_parameter);
		}


					// assignments

		/** copy assignment */
		Identifier & operator = (const Identifier & i_source)
		{
			m_hash = i_source.m_hash;
			m_string = i_source.m_string;
			return *this;
		}

		/** move assignment */
		Identifier & operator = (Identifier && i_from)
		{
			m_hash = std::move(i_from.m_hash);
			m_string = std::move(i_from.m_string);
			return *this;
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

		template <typename OUT_STREAM>
			void to_string(OUT_STREAM & i_dest) const
		{
			i_dest << m_string;
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

		/** copy constructor */
		Identifier(const Identifier & i_source)
			: m_hash(i_source.m_hash)
		{

		}

		/** move constructor */
		Identifier(Identifier && i_from)
			: m_hash(std::move(i_from.m_hash))
		{

		}

		struct Make {};

		/** forwarding constructor */
		template<typename... PARAMETERS>
			Identifier(Make i_make, PARAMETERS &&... i_parameters)
				: m_hash(Hasher()(std::forward<PARAMETERS>(i_parameters)...))
		{
			
		}


					// assignments

		/** copy assignment */
		Identifier & operator = (const Identifier & i_source)
		{
			m_hash = i_source.m_hash;
			return *this;
		}

		/** move assignment */
		Identifier & operator = (Identifier && i_from)
		{
			m_hash = std::move(i_from.m_hash);
			return *this;
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

		template <typename OUT_STREAM>
			void to_string(OUT_STREAM & i_dest) const
		{
			i_dest << '%' << m_hash;
		}

	private: // data members
		HashType m_hash;
	};
}

