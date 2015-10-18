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
	class InStringBuffer
	{
	public:

		InStringBuffer(const char * i_buffer, size_t i_length);

		InStringBuffer(const char * i_null_terminated_string)
			: InStringBuffer(i_null_terminated_string, strlen(i_null_terminated_string))
				{ }

		
			// accept
		
		bool accept_char(char i_character);

		bool accept_cstr(const char * i_string, size_t i_string_length);
		
		bool accept_cstr(const char * i_null_terminated_string)
		{
			return accept_cstr(i_null_terminated_string, strlen(i_null_terminated_string));
		}		

		template <size_t ARRAY_SIZE>
		bool accept_literal(const char(&i_array)[ARRAY_SIZE])
		{
			REFLECTIVE_ASSERT(i_array[ARRAY_SIZE - 1] == 0, "the array must contain a null terminated string");
			return accept_cstr(i_array, ARRAY_SIZE - 1);
		}
			
		bool accept_char_case_ins(char i_character);

		bool accept_cstr_case_ins(const char * i_null_terminated_string)
		{
			return accept_cstr_case_ins(i_null_terminated_string, strlen(i_null_terminated_string));
		}

		bool accept_cstr_case_ins(const char * i_string, size_t i_string_length);

		template <size_t ARRAY_SIZE>
			bool accept_literal_case_ins(const char(&i_array)[ARRAY_SIZE])
		{
			REFLECTIVE_ASSERT(i_array[ARRAY_SIZE - 1] == 0, "the array must contain a null terminated string");
			return accept_cstr_case_ins(i_array, ARRAY_SIZE - 1);
		}

		bool accept_range(char i_first, char i_last);

		bool accept_whitespaces();


		//StringView read_string_until();


				// read

		template <typename TYPE>
			bool read(TYPE & o_object, OutStringBuffer i_error)
		{
			return ReadAny<TYPE, has_assign_from_string<TYPE>::value>::read(*this, i_error, o_object);
		}

		template <typename TYPE>
			bool read(TYPE & o_object)
		{
			char small_buffer[sizeof(int)];
			OutStringBuffer small_error_buffer(small_buffer);
			return ReadAny<TYPE, has_assign_from_string<TYPE>::value>::read(*this, small_error_buffer, o_object);
		}

		template <typename UNARY_FUNC>
			StringView accept_until(const UNARY_FUNC & i_unary_func);

			//	

		const char * next_char() const				{ return m_next_char; }

		const char * end_of_buffer() const			{ return m_end_of_buffer; }

		size_t remaining_buffer_length() const		{ return m_end_of_buffer - m_next_char; }

		void manual_advance(size_t i_read_length);

	private:

		template <typename TYPE, bool HAS_ASSIGN_FROM_STRING_METHOD> struct ReadAny;
		template <typename TYPE> struct ReadAny < TYPE, true >
		{
			static bool read(InStringBuffer & i_source, OutStringBuffer & i_error_dest, TYPE & o_object) 
			{
				return o_object.assign_from_string(i_source, i_error_dest);
			}
		};
		template <typename TYPE> struct ReadAny < TYPE, false >
		{
			static bool read(InStringBuffer & i_source, OutStringBuffer & i_error_dest, TYPE & o_object)
			{
				return assign_from_string(i_source, i_error_dest, o_object);
			}
		};

	private:
		const char * m_next_char;
		const char * m_end_of_buffer;
		#ifdef _DEBUG
			const char * m_dbg_buffer; /**< pointer to the beginning of the buffer (which can be nullptr). The stream does not need
									this, so it is provided only in debug.*/
		#endif
	};

	template <typename BASIC_STRING, typename CHAR, typename CHAR_TRAITS >
		inline bool starts_with(const BASIC_STRING & i_subject, BasicStringView<CHAR, CHAR_TRAITS> & i_what)
	{
		static_assert( std::is_same<BASIC_STRING::value_type, CHAR>::value &&
			std::is_same<BASIC_STRING::traits_type, CHAR_TRAITS>::value );

		auto const what_len = i_what.length();
		return i_subject.length() >= what_len &&
			CHAR_TRAITS::compare(i_subject.data(), i_what.data(), what_len) == 0;
	}

	template <typename CHAR, typename CHAR_TRAITS>
		inline bool accept(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, BasicStringView<CHAR, CHAR_TRAITS> & i_what)
	{
		if (starts_with(io_subject, i_what))
		{
			io_subject.remove_prefix(i_what.length());
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename CHAR, typename CHAR_TRAITS>
		inline bool accept(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, CHAR i_what)
	{
		if (io_subject.length() > 0 && CHAR_TRAITS::eq( io_subject[0], i_what ) )
		{
			io_subject.remove_prefix(1);
			return true;
		}
		else
		{
			return false;
		}
	}

	template <typename CHAR, typename CHAR_TRAITS, typename PREDICATE >
		BasicStringView<CHAR, CHAR_TRAITS> read_until(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, PREDICATE && i_predicate)
	{
		auto from = io_subject.data();
		size_t size = 0;

		size_t const dest_length = io_subject.length();
		while (size < dest_length && !i_predicate(io_subject[size]) )
		{
			size++;
		}

		io_subject.remove_prefix(space_count);
		return StringView(from, size);
	}

	template <typename CHAR, typename CHAR_TRAITS, typename PREDICATE >
		BasicStringView<CHAR, CHAR_TRAITS> read_until_eq(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, CHAR i_target)
	{
		auto from = io_subject.data();
		size_t size = 0;

		size_t const dest_length = io_subject.length();
		while (size < dest_length && !CHAR_TRAITS::eq( io_subject[size], i_target) )
		{
			size++;
		}

		io_subject.remove_prefix(space_count);
		return StringView(from, size);
	}

	template <typename CHAR, typename CHAR_TRAITS, typename PREDICATE >
		BasicStringView<CHAR, CHAR_TRAITS> read_while(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, PREDICATE && i_predicate)
	{
		auto from = io_subject.data();
		size_t size = 0;

		size_t const dest_length = io_subject.length();
		while (size < dest_length && i_predicate(io_subject[size]) )
		{
			size++;
		}

		io_subject.remove_prefix(space_count);
		return StringView(from, size);
	}


	template <typename CHAR, typename CHAR_TRAITS, typename PREDICATE >
		BasicStringView<CHAR, CHAR_TRAITS> read_while_eq(BasicStringView<CHAR, CHAR_TRAITS> & io_subject, CHAR i_target)
	{
		auto from = io_subject.data();
		size_t size = 0;

		size_t const dest_length = io_subject.length();
		while (size < dest_length && CHAR_TRAITS::eq( io_subject[size], i_target) )
		{
			size++;
		}

		io_subject.remove_prefix(space_count);
		return StringView(from, size);
	}

	template <typename CHAR, typename CHAR_TRAITS>
		inline size_t read_whitespaces(BasicStringView<CHAR, CHAR_TRAITS> & io_subject)
	{
		size_t space_count = 0;

		size_t const dest_length = io_subject.length();
		while (space_count < dest_length && isspace( io_subject[space_count]))
		{
			space_count++;
		}

		io_subject.remove_prefix(space_count);
		return space_count;
	}
}
