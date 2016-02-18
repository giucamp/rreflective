
namespace reflective
{
	// http://stackoverflow.com/questions/18570285/using-sfinae-to-detect-a-member-function

	/** Checks at compile time if a type has member function with this name and signature:
			void to_string(OutStringBuffer & i_dest) const;
		Usage: const bool has = has_to_string<MyClass>::value; */
	template <typename TYPE> class has_to_string
	{
	private:
		typedef char Yes;
		typedef Yes No[2];

		template <typename U, U> struct really_has;

		template<typename C> static Yes& Test(really_has <void (C::*)(OutStringBuffer & i_dest) const, &C::to_string>*);
		template<typename> static No& Test(...);

	public:
		static bool const value = sizeof(Test<TYPE>(0)) == sizeof(Yes);
	};

	/** Checks at compile time if a type has member function with this name and signature:
			bool assign_from_string(StringView & i_source, OutStringBuffer & i_error_dest);
		Usage: const bool has = has_assign_from_string<MyClass>::value; */
	template <typename TYPE> class has_assign_from_string
	{
	private:
		typedef char Yes;
		typedef Yes No[2];

		template <typename U, U> struct really_has;

		template<typename C> static Yes& Test(really_has <bool (C::*)(StringView & i_source, OutStringBuffer & i_error_dest), &C::assign_from_string>*);
		template<typename> static No& Test(...);

	public:
		static bool const value = sizeof(Test<TYPE>(0)) == sizeof(Yes);
	};
};
