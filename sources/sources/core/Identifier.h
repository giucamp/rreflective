
#pragma once
#include "../reflective_common.h"
#include "StringHash.h"

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

		/** forwarding constructor */
		template<typename FIRST_PARAMETER, typename... OTHER_PARAMETERS>
			Identifier(FIRST_PARAMETER && i_first_parameter, OTHER_PARAMETERS &&... i_other_parameters)
				: m_string(i_first_parameter, i_other_parameters...)
		{
			m_hash = Hasher()(std::forward<FIRST_PARAMETER>(i_first_parameter), std::forward<OTHER_PARAMETERS>(i_other_parameters)...);
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

		/** forwarding constructor */
		template<typename FIRST_PARAMETER, typename... OTHER_PARAMETERS>
			Identifier(FIRST_PARAMETER && i_first_parameter, OTHER_PARAMETERS &&... i_other_parameters)
				: m_hash(Hasher()(std::forward<FIRST_PARAMETER>(i_first_parameter), std::forward<OTHER_PARAMETERS>(i_other_parameters)...))
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
		
	private: // data members
		HashType m_hash;
	};
}

