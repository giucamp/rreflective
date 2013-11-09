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

#if 0

namespace reflective
{
	class Lexical
	{
	public:
				
		// the values of this enum are fixed, so it can be used to indicize an array
		enum TokenKind
		{
			eEnd = 0, // no token to read

			eUnknownChar,

			eWord, // identifiers, keywords
			
			eOperatorPlus, // +
			eOperatorMinus, // -
			eOperatorStar, // *
			eOperatorSlash, // /

			eOperatorIncrement, // ++
			eOperatorDecrement, // --

			eLeftParenthesis, // (
			eRightParenthesis, // )

			eOperatorAddAssignment, // +=
			eOperatorSubAssignment, // -=

			eOpeartorStructureDeference, // ->
			eOpeartorPointerToMemberDeference, // ->*
			eOpeartorPointerToMemberAccess, // .*

			eLiteralInt, // 4
			eLiteralFloat, // 4.f
			eLiteralChar, // 'd'
			eLiteralString // "abc"
		};

		struct TokenData
		{
			// kind of the accepted token - see TokenKind
			TokenKind m_token_kind;

			/* string assigned depending on the value of token kind. It may point to a subpart of the accepted chars 
					(like in the case of literal, constants, identifier), or it may point to a static allocated
					error message text, or may be null. */
			const uint8_t * m_text_start, * m_text_end;
		};

		// accept_token
		static size_t accept_token( 
			const uint8_t * i_text_start, const uint8_t * i_text_end,
			TokenData * o_token_data );

	private: // data members

		struct HandlingParams
		{
			TokenKind m_token_kind;
			TokenData * m_token_data;
			const uint8_t * m_text_start;
			const uint8_t * m_text_end;
		};

		static size_t _handle_word( HandlingParams & io_params );
		static size_t _handle_operator_plus( HandlingParams & io_params );
		static size_t _handle_operator_minus( HandlingParams & io_params );
		static size_t _handle_operator_star( HandlingParams & io_params );
		static size_t _handle_operator_slash( HandlingParams & io_params );
		static size_t _handle_operator( HandlingParams & io_params );
		static size_t _handle_numeric_literal( HandlingParams & io_params );
		static size_t _handle_string_literal( HandlingParams & io_params );
		static size_t _handle_char_literal( HandlingParams & io_params );
		static size_t _handle_unrecognized_char( HandlingParams & io_params );

		class CharacterTable;
	};

	
} // namespace reflective

#endif
