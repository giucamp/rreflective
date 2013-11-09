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

#ifndef _IMPLEMENTING_REFLECTIVE
	#error compile "reflective.cpp" instead of this file
#endif

namespace reflective
{
	// Symbol::symbol_types
	const Type * Symbol::symbol_types[ eMaxTypeIdCount ];

	#if REFLECTIVE_ENABLE_ASSERT
		void Symbol::dbg_check_sumbol_name( const StaticConstString & name )
		{
			const char * const name_str = name.start_of_chars();
			const char * const name_end = name_str + name.length();

			const char * curr_char = name_str;
			while( curr_char < name_end )
			{
				if( *curr_char == '[' )
					return;
				curr_char++;
			}

			curr_char = name_str;
			int curly_bracket_depth = 0;
			//bool squared_bracket = false;
			while( curr_char < name_end )
			{
				if( *curr_char == '<' )
					curly_bracket_depth++;
				else if( *curr_char == '>' )
					curly_bracket_depth--;
				else if( *curr_char == 0 )
				{
					REFLECTIVE_ASSERT( curly_bracket_depth == 0 );
					REFLECTIVE_ASSERT( curr_char + 1 == name_end );
				}
				/*else if( *curr_char == '[' )
				{
					REFLECTIVE_ASSERT( !squared_bracket );
					squared_bracket = true;
				}
				else if( *curr_char == ']' )
				{
					REFLECTIVE_ASSERT( squared_bracket );
					squared_bracket = true;
				}
				else if( squared_bracket )
				{
					REFLECTIVE_ASSERT( *curr_char == ' ' || 
						(*curr_char >= '0' && *curr_char >= '9') );
				}*/
				else if( curly_bracket_depth == 0 )
				{
					REFLECTIVE_ASSERT( isalnum( *curr_char ) != 0 || *curr_char == '_' || *curr_char == ' ' );
				}
				curr_char++;
			}
			REFLECTIVE_ASSERT( curly_bracket_depth == 0 );
			//REFLECTIVE_ASSERT( !squared_bracket );
		}
	#endif

	// Symbol::resolve_type
	const Type & Symbol::resolve_type( const Type & base_type, const void * object )
	{
		REFLECTIVE_ASSERT( base_type >= get_type<Symbol>() );
		REFLECTIVE_UNUSED( base_type );

		static bool types_set = false;
		if( !types_set )
		{
			types_set = true;
			symbol_types[ eTypeSymbol ]			= &safe_get_type<Symbol>();
			symbol_types[ eTypeNamespace ]		= &safe_get_type<Namespace>();
			symbol_types[ eTypeType ]			= &safe_get_type<Type>();
			symbol_types[ eTypeClass ]			= &safe_get_type<Class>();
			symbol_types[ eTypePrimitiveType ]	= &safe_get_type<PrimitiveType>();
			symbol_types[ eTypeEnum ]			= &safe_get_type<Enum>();
			symbol_types[ eTypeEnumMember ]		= &safe_get_type<Enum::Member>();
			symbol_types[ eTypeProperty ]		= &safe_get_type<Property>();
			symbol_types[ eTypeEvent ]			= null;//&safe_get_type<AbstractEvent>();
			symbol_types[ eTypeAction ]			= &safe_get_type<Action>();
			symbol_types[ eTypeParameter ]		= &safe_get_type<Parameter>();
			symbol_types[ eTypeClassTemplate ]	= &safe_get_type<ClassTemplate>();
			REFLECTIVE_COMPILETIME_ASSERT( eTypeClassTemplate + 1 == eFirstUserTypeId );
		}

		const Symbol & symbol = *static_cast<const Symbol*>( object );
		REFLECTIVE_ASSERT( symbol._type_id < eMaxTypeIdCount );
		REFLECTIVE_ASSERT( symbol_types[ symbol._type_id ] != null );
		return *symbol_types[ symbol._type_id ];
	}

	// Symbol::query_custom_type_id
	int Symbol::query_custom_type_id()
	{
		static int next_user_id = eFirstUserTypeId;
		if( next_user_id >= eMaxTypeIdCount )
			return -1;
		return next_user_id++;
	}
}


namespace reflective_externals
{
	// reflection of reflective::Symbol
	reflective::Class * init_type(
		reflective::Symbol * null_pointer_1,
		reflective::Symbol * null_pointer_2 )
	{
		REFLECTIVE_UNUSED_2( null_pointer_1, null_pointer_2 );

		using namespace ::reflective;
		typedef reflective::Symbol ThisClass;
		
		static Class * result = null;
		if( result != null )
			return result;

		Class * class_object = new_class<ThisClass>( "reflective", "Symbol" );
		result = class_object;		

		class_object->set_type_resolver( Symbol::resolve_type );
		
		// properties
		const Property * properties[] = 
		{
			new_property<ThisClass>( "name", &ThisClass::name, ClassMember::IDENTIFIER )
		};
		
		// assign members
		class_object->assign_properties( properties );		
		
		// return type		
		return class_object;
	}
}
