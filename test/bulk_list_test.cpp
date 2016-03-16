
#include <string>
#include <iostream>
#include <cstdlib>
#include "..\sources\reflective.h"
#include <cstdlib>
#include <tuple>
#include "..\sources\core\bulk_list.h"

/*namespace reflective
{
	template <typename OTHER_TYPE, typename... CONSTR_PARAMS>
	static OTHER_TYPE * linear_alloc(void * & io_address, CONSTR_PARAMS && ...i_params)
	{
		OTHER_TYPE * result = new OTHER_TYPE(std::forward<CONSTR_PARAMS>(i_params)...);
		io_address = result + 1;
		return result;
	}

	template <typename TYPE>
		class BulkListKnot
	{
	public:
		BulkListKnot * m_next;
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
			void * bulk = alloc;
			Size<TYPES...>::construct(alloc, nullptr, std::forward<TYPES>(i_tuple)...);
			return BulkList(bulk);
		}

		BulkList() : m_bulk(nullptr) {}

		BulkList(const BulkList &) = delete;

		BulkList & operator = (const BulkList &) = delete;

		BulkList(BulkList && i_from)
			: m_bulk(i_from.m_bulk)
		{
			i_from.m_bulk = nullptr;
		}

		BulkList & operator = (BulkList && i_from)
		{
			m_bulk = i_from.m_bulk;
			i_from.m_bulk = nullptr;
		}

		~BulkList()
		{
			free(m_bulk);
		}

	private:
		
		BulkList(void * i_bulk)
		{
			m_bulk = i_bulk;
		}
		
		template <typename FIRST_TYPE, typename... OTHER_TYPES>
			struct Size
		{
			static const size_t s_size = sizeof(KNOT) + sizeof(FIRST_TYPE) + Size<OTHER_TYPES...>::s_size;

			inline static void construct(void * & io_address, KNOT * i_prev_knot, FIRST_TYPE && i_source, OTHER_TYPES && ... i_s1)
			{
				KNOT * knot = linear_alloc<KNOT>(io_address);
				if (i_prev_knot != nullptr)
				{
					i_prev_knot->m_next = knot;
				}				

				linear_alloc<FIRST_TYPE>(io_address, std::forward<FIRST_TYPE>(i_source));
								
				Size<OTHER_TYPES...>::construct(io_address, knot, std::forward<OTHER_TYPES>(i_s1)...);
			}
		};

		template <typename FIRST_TYPE>
			struct Size<FIRST_TYPE>
		{
			static const size_t s_size = sizeof(FIRST_TYPE) + sizeof(KNOT);
			
			inline static void construct(void * & io_address, KNOT * i_prev_knot, FIRST_TYPE && i_source)
			{
				KNOT * knot = linear_alloc<KNOT>(io_address);
				if (i_prev_knot != nullptr)
				{
					i_prev_knot->m_next = knot;
				}
				knot->m_next = nullptr;

				linear_alloc<FIRST_TYPE>(io_address, std::forward<FIRST_TYPE>(i_source));
			}
		};

	private:
		void * m_bulk;
	};
}*/

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

	//BulkList<P>::make(make_p(1));

	BulkList<P>::make(make_p("s"), make_p(4), make_p(4.3), make_p("s"));
}
