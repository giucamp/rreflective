
#pragma once
#include "../reflective_common.h"

namespace reflective
{
	class TextOutStream
	{
	public:

		virtual ~TextOutStream() = default;

		virtual void write(const char * i_text, const size_t i_text_length) = 0;

		void write(const char * i_text)								{ write(i_text, strlen(i_text)); }
		
		TextOutStream & operator << (const char * i_text)			{ write(i_text, strlen(i_text));  return *this; }

		template <size_t ARRAY_SIZE>
			TextOutStream & operator << (const char(&literal_tring)[ARRAY_SIZE])			{ write(i_text, ARRAY_SIZE - 1);  return *this; }		
	};
}

