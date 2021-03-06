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
	/** Abstract class that handles the iteration over a collection. See CollectionHandler.
		Usage:
		for( AbstractIterator * iter = collection_handler->create_iterator( ... ); !it->is_end(); it->move_next() )
		{
			// ...
		}
	*/
	class AbstractIterator
	{
	public:		

		/** moves the iterator to the next item */
		virtual void move_next() = 0;

		/** postfix increment, alias for move_next */
		void operator ++ ( int )							{ REFLECTIVE_ASSERT(!is_end()); move_next(); }

		/** prefix increment, alias for move_next */
		AbstractIterator & operator ++ ()					{ REFLECTIVE_ASSERT(!is_end()); move_next(); return *this; }

		/** returns true if there are no more items */
		bool is_end() const									{ return m_curr_item.object() == nullptr; }

		/** returns true if there are still items */
		operator bool () const								{ return !is_end(); }

		/** retrieves the current item. At each iteration check the result of is_end() before calling this method. */
		const ObjectPointerWrapper & curr_item() const		{ REFLECTIVE_ASSERT(!is_end()); return m_curr_item; }

		/** retrieves the current key, or nullptr if the collection does not have a key. At each iteration check the result of is_end() before calling this method. */
		const void * key_value() const						{ REFLECTIVE_ASSERT(!is_end());return m_key; }

		virtual ~AbstractIterator() { }

	protected:
		
		AbstractIterator()									: m_key( nullptr ) { }
		
		void set_item( const ObjectPointerWrapper & i_curr_item, const void * i_key )
		{
			m_curr_item = i_curr_item;
			m_key = i_key;
		}

		void set_item( void * i_curr_item, const QualifiedType & i_type, const void * i_key )
		{
			m_curr_item = ObjectPointerWrapper( i_curr_item, i_type );
			m_key = i_key;
		}

		void set_item_null()
		{
			m_curr_item = ObjectPointerWrapper();
			m_key = nullptr;
		}

	private:
		ObjectPointerWrapper m_curr_item;
		const void * m_key;
	};

} // namespace reflective
