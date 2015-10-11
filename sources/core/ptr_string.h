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
	class PtrString
	{
	private:
		const char * m_chars;

	public:

		PtrString()
			: m_chars(nullptr) {}

		PtrString(const char * i_chars)
			: m_chars(i_chars) { }

		const char * data() const { return m_chars; }

		const char * c_str() const { return m_chars; }

		const char * begin() const { return m_chars; }

		const char * end() const { return m_chars + std::strlen(m_chars); }

		const char * cbegin() const { return m_chars; }

		const char * cend() const { return m_chars + std::strlen(m_chars); }

		size_t size() const { return std::strlen(m_chars);  }

		size_t length() const { return std::strlen(m_chars); }
	};

	inline std::ostream & operator << (std::ostream & i_dest, const PtrString & i_string)
	{
		i_dest.write(i_string.data(), i_string.length());
		return i_dest;
	}

	inline OutStringBuffer & operator << (OutStringBuffer & i_dest, const PtrString & i_string)
	{
		i_dest.write_nstr(i_string.data(), i_string.length());
		return i_dest;
	}

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

		/*template <size_t SIZE>
			ArrayView(TYPE(&i_objects)[SIZE])
				: m_objects(i_objects), m_size(SIZE)
		{
		}*/

		ArrayView(std::initializer_list< typename std::remove_const< TYPE>::type > i_initializer_list)
			: m_objects(i_initializer_list.begin()), m_size(i_initializer_list.end() - i_initializer_list.begin())
				{ }

		TYPE * data() const { return m_objects; }

		TYPE * begin() const { return m_objects; }

		TYPE * end() const { return m_objects + m_size; }

		TYPE * cbegin() const { return m_objects; }

		TYPE * cend() const { return m_objects + m_size; }

		TYPE & operator [] (size_t i_index)
		{
			REFLECTIVE_ASSERT(i_index < m_size, "Index out of bounds"); 
			return m_objects[i_index]; 
		}

		const TYPE & operator [] (size_t i_index) const
		{
			REFLECTIVE_ASSERT(i_index < m_size, "Index out of bounds");
			return m_objects[i_index];
		}

		size_t size() const { return m_size; }

	private:
		TYPE * m_objects;
		size_t m_size;
	};

	class StringView
	{
	public:

		StringView()
			: m_chars(nullptr), m_size(0)
				{ }

		StringView(const char * i_c_string)
			: m_chars(i_c_string), m_size( strlen(i_c_string) ) 
				{ }

		StringView(const char * i_c_string, size_t i_size)
			: m_chars(i_c_string), m_size(i_size)
				{ }

		const char * data() const { return m_chars; }

		const char * begin() const { return m_chars; }

		const char * end() const { return m_chars + m_size; }

		const char * cbegin() const { return m_chars; }

		const char * cend() const { return m_chars + m_size; }
		
		size_t size() const { return m_size; }

		size_t length() const { return m_size; }

	private:
		const char * m_chars;
		size_t m_size;
	};

	inline std::ostream & operator << (std::ostream & i_dest, const StringView & i_string)
	{
		i_dest.write(i_string.data(), i_string.length());
		return i_dest;
	}

	inline OutStringBuffer & operator << (OutStringBuffer & i_dest, const StringView & i_string)
	{
		i_dest.write_nstr(i_string.data(), i_string.length());
		return i_dest;
	}
}
