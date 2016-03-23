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
	/** This header provides a temporary, not fully not compliant implementation of some classes which are not 
		in C++14, but should be included in the C++17. To avoid confusion with the standard, the names defined
		are all camel case or inside the static class reflective::Ext, so that in the future they may become
		aliases of the standard names. */

	template <typename TYPE>
		class ArrayView
	{
	public:

		ArrayView()
			: m_objects(nullptr), m_size(0)
		{
		}

		ArrayView(TYPE * i_objects, size_t i_size)
			: m_objects(i_objects), m_size(i_size)
		{
		}

		template <size_t SIZE>
			ArrayView(TYPE(&i_objects)[SIZE])
				: m_objects(i_objects), m_size(SIZE)
		{
		}

		ArrayView(std::initializer_list<TYPE> i_initializer_list)
			: m_objects(i_initializer_list.begin()), m_size(i_initializer_list.end() - i_initializer_list.begin())
				{ }

		TYPE * data() const { return m_objects; }

		TYPE * begin() const { return m_objects; }

		TYPE * end() const { return m_objects + m_size; }

		TYPE * cbegin() const { return m_objects; }

		TYPE * cend() const { return m_objects + m_size; }

		TYPE & operator [] (size_t i_index)
		{
			assert(i_index < m_size);
			return m_objects[i_index];
		}

		const TYPE & operator [] (size_t i_index) const
		{
			assert(i_index < m_size);
			return m_objects[i_index];
		}

		size_t size() const { return m_size; }

	private:
		TYPE * m_objects;
		size_t m_size;
	};

	// void_t invented by Walter E. Brown - it will be available is std from C++17 on
	// comented out: Visual Studio does not support void_t until 2015 with Update 1
	/*namespace details
	{
		template<typename... Ts> struct MakeVoid { typedef void type; };
	}	
	template<typename... Ts> using VoidT = typename details::MakeVoid<Ts...>::type;*/

	class Ext
	{
	public:
		
		template <typename CONTAINER, typename ELEMENT>
			static auto find(CONTAINER & i_container, ELEMENT && i_value) -> decltype(std::find(i_container.begin(), i_container.end(), i_value))
		{
			return std::find(i_container.begin(), i_container.end(), i_value);
		}

		template <typename CONTAINER, typename PREDICATE>
			static auto find_if(CONTAINER & i_container, PREDICATE && i_predicate) -> decltype(std::find_if(i_container.begin(), i_container.end(), i_predicate))
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate);
		}

		template <typename CONTAINER, typename ELEMENT>
			static bool contains(CONTAINER & i_container, ELEMENT && i_value)
		{
			return std::find(i_container.begin(), i_container.end(), i_value) != i_container.end();
		}

		template <typename CONTAINER, typename PREDICATE>
			static bool contains_if(CONTAINER & i_container, PREDICATE && i_predicate)
		{
			return std::find_if(i_container.begin(), i_container.end(), i_predicate) != i_container.end();
		}
	};
}
