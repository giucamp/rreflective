
#pragma once
#include <assert.h>
#include <type_traits>
#include <limits>
#include "density_common.h"

namespace reflective
{
	/*
		Element Type Synopsis

		class ElementType
		{
			public:

				template <typename COMPLETE_TYPE> static AutomaticElementType make() noexcept;

				ElementType(const ElementType &) noexcept;
				ElementType(ElementType &&) noexcept;
				~ElementType() noexcept;

				size_t size() const noexcept;
				size_t alignment() const noexcept;

				void copy_construct(void * i_dest_element, const void * i_source_element) const
				void move_construct(void * i_dest_element, void * i_source_element) const noexcept
				void destroy(void * i_element) const noexcept;
		};
	
	*/
	/** Specifies the way in which the size and the alignment of elements of a DenseList are stored */
	enum SizeAlignmentMode
	{
		most_general, /**< Uses two separate size_t to store the size and the alignment. */
		compact, /**< Both size and alignment are stored in a single size_t word. The alignment uses the 25% of the bits
					of the size_t, while the alignment uses all the other bits. For example, if size_t is big 64-bits, the
					alignment is stored in 16 bits, while the size is stored in 48 bits.
					If the size or the alignment can't be represented with the given number of bits, the behaviour is undefined.
					The implementation may report this error with a debug assert.
					If size_t has not a binary representation (that is std::numeric_limits<size_t>::radix != 2), using this
					representation wi resut in a compile time error. */
		assume_normal_alignment /**< Use a size_t word to store the size, and do not store the alignment: just assume that
					every element does not need an alignment more strict than a void pointer (void*).
					If an element actually needs a more strict alignment , the behaviour is undefined.
					The implementation may report this error with a debug assert.*/
	};

	enum ElementTypeCaps
	{
		none = 0,
		move_only = 1 << 1,
		copy_only = 1 << 0,		
		copy_and_move = move_only | copy_only,
	};

	namespace details
	{
		// This class is used by the default type-infos to store the size and the alignment according to the secified SizeAlignmentMode
		template <SizeAlignmentMode MODE>
			class ElementType_SizeAlign;
		template <> class ElementType_SizeAlign<SizeAlignmentMode::most_general>
		{
		public:

			ElementType_SizeAlign() = delete;
			ElementType_SizeAlign & operator = (const ElementType_SizeAlign &) = delete;
			ElementType_SizeAlign & operator = (ElementType_SizeAlign &&) = delete;

			ElementType_SizeAlign(const ElementType_SizeAlign &) REFLECTIVE_NOEXCEPT = default;
			#if defined(_MSC_VER) && _MSC_VER < 1900
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
					: m_size(i_source.m_size), m_alignment(i_source.m_alignment) { }			
			#else
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT = default;
			#endif

			ElementType_SizeAlign(size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
				: m_size(i_size), m_alignment(i_alignment) {}
			
			size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
			size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
						
		private:
			const size_t m_size, m_alignment;
		};
		template <> class ElementType_SizeAlign<
			std::enable_if<std::numeric_limits<size_t>::radix == 2, SizeAlignmentMode>::type::compact>
		{
		public:
			
			ElementType_SizeAlign() = delete;
			ElementType_SizeAlign & operator = (const ElementType_SizeAlign &) = delete;
			ElementType_SizeAlign & operator = (ElementType_SizeAlign &&) = delete;

			ElementType_SizeAlign(const ElementType_SizeAlign &) REFLECTIVE_NOEXCEPT = default;
			#if defined(_MSC_VER) && _MSC_VER < 1900
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
					: m_size(i_source.m_size), m_alignment(i_source.m_alignment) { }			
			#else
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT = default;
			#endif
			
			ElementType_SizeAlign(size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
				: m_size(i_size), m_alignment(i_alignment)
			{
				// check the correcteness of the narrowing conversion - a failure on this gives undefined behaviour
				assert(m_size == i_size && m_alignment == i_alignment);
			}
			size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
			size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }

		private:
			//static_assert(std::numeric_limits<size_t>::radix == 2, "size_t is expected to be a binary number");
			const size_t m_size : std::numeric_limits<size_t>::digits - std::numeric_limits<size_t>::digits / 4;
			const size_t m_alignment : std::numeric_limits<size_t>::digits / 4;
		};
		template <> class ElementType_SizeAlign<SizeAlignmentMode::assume_normal_alignment>
		{
		public:

			ElementType_SizeAlign() = delete;
			ElementType_SizeAlign & operator = (const ElementType_SizeAlign &) = delete;
			ElementType_SizeAlign & operator = (ElementType_SizeAlign &&) = delete;

			ElementType_SizeAlign(const ElementType_SizeAlign &) REFLECTIVE_NOEXCEPT = default;
			#if defined(_MSC_VER) && _MSC_VER < 1900
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
					: m_size(i_source.m_size) { }			
			#else
				ElementType_SizeAlign(ElementType_SizeAlign && i_source) REFLECTIVE_NOEXCEPT = default;
			#endif

			ElementType_SizeAlign(size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
				: m_size(i_size)
			{
				// check the correcteness of the alignment - a failure on this gives undefined behaviour
				assert(i_alignment <= std::alignment_of<void*>::value );
				(void)i_alignment;
			}

			size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
			size_t alignment() const REFLECTIVE_NOEXCEPT { return std::alignment_of<void*>::value; }

		private:
			const size_t m_size;
		};

		template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE, 
			bool HAS_VIRTUAL_DESTRUCTOR = std::has_virtual_destructor<ELEMENT>::value> class ElementType_Destr;

		template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
			class ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE, true> 
				: public ElementType_SizeAlign<SIZE_ALIGNMENT_MODE>
		{
		public:
			static_assert(std::is_nothrow_destructible<ELEMENT>::value, "the destructor must be nothrow");

			ElementType_Destr(size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
				: ElementType_SizeAlign<SIZE_ALIGNMENT_MODE>(i_size, i_alignment) { }

			void destroy(void * i_element) const REFLECTIVE_NOEXCEPT
			{
				assert(i_element != nullptr);
				static_cast<ELEMENT*>(i_element)->~ELEMENT();
			}
		};

		template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
			class ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE, false> 
				: public ElementType_SizeAlign<SIZE_ALIGNMENT_MODE>
		{
		public:
			static_assert(std::is_nothrow_destructible<ELEMENT>::value, "the destructor must be nothrow");

			ElementType_Destr(size_t i_size, size_t i_alignment) REFLECTIVE_NOEXCEPT
				: ElementType_SizeAlign<SIZE_ALIGNMENT_MODE>(i_size, i_alignment) { }

			void destroy(void * i_element) const REFLECTIVE_NOEXCEPT
			{
				assert(i_element != nullptr);
				static_cast<ELEMENT*>(i_element)->~ELEMENT();
			}
		};

		// CopyConstructImpl<COMPLETE_TYPE>::invoke
		template <typename COMPLETE_TYPE, bool CAN = std::is_copy_constructible<COMPLETE_TYPE>::value> struct CopyConstructImpl;
		template <typename COMPLETE_TYPE> struct CopyConstructImpl<COMPLETE_TYPE, true>
		{
			static void invoke(void * i_first, void * i_second)
			{
				new (i_first) COMPLETE_TYPE(*static_cast<const COMPLETE_TYPE*>(i_second));
			}
		};
		template <typename COMPLETE_TYPE> struct CopyConstructImpl<COMPLETE_TYPE, false>
		{
			static void invoke(void *, void *)
			{
				throw std::exception("copy-construction not supported");
			}
		};

		// MoveConstructImpl<COMPLETE_TYPE>::invoke
		template <typename COMPLETE_TYPE, bool CAN = std::is_nothrow_move_constructible<COMPLETE_TYPE>::value > struct MoveConstructImpl;
		template <typename COMPLETE_TYPE> struct MoveConstructImpl<COMPLETE_TYPE, true>
		{
			static void invoke(void * i_first, void * i_second)
			{
				new (i_first) COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_second)));
			}
		};
		template <typename COMPLETE_TYPE> struct MoveConstructImpl<COMPLETE_TYPE, false>
		{
			static void invoke(void *, void *)
			{
				throw std::exception("move-construction not supported");
			}
		};

		template <typename TYPE, 
			bool CAN_COPY = std::is_copy_constructible<TYPE>::value,
			bool CAN_MOVE = std::is_nothrow_move_constructible<TYPE>::value> struct GetAutoCopyMoveCap;

		template <typename TYPE > struct GetAutoCopyMoveCap<TYPE, false, false>
			{ static const ElementTypeCaps value = ElementTypeCaps::none; };
		template <typename TYPE > struct GetAutoCopyMoveCap<TYPE, false, true>
			{ static const ElementTypeCaps value = ElementTypeCaps::move_only; };
		template <typename TYPE > struct GetAutoCopyMoveCap<TYPE, true, false>
			{ static const ElementTypeCaps value = ElementTypeCaps::copy_only; };
		template <typename TYPE > struct GetAutoCopyMoveCap<TYPE, true, true>
			{ static const ElementTypeCaps value = ElementTypeCaps::copy_and_move; };

	} // namespace details


	template <typename ELEMENT, ElementTypeCaps COPY_MOVE, SizeAlignmentMode SIZE_ALIGNMENT_MODE = SizeAlignmentMode::most_general>
		class ElementType;
	
	template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE = SizeAlignmentMode::most_general>
		using AutomaticElementType = typename std::conditional<std::is_same<void, ELEMENT>::value,
			ElementType<void, ElementTypeCaps::copy_and_move, SIZE_ALIGNMENT_MODE >,
			ElementType<ELEMENT, details::GetAutoCopyMoveCap<ELEMENT>::value, SIZE_ALIGNMENT_MODE >
		>::type;
		
	template <SizeAlignmentMode SIZE_ALIGNMENT_MODE>
		class ElementType<void, ElementTypeCaps::copy_and_move, SIZE_ALIGNMENT_MODE> 
			: public details::ElementType_Destr<void, SIZE_ALIGNMENT_MODE>
	{
	public:
		
		using Element = void;
		static const SizeAlignmentMode s_size_alignment_mode = SIZE_ALIGNMENT_MODE;
		static const ElementTypeCaps s_caps = ElementTypeCaps::copy_and_move;

		ElementType() = delete;
		ElementType & operator = (const ElementType &) = delete;
		ElementType & operator = (ElementType &&) = delete;

		ElementType(const ElementType &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: details::ElementType_Destr<void, SIZE_ALIGNMENT_MODE>(std::move(i_source)), m_function(i_source.m_function)
					{ }			
		#else
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif
					
		template <typename COMPLETE_TYPE>
			static ElementType make() REFLECTIVE_NOEXCEPT
		{
			return ElementType(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &function_impl< COMPLETE_TYPE > );
		}

		void copy_construct(void * i_destination, const void * i_source_element) const
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(Operation::copy, i_destination, const_cast<void*>(i_source_element));
		}

		void move_construct(void * i_destination, void * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(Operation::move, i_destination, i_source_element); // first > second
		}

	private:

		enum class Operation { copy, move, destroy };
 
		using FunctionPtr = void(*)(Operation i_operation, void * i_first, void * i_second );

		ElementType(size_t i_size, size_t i_alignment, FunctionPtr i_function) REFLECTIVE_NOEXCEPT
			: details::ElementType_Destr<void, SIZE_ALIGNMENT_MODE>(i_size, i_alignment), m_function(i_function) { }

		template <typename COMPLETE_TYPE>
			static void function_impl(Operation i_operation, void * i_first, void * i_second)
		{
			switch (i_operation )
			{
				case Operation::copy:
					details::CopyConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
					break;

				case Operation::move:
					details::MoveConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
					break;

				default:
					static_cast<COMPLETE_TYPE*>(i_first)->COMPLETE_TYPE::~COMPLETE_TYPE();
					break;
			}
		}

	private:
		const FunctionPtr m_function;
	};

	template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
		class ElementType<ELEMENT, ElementTypeCaps::copy_and_move, SIZE_ALIGNMENT_MODE> : public details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>
	{
	public:

		using Element = ELEMENT;
		static const SizeAlignmentMode s_size_alignment_mode = SIZE_ALIGNMENT_MODE;
		static const ElementTypeCaps s_caps = ElementTypeCaps::copy_and_move;

		ElementType() = delete;
		ElementType & operator = (const ElementType &) = delete;
		ElementType & operator = (ElementType &&) = delete;

		ElementType(const ElementType &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: details::ElementType_Destr<ELEMENT,SIZE_ALIGNMENT_MODE>(std::move(i_source)), m_function(i_source.m_function)
					{ }			
		#else
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif
					
		template <typename COMPLETE_TYPE>
			static ElementType make() REFLECTIVE_NOEXCEPT
		{
			static_assert(std::is_base_of<ELEMENT, COMPLETE_TYPE>::value, "not covariant type");
			return ElementType(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &function_impl< COMPLETE_TYPE > );
		}

		void copy_construct(void * i_destination, const void * i_source_element) const
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(Operation::copy, i_destination, const_cast<void*>(i_source_element));
		}

		void move_construct(void * i_destination, void * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(Operation::move, i_destination, i_source_element); // first > second
		}

	private:

		enum class Operation { copy, move, destroy };
 
		using FunctionPtr = void(*)(Operation i_operation, void * i_first, void * i_second );

		ElementType(size_t i_size, size_t i_alignment, FunctionPtr i_function) REFLECTIVE_NOEXCEPT
			: details::ElementType_Destr<ELEMENT,SIZE_ALIGNMENT_MODE>(i_size, i_alignment), m_function(i_function) { }

		template <typename COMPLETE_TYPE>
			static void function_impl(Operation i_operation, void * i_first, void * i_second)
		{
			switch (i_operation )
			{
				case Operation::copy:
					details::CopyConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
					break;

				case Operation::move:
					details::MoveConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
					break;
			}
		}

	private:
		const FunctionPtr m_function;
	};

	template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
		class ElementType<ELEMENT, ElementTypeCaps::copy_only, SIZE_ALIGNMENT_MODE> : public details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>
	{
	public:

		using Element = ELEMENT;
		static const SizeAlignmentMode s_size_alignment_mode = SIZE_ALIGNMENT_MODE;
		static const ElementTypeCaps s_caps = ElementTypeCaps::copy_only;

		ElementType() = delete;
		ElementType & operator = (const ElementType &) = delete;
		ElementType & operator = (ElementType &&) = delete;

		ElementType(const ElementType &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: details::ElementType_Destr<ELEMENT,SIZE_ALIGNMENT_MODE>(std::move(i_source)), m_function(i_source.m_function)
					{ }			
		#else
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif
					
		template <typename COMPLETE_TYPE>
			static ElementType make() REFLECTIVE_NOEXCEPT
		{
			static_assert(std::is_base_of<ELEMENT, COMPLETE_TYPE>::value, "not covariant type");
			return ElementType(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &function_impl< COMPLETE_TYPE > );
		}

		void copy_construct(void * i_destination, const void * i_source_element) const
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(i_destination, const_cast<void*>(i_source_element));
		}

	private:

		enum class Operation { copy, move, destroy };
 
		using FunctionPtr = void(*)(void * i_first, void * i_second );

		ElementType(size_t i_size, size_t i_alignment, FunctionPtr i_function) REFLECTIVE_NOEXCEPT
			: details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>(i_size, i_alignment), m_function(i_function) { }

		template <typename COMPLETE_TYPE>
			static void function_impl(void * i_first, void * i_second)
		{
			details::CopyConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
		}

	private:
		const FunctionPtr m_function;
	};

	template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
		class ElementType<ELEMENT, ElementTypeCaps::move_only, SIZE_ALIGNMENT_MODE> 
			: public details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>
	{
	public:

		using Element = ELEMENT;
		static const SizeAlignmentMode s_size_alignment_mode = SIZE_ALIGNMENT_MODE;
		static const ElementTypeCaps s_caps = ElementTypeCaps::move_only;

		ElementType() = delete;
		ElementType & operator = (const ElementType &) = delete;
		ElementType & operator = (ElementType &&) = delete;

		ElementType(const ElementType &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>(std::move(i_source)), m_function(i_source.m_function)
					{ }			
		#else
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif
					
		template <typename COMPLETE_TYPE>
			static ElementType make() REFLECTIVE_NOEXCEPT
		{
			static_assert(std::is_base_of<ELEMENT, COMPLETE_TYPE>::value, "not covariant type");
			return ElementType(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &function_impl< COMPLETE_TYPE > );
		}

		void move_construct(void * i_destination, void * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			assert(i_destination != nullptr && i_source_element != nullptr && i_destination != i_source_element);
			(*m_function)(i_destination, i_source_element); // first > second
		}

	private:

		using FunctionPtr = void(*)(void * i_first, void * i_second );

		ElementType(size_t i_size, size_t i_alignment, FunctionPtr i_function) REFLECTIVE_NOEXCEPT
			: details::ElementType_Destr<ELEMENT,SIZE_ALIGNMENT_MODE>(i_size, i_alignment), m_function(i_function) { }

		template <typename COMPLETE_TYPE>
			static void function_impl(void * i_first, void * i_second)
		{
			details::MoveConstructImpl<COMPLETE_TYPE>::invoke(i_first, i_second);
		}

	private:
		const FunctionPtr m_function;
	};

	template <typename ELEMENT, SizeAlignmentMode SIZE_ALIGNMENT_MODE>
		class ElementType<ELEMENT, ElementTypeCaps::none, SIZE_ALIGNMENT_MODE>
			: public details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>
	{
	public:

		using Element = ELEMENT;
		static const SizeAlignmentMode s_size_alignment_mode = SIZE_ALIGNMENT_MODE;
		static const ElementTypeCaps s_caps = ElementTypeCaps::none;

		ElementType() = delete;
		ElementType & operator = (const ElementType &) = delete;
		ElementType & operator = (ElementType &&) = delete;

		ElementType(const ElementType &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>(std::move(i_source)) { }
		#else
			ElementType(ElementType && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif
					
		template <typename COMPLETE_TYPE>
			static ElementType make() REFLECTIVE_NOEXCEPT
		{
			static_assert(std::is_base_of<ELEMENT, COMPLETE_TYPE>::value, "not covariant type");
			return ElementType(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value );
		}

	private:

		ElementType(size_t i_size, size_t i_alignment ) REFLECTIVE_NOEXCEPT
			: details::ElementType_Destr<ELEMENT, SIZE_ALIGNMENT_MODE>(i_size, i_alignment) { }
	};
}