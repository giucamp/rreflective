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
	// IIterator
	class IIterator
	{
	public:		

		struct Group
		{
			QualifiedType qualified_type;
			void * curr_in_group;
			void * end_of_group;
		};

		struct ConstGroup
		{
			QualifiedType qualified_type;
			const void * curr_in_group;
			const void * end_of_group;
		};

		virtual void first_group( Group & out_group ) = 0;
		virtual void next_group( Group & out_group ) = 0;

		virtual void first_group( ConstGroup & out_group ) = 0;
		virtual void next_group( ConstGroup & out_group ) = 0;

		virtual bool insert( 
			size_t group_offset_index, const Type & items_type, 
			const void * source_object, size_t item_count,
			const void * i_key_value, Group & out_curr_group ) = 0;

		virtual bool remove( size_t offset_index, size_t item_count, Group & out_curr_group ) = 0;

		virtual const void * get_key_value( size_t group_offset_index ) = 0;

		virtual ~IIterator();

	protected:
		IIterator();
	};


} // namespace reflective
