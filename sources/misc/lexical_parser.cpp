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

#if 0

namespace reflective
{
	// Lexical::CharacterTable
	class Lexical::CharacterTable
	{
	public:

		static const CharacterTable & get_instance()
		{
			static const CharacterTable instance;
			return instance;
		}

		typedef size_t (*HandlerFunction)( HandlingParams & io_params );

		struct CharEntry
		{
			TokenKind kind;
			HandlerFunction handler_function;
		};

	private: // data members
		static const size_t s_entry_count = 255;
		CharEntry _char_entries[ s_entry_count ];

	private:
		void _register_character( uint8_t i_character, TokenKind i_token_kind, HandlerFunction i_handler_function )
		{
			REFLECTIVE_ASSERT( i_character < s_entry_count );
			_char_entries[ i_character ].kind = i_token_kind;
			_char_entries[ i_character ].handler_function = i_handler_function;
		}

	public:

		CharacterTable()
		{
			// init all the chars with eUnknownChar and _handle_unrecognized_char
			for( size_t character = 0; character <= s_entry_count; character++ )
				_register_character( static_cast<uint8_t>( character ), eUnknownChar, &_handle_unrecognized_char );

			// letters
			for( uint8_t character = 'A'; character <= 'Z'; character++ )
				_register_character( character, eWord, &_handle_word );
			for( uint8_t character = 'a'; character <= 'z'; character++ )
				_register_character( character, eWord, &_handle_word );
			_register_character( '_', eWord, &_handle_word );

			// operators
			_register_character( '+', eOperatorPlus, &_handle_operator_plus );
			_register_character( '-', eOperatorMinus, &_handle_operator_minus );
			_register_character( '*', eOperatorStar, &_handle_operator );
			_register_character( '/', eOperatorSlash, &_handle_operator );

			// numeric literals
			for( uint8_t character = '0'; character <= '0'; character++ )
				_register_character( character, eUnknownChar, &_handle_numeric_literal );

			// char literal
			_register_character( '\'', eLiteralChar, &_handle_char_literal );			

			// string literal
			_register_character( '\'', eLiteralString, &_handle_string_literal );
		}

		const CharEntry & get_entry( uint8_t i_character ) const
		{
			REFLECTIVE_ASSERT( i_character < s_entry_count );
			return _char_entries[ i_character ];
		}
	};


	// Lexical::accept_token
	size_t Lexical::accept_token( 
			const uint8_t * i_text_start, const uint8_t * i_text_end,
			TokenData * o_token_data )
	{
		// jump white chars
		for(;;)
		{
			if( i_text_start == i_text_end )
			{
				return eEnd;
			}

			if( !isspace( *i_text_start ) != 0 )
				break;

			++i_text_start;
		}

		const CharacterTable::CharEntry & entry = CharacterTable::get_instance().get_entry( *i_text_start );		

		HandlingParams params;
		params.m_token_kind = entry.kind;
		params.m_text_start = i_text_start;
		params.m_text_end = i_text_end;
		params.m_token_data = o_token_data;
		o_token_data->m_text_start = i_text_start;
		const size_t accepted_chars = (*entry.handler_function)( params );

		return accepted_chars;
	}

	// Lexical::_handle_word
	size_t Lexical::_handle_word( HandlingParams & io_params )
	{
		const uint8_t * curr_char = io_params.m_text_start;
		
		// loop valid chars
		REFLECTIVE_ASSERT( isalpha( *curr_char ) != 0 || *curr_char == '_' );
		do {
			curr_char++;
			if( curr_char >= io_params.m_text_end )
				break;

		} while( isalnum( *curr_char ) != 0 || *curr_char == '_' );

		io_params.m_token_data->m_text_end = curr_char;

		return curr_char - io_params.m_text_start;
	}

	// Lexical::_handle_operator_plus
	size_t Lexical::_handle_operator_plus( HandlingParams & io_params )
	{
		REFLECTIVE_ASSERT( *io_params.m_text_start == '+' );

		const uint8_t * next_char = io_params.m_text_start + 1;
		if( next_char < io_params.m_text_end )
		{
			switch( *next_char )
			{
			case '+': // recognize ++
				io_params.m_token_data->m_token_kind = eOperatorIncrement;
				io_params.m_token_data->m_text_end = next_char + 1;
				return 2;

			case '=': // recognize +=
				io_params.m_token_data->m_token_kind = eOperatorAddAssignment;
				io_params.m_token_data->m_text_end = next_char + 1;
				return 2;
			}
		}

		io_params.m_token_data->m_text_end = next_char;
		return 1;
	}
	
	// Lexical::_handle_operator_minus
	size_t Lexical::_handle_operator_minus( HandlingParams & io_params )
	{
		REFLECTIVE_ASSERT( *io_params.m_text_start == '-' );
		const uint8_t * next_char = io_params.m_text_start + 1;
		if( next_char < io_params.m_text_end )
		{
			switch( *next_char )
			{
			case '-': // recognize --
				io_params.m_token_kind = eOperatorDecrement;
				io_params.m_token_data->m_text_end = next_char + 1;
				return 2;

			case '=': // recognize -=
				io_params.m_token_kind = eOperatorSubAssignment;
				io_params.m_token_data->m_text_end = next_char + 1;
				return 2;

			case '>': 
				{
					const uint8_t * next_next_char = next_char + 1;
					if( next_next_char < io_params.m_text_end && *next_next_char == '*' )
					{
						// recognize ->*
						io_params.m_token_kind = eOpeartorPointerToMemberDeference;
						io_params.m_token_data->m_text_end = next_next_char + 1;
						return 3;
					}
					else
					{
						// recognize ->
						io_params.m_token_kind = eOpeartorStructureDeference;
						io_params.m_token_data->m_text_end = next_char + 1;
						return 2;
					}
				}
			}
		}

		return 1;
	}

	size_t Lexical::_handle_numeric_literal( HandlingParams & io_params )
	{
		return 0;
	}

} // namespace reflective

#endif


