
namespace reflective
{
	#if REFLECTIVE_ENABLE_TESTING

	// insiperd by the soulution of iammilind in the question http://stackoverflow.com/questions/5839357/detect-operator-support-with-decltype-sfinae
	// but using is_same<NoSupport,decltype()> instead of sizeof.

	namespace supports {
		
		struct NoSupport {};

		template<typename T>  // used if T doesn't have "operator <" associated
			NoSupport operator << (OutBufferTextStream &, const T &);

		OutBufferTextStream & get_ou();

		template<typename T>
		const T & get_T();

		template <class T>
		struct has_stringizer {
			T t;
			static const bool value = !std::is_same< NoSupport, decltype(get_ou() << get_T<T>()) >::value; // (sizeof(t < t) != sizeof(long long));
		};
	}

	namespace testing
	{
		class A
		{
		};

		class B
		{
			B & operator ++ () { return *this; }
		};

		std::ostream & operator << (std::ostream & i_stream, A)
		{
			return i_stream;
		}

		OutBufferTextStream & operator << (OutBufferTextStream & i_stream, B)
			{ return i_stream; }

		void string_functions_test()
		{
			/*static_assert(!details::sfinae::has_stringizers<A>::has_out_to_OutStringBuffer, "");
			static_assert(details::sfinae::has_stringizers<A>::has_out_to_std_ostream, "");
			static_assert(details::sfinae::has_stringizers<B>::has_out_to_OutStringBuffer, "");
			static_assert(!details::sfinae::has_stringizers<B>::has_out_to_std_ostream, "");

			static_assert(details::sfinae::has_stringizers<int>::has_out_to_std_ostream, "");
			static_assert(details::sfinae::has_stringizers<double>::has_out_to_std_ostream, "");
			static_assert(!details::sfinae::has_stringizers<int***>::has_out_to_std_ostream, "");*/
		}
	}

	#endif
}

