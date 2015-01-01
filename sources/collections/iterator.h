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
	/** Wraps an abstract iterator, providing easy construction and automatic destruction. */
	class Iterator
	{
	public:

		/** Construct an iterator for the specified object. If the object is a pointer, the pointed object is considered.
			If the object is not a collection, the iteration ends before the first step (is_end returns always false). */
		Iterator( ObjectPointerWrapper i_collection );

		/** moves the iterator to the next item */
		void move_next()									{ REFLECTIVE_ASSERT(!is_end()); m_iterator->move_next(); }

		/** postfix increment, alias for move_next */
		void operator ++ ( int )							{ REFLECTIVE_ASSERT(!is_end()); m_iterator->move_next(); }

		/** prefix increment, alias for move_next */
		Iterator & operator ++ ()							{ REFLECTIVE_ASSERT(!is_end()); m_iterator->move_next(); return *this; }

		/** returns true if there are no more items */
		bool is_end() const									{ return m_iterator != nullptr ? m_iterator->is_end() : true; }

		/** returns true if there are still items */
		operator bool () const								{ return !is_end(); }

		/** retrieves the current item. At each iteration check the result of is_end() before calling this method. */
		const ObjectPointerWrapper & curr_item() const		{ REFLECTIVE_ASSERT(!is_end()); return m_iterator->curr_item(); }

		/** retrieves the current key, or nullptr if the collection does not have a key. At each iteration check the result of is_end() before calling this method. */
		const void * key_value() const						{ REFLECTIVE_ASSERT(!is_end()); return m_iterator->key_value(); }

		~Iterator();

	private:
		Iterator( const Iterator & );
		Iterator & operator = ( const Iterator & );

	private:
		AbstractIterator * m_iterator;
	};

} // namespace reflective
