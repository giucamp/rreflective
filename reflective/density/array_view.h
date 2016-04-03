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

namespace reflective
{	
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
}
