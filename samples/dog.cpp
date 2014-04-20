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

#include "sample_common.h"

namespace Animals
{
	class Dog
	{	
		float m_size;

	public:

		Dog() : m_size( 1.f ) { }

		void Bau( float volume, Dog * * target_dogs )
		{
			printf( "Bau %f\n", volume );
		}
	};
}




const reflective::Type * GetDogType()
{
	using namespace reflective;

	StaticConstString path( "Animals::Dog" );
	const Type * dog_type = Namespace::global().find_child_type( path );
	return dog_type;
}

template <typename TYPE>
	void test_qualified_type()
{
	using namespace reflective;

	char str[ 1024 ], err[1024];
	StringOutputStream str_buff( str, 1024 );
	StringOutputStream err_buff( err, 1024 );

	QualifiedType qual_type = reflective::get_qualified_type<TYPE>();
	to_string( str_buff, qual_type );
	printf( "%s\n", str_buff.buffer() );
	printf( "%s\n", typeid(TYPE).name() );
	
	QualifiedType qual1;
	FromStringBuffer ibuff( str, strlen(str) );
	assign_from_string( qual1, ibuff, err_buff );

	if( qual1 != qual_type )
	{
		printf( "DIFFF %s\n", str_buff.buffer() );
	}

	printf( "---------------------\n" );
}

int main()
{
	using namespace reflective;
	using namespace Animals;

	// get the type using get_type<T>() global function
	const Type & animal_type = get_type< Dog >();

	// get some info
	const SymbolName class_name = animal_type.name();
	const size_t class_size = animal_type.size();
	const SymbolName namespace_name = animal_type.parent_namespace().name();
	const SymbolName outer_namespace_name = animal_type.parent_namespace().parent()->name();
	const Type::Capabilities capabilities = animal_type.capabilities(); 
		// Type::Capabilities is an enum with OR support

	// create a StringOutputStream on the stack
	char chars[ 1024 ];
	StringOutputStream char_buffer( chars, 1024 );	
	
	// write some info on the buffer
	to_string( char_buffer, class_name ); char_buffer.append_literal( ", " );
	to_string( char_buffer, class_size ); char_buffer.append_literal( ", " );
	to_string( char_buffer, namespace_name ); char_buffer.append_literal( ", " );
	to_string( char_buffer, outer_namespace_name ); char_buffer.append_literal( ", " );
	to_string( char_buffer, capabilities ); char_buffer.append_literal( "\n" );

	// print on the ouput the text
	printf( chars );

	const reflective::Type * dog_type2 = GetDogType();

	typedef float DogSize;

	Dog boby;
	DogSize size_of_boby = 5;
	const Type & dog_type = type_of( boby );
	const Type & height_type = type_of( size_of_boby );

	const bool is_class = is_instance_of<Class>( dog_type );
	const bool is_enum = type_of( dog_type ) == get_type<Enum>();

	test_qualified_type<int>();
	test_qualified_type<const int>();
	test_qualified_type<int*>();
	test_qualified_type<int***>();
	test_qualified_type<int*const**>();
	test_qualified_type<int const*const**>();
	test_qualified_type<const int*const*const* const>();
	test_qualified_type<const Type ********>();


	return 0;
}

