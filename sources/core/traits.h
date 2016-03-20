
namespace reflective
{
	namespace details
	{
		/** HasSetupClassStaticFunc< TYPE >::value is non-zero if the type has a static function with the signature:
				static void setup_class( reflective::Class & class_obj );
			implementation - SFINAE - see http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error */
		struct ___HasSetupClassStaticFunc
		{
			struct ___true { static const int val = 1; };
			struct ___false { static const int val = 0; };
		
			template <class TYPE>
				static ___true func( int, decltype( TYPE::setup_class( *(Class*)0 ) ) par = 0 );

			template <class TYPE>
				static ___false func( ... );
		};			
		template <class TYPE>
			struct HasSetupClassStaticFunc : ___HasSetupClassStaticFunc
		{
			typedef decltype( func<TYPE>( 0 ) ) B;
			static const int value = B::val;
		};
	}
	
};
