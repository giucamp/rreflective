
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"
#include <cstdlib>
#include <tuple>

namespace reflective
{
	template <typename TYPE>
		class BulkListKnot
	{

	};

	template <typename TYPE, typename KNOT = BulkListKnot<TYPE> >
		class BulkList
	{
	public:

		template <typename... TYPES>
			inline static BulkList make(TYPES... i_tuple)
		{
			const size_t size = Size<TYPES...>::s_size;
			void * alloc = malloc(size);
			BulkList res;
			res.m_bulk = alloc;
			Size<TYPES...>::construct(alloc, std::forward<TYPES>(i_tuple)...);
			return res;
		}

	private:
		
		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct Size
		{
			static const size_t s_size = sizeof(FIRST_TYPE) + Size<OTHER_TYPES...>::s_size;

			inline static void construct(void * & io_address)
			{
				new (io_address) FIRST_TYPE();
				io_address = reinterpret_cast<void * >(reinterpret_cast<uintptr_t>(io_address) + sizeof(FIRST_TYPE));

				Size<OTHER_TYPES...>::construct(io_address);
			}

			inline static void construct(void * & io_address, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				new (io_address) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				io_address = reinterpret_cast<void * >(reinterpret_cast<uintptr_t>(io_address) + sizeof(FIRST_TYPE));

				Size<OTHER_TYPES...>::construct(io_address, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename FIRST_TYPE>
			struct Size<FIRST_TYPE>
		{
			static const size_t s_size = sizeof(FIRST_TYPE);

			inline static void construct(void * & io_address)
			{
				new (io_address) FIRST_TYPE();
				io_address = reinterpret_cast<void * >(reinterpret_cast<uintptr_t>(io_address) + sizeof(FIRST_TYPE));
			}

			inline static void construct(void * & io_address, FIRST_TYPE && i_source)
			{
				new (io_address) FIRST_TYPE(std::forward<FIRST_TYPE>(i_source));
				io_address = reinterpret_cast<void * >( reinterpret_cast<uintptr_t>( io_address ) + sizeof(FIRST_TYPE));
			}
		};

	private:
		void * m_bulk;
	};
}


namespace Test1
{
	struct P
	{
		int x, y;
	};

	struct P1 : public P
	{
		double d1;
	};

	struct P2 : public P
	{
		std::string s1;

		P2()
			: s1("test")
		{
			;
			std::cout << "P2 default constructor " << s1 << std::endl;
		}

		P2(const P2 &)
		{
			std::cout << "P2 copy constructor " << s1 << std::endl;
		}

		P2(P2 && src)
			: s1( std::move(src.s1) )
		{			
			std::cout << "P2 move constructor " << s1 << std::endl;
		}

		P2 & operator = (P2 && src)
		{
			s1 = std::move(src.s1);
			std::cout << "P2 move assignment " << s1 << std::endl;
			return *this;
		}

		P2 & operator = (const P2 & src)
		{
			s1 = src.s1;
			std::cout << "P2 copy assignment " << s1 << std::endl;
			return *this;
		}

		~P2()
		{
			std::cout << "P2 destructor" << s1 << std::endl;
		}
	};

	P make_p(int) { return P{}; }
	P1 make_p(double) { return P1{}; }
	P2 make_p(std::string) { return P2{}; }
}


void bulk_list__test()
{
	using namespace reflective;
	using namespace Test1;

	BulkList<P>::make(make_p("s"), make_p(4), make_p(4.3), make_p("s"));
}
