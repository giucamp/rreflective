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
	// ArrayType::constructor
	inline ArrayType::ArrayType( const QualifiedType & type, size_t count )
		: Type( Namespace::edit_global(), _construct_name( type, count ), type.type()->size() * count, type.type()->alignment() ),
		  _type( type ), _count( count )
	{
	}

	// ArrayType::type
	inline const QualifiedType & ArrayType::type() const
	{
		return _type;
	}

	// ArrayType::count
	inline size_t ArrayType::count() const
	{
		return _count;
	}

} // namespace reflective


namespace reflective_externals
{
	template < class TYPE, size_t SIZE >
		inline reflective::Type * init_type(
			TYPE (* null_pointer_1)[ SIZE ],
			TYPE (* null_pointer_2)[ SIZE ] )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		static ArrayType * result = null;
		if( result )
			return result;

		mem::AbstractLifoAllocator & lifo_allocator = reflective_externals::lifo_allocator();

		void * allocation = lifo_allocator.allocate( alignment_of( ArrayType ), sizeof( ArrayType ) );

		result = new( allocation ) ArrayType( safe_get_qualified_type<TYPE>(), SIZE );

		return result;
	}

} // namespace reflective_externals
