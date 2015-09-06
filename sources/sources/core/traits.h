

#pragma once

namespace reflective
{
	class TextOutBuffer;

	// http://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function

	template <typename TYPE> class has_to_string
	{
	private:
		typedef char Yes;
		typedef Yes No[2];

		template <typename U, U> struct really_has;

		template<typename C> static Yes& Test(really_has <void (C::*)(TextOutBuffer & i_dest) const, &C::to_string>*);
		template<typename> static No& Test(...);

	public:
		static bool const value = sizeof(Test<TYPE>(0)) == sizeof(Yes);
	};
};
