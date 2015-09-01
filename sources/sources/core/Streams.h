
#pragma once
#include "../reflective_common.h"

namespace reflective
{
	/** This class implements an output text stream. The buffer to be written is provided by the user of the class.  */
	class TextOutStream
	{
	public:

		TextOutStream();

		TextOutStream(char * i_buffer, size_t i_buffer_size);
						
		void write(const char * i_string, const size_t i_string_length);

		void write(const char * i_text)								{ write(i_text, strlen(i_text)); }
		
		TextOutStream & operator << (const char * i_text)			{ write(i_text, strlen(i_text));  return *this; }

		template <size_t ARRAY_SIZE>
			TextOutStream & operator << (const char(&literal_tring)[ARRAY_SIZE])			{ write(i_text, ARRAY_SIZE - 1);  return *this; }

	private:
		char * m_next_char_to_write;
		char * m_end_of_buffer;
		size_t m_needed_length; /**< chars written to the stream, interdependently from the actual buffer length */
		#ifdef _DEBUG
			char * m_dbg_buffer; /**< pointer to the beginning of the buffer (which can be nullptr). The stream does not need
									 this, so it is provided only in debug.*/
		#endif
	};
}
