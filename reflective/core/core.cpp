

#ifndef REFLECTIVE_ENABLE_TESTING
	#error REFLECTIVE_ENABLE_TESTING must be defined
#endif
#if REFLECTIVE_ENABLE_TESTING
	#include "testing\bulk_list_test.cpp"
	#include "testing\out_buffer_text_stream.cpp"
	#include "testing\in_buffer_text_stream.cpp"
#endif
