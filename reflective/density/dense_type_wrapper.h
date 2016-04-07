
#pragma once
#include <assert.h>
#include <type_traits>
#include <limits>
#include "density_common.h"

namespace reflective
{
	/** Specifies the way in which the size and the alignment of elements of a DenseList are stored */
	enum DenseList_SizeAlignmentMode
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

	enum DenseList_CopyMove
	{
		copy_and_move,
		move_only,		
		none,
	};

	namespace details
	{
		// This class is used by the default type-infos to store the size and the alignment according to the secified DenseList_SizeAlignmentMode
		template <DenseList_SizeAlignmentMode MODE>
			class DenseList_SizeAlignmentChunk;
		template <> class DenseList_SizeAlignmentChunk<DenseList_SizeAlignmentMode::most_general>
		{
		public:
			DenseList_SizeAlignmentChunk(size_t i_size, size_t i_alignment)
				: m_size(i_size), m_alignment(i_alignment) {}
			
			size_t size() const { return m_size; }
			size_t alignment() const { return m_alignment; }

			DenseList_SizeAlignmentChunk & operator = (const DenseList_SizeAlignmentChunk &) = delete;
			
		private:
			const size_t m_size, m_alignment;
		};
		template <> class DenseList_SizeAlignmentChunk<
			std::enable_if<std::numeric_limits<size_t>::radix == 2, DenseList_SizeAlignmentMode>::type::compact>
		{
		public:
			DenseList_SizeAlignmentChunk(size_t i_size, size_t i_alignment)
				: m_size(i_size), m_alignment(i_alignment)
			{
				// check the correcteness of the narrowing conversion - a failure on this gives undefined behaviour
				assert(m_size == i_size && m_alignment == i_alignment);
			}
			size_t size() const { return m_size; }
			size_t alignment() const { return m_alignment; }

			DenseList_SizeAlignmentChunk & operator = (const DenseList_SizeAlignmentChunk &) = delete;

		private:
			static_assert(std::numeric_limits<size_t>::radix == 2, "size_t is expected to be a binary number");
			const size_t m_size : std::numeric_limits<size_t>::digits - std::numeric_limits<size_t>::digits / 4;
			const size_t m_alignment : std::numeric_limits<size_t>::digits / 4;
		};
		template <> class DenseList_SizeAlignmentChunk<DenseList_SizeAlignmentMode::assume_normal_alignment>
		{
		public:
			DenseList_SizeAlignmentChunk(size_t i_size, size_t i_alignment)
				: m_size(i_size)
			{
				// check the correcteness of the alignment - a failure on this gives undefined behaviour
				assert(i_alignment <= std::alignment_of<void*>::value );
				(void)i_alignment;
			}

			size_t size() const { return m_size; }
			size_t alignment() const { return std::alignment_of<void*>::value; }
			DenseList_SizeAlignmentChunk & operator = (const DenseList_SizeAlignmentChunk &) = delete;

		private:
			const size_t m_size;
		};

		/*template <typename ELEMENT, DenseList_SizeAlignmentMode SIZE_ALIGNMENT_MODE, DenseList_CopyMove COPY_MOVE_MODE>
			class AutomaticTypeWrapper;
		template <typename ELEMENT> 
			class AutomaticTypeWrapper<ELEMENT, DenseList_CopyMove::copy_and_move, SIZE_ALIGNMENT_MODE>
		{
		public:



			using FunctionPtr = void(*)(void * i_dest_element, void * i_source_element );

			void copy_construct(void * i_destination, const ELEMENT & i_source_element) const
			{
				(*m_function)(i_destination, i_source_element);
			}

			void move_construct_if_no_except(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
			{
				(*m_function)(i_destination, i_source_element);
			}

		private:
			const FunctionPtr m_function;
		};*/
	}

	template <typename ELEMENT> class CopyableTypeWrapper;
	template <typename ELEMENT> class MovableTypeWrapper;
	template <typename ELEMENT> class UnmovableTypeWrapper;

	template <typename ELEMENT> using AutomaticTypeWrapper = typename std::conditional< std::is_copy_constructible<ELEMENT>::value,
		CopyableTypeWrapper<ELEMENT>,
			typename std::conditional< std::is_nothrow_move_constructible<ELEMENT>::value,
				MovableTypeWrapper<ELEMENT>, UnmovableTypeWrapper<ELEMENT> >::type
		>::type;

	template <typename ELEMENT, typename ALLOCATOR = std::allocator<ELEMENT>, 
		typename TYPE_WRAPPER = AutomaticTypeWrapper<ELEMENT> > class DenseList;

	/** Creates and returns by value a DenseList, whose element base type is ELEMENT. The specified parameters
		are added to the list<br>
		Example: auto int_list = make_dense_list<int>( 1, 2 * 2, 3 * 3 ); */
	template <typename ELEMENT, typename... TYPES>
		inline DenseList<ELEMENT, std::allocator<ELEMENT>> make_dense_list(TYPES &&... i_parameters)
	{
		return DenseList<ELEMENT, std::allocator<ELEMENT>>::make( std::forward<TYPES>(i_parameters)... );
	}
	template <typename ELEMENT, typename ALLOCATOR, typename... TYPES>
		inline DenseList<ELEMENT, ALLOCATOR> alloc_dense_list(const ALLOCATOR & i_allocator, TYPES &&... i_parameters)
	{
		return DenseList<ELEMENT, ALLOCATOR>::make_with_alloc(i_allocator, std::forward<TYPES>(i_parameters)... );
	}

	template <typename ELEMENT>
		class CopyableTypeWrapper
	{
	public:

		CopyableTypeWrapper() = delete;
		CopyableTypeWrapper & operator = (const CopyableTypeWrapper &) = delete;
		CopyableTypeWrapper & operator = (CopyableTypeWrapper &&) = delete;

		CopyableTypeWrapper(const CopyableTypeWrapper &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			CopyableTypeWrapper(CopyableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
					{ }			
		#else
			CopyableTypeWrapper(CopyableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static CopyableTypeWrapper make() REFLECTIVE_NOEXCEPT
		{
			return CopyableTypeWrapper(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, 
				&CopyConstructImpl< COMPLETE_TYPE >, &MoverDestructorImpl< COMPLETE_TYPE >);
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
		
		size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
		
		void copy_construct(void * i_destination, const ELEMENT & i_source_element) const
		{
			(*m_copy_constructor)(i_destination, i_source_element);
		}

		void move_construct_if_no_except(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

		void destroy(ELEMENT * i_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

	private:

		using CopyConstructorPtr = void(*)(void * i_dest_element, const ELEMENT & i_source_element);
		using MoverDestructorPtr = void (*)(void * i_dest_element, ELEMENT * i_source_element);

		CopyableTypeWrapper(size_t i_size, size_t i_alignment, CopyConstructorPtr i_copy_constructor, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_copy_constructor(i_copy_constructor), m_mover_destructor(i_mover_destructor)
				{ }

		template <typename COMPLETE_TYPE>
			static void CopyConstructImpl(void * i_destination, const ELEMENT & i_source_element)
		{
			new (i_destination) COMPLETE_TYPE(static_cast<const COMPLETE_TYPE&>(i_source_element));
		}

		template <typename COMPLETE_TYPE>
			static void MoverDestructorImpl(void * i_destination, ELEMENT * i_source_element)
		{
			if (i_destination != nullptr)
			{
				new (i_destination) COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_source_element)));
			}
			static_cast<COMPLETE_TYPE*>(i_source_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

	private:
		size_t const m_size, m_alignment;
		CopyConstructorPtr const m_copy_constructor;
		MoverDestructorPtr const m_mover_destructor;
	};

	template <typename ELEMENT>
		class MovableTypeWrapper
	{
	public:

		MovableTypeWrapper() = delete;
		MovableTypeWrapper & operator = (const MovableTypeWrapper &) = delete;
		MovableTypeWrapper & operator = (MovableTypeWrapper &&) = delete;

		MovableTypeWrapper(const MovableTypeWrapper &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			MovableTypeWrapper(MovableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment), m_mover_destructor(i_source.m_mover_destructor)
					{ }			
		#else
			MovableTypeWrapper(MovableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static MovableTypeWrapper make() REFLECTIVE_NOEXCEPT
		{
			return MovableTypeWrapper(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value, &MoverDestructorImpl< COMPLETE_TYPE >);
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
		
		size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
		
		void move_construct_if_no_except(void * i_destination, ELEMENT * i_source_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(i_destination, i_source_element);
		}

		void destroy(ELEMENT * i_element) const REFLECTIVE_NOEXCEPT
		{
			(*m_mover_destructor)(nullptr, i_element);
		}

	private:

		using MoverDestructorPtr = void (*)(void * i_dest_element, ELEMENT * i_source_element);

		MovableTypeWrapper(size_t i_size, size_t i_alignment, MoverDestructorPtr i_mover_destructor)
			: m_size(i_size), m_alignment(i_alignment), m_mover_destructor(i_mover_destructor)
				{ }
		
		template <typename COMPLETE_TYPE>
			static void MoverDestructorImpl(void * i_destination, ELEMENT * i_source_element)
		{
			if (i_destination != nullptr)
			{
				new (i_destination) COMPLETE_TYPE(std::move(*static_cast<COMPLETE_TYPE*>(i_source_element)));
			}
			static_cast<COMPLETE_TYPE*>(i_source_element)->COMPLETE_TYPE::~COMPLETE_TYPE();
		}

	private:
		size_t const m_size, m_alignment;
		MoverDestructorPtr const m_mover_destructor;
	};

	template <typename ELEMENT>
		class UnmovableTypeWrapper
	{
	public:

		UnmovableTypeWrapper() = delete;
		UnmovableTypeWrapper & operator = (const UnmovableTypeWrapper &) = delete;
		UnmovableTypeWrapper & operator = (UnmovableTypeWrapper &&) = delete;

		UnmovableTypeWrapper(const UnmovableTypeWrapper &) REFLECTIVE_NOEXCEPT = default;

		#if defined(_MSC_VER) && _MSC_VER < 1900
			UnmovableTypeWrapper(UnmovableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT // visual studio 2013 doesnt seems to support defauted move constructors
				: m_size(i_source.m_size), m_alignment(i_source.m_alignment)
					{ }			
		#else
			UnmovableTypeWrapper(UnmovableTypeWrapper && i_source) REFLECTIVE_NOEXCEPT = default;
		#endif

		template <typename COMPLETE_TYPE>
			static UnmovableTypeWrapper make() REFLECTIVE_NOEXCEPT
		{
			return UnmovableTypeWrapper(sizeof(COMPLETE_TYPE), std::alignment_of<COMPLETE_TYPE>::value);
		}

		size_t size() const REFLECTIVE_NOEXCEPT { return m_size; }
		
		size_t alignment() const REFLECTIVE_NOEXCEPT { return m_alignment; }
		
		void destroy(ELEMENT * i_element) const REFLECTIVE_NOEXCEPT
		{
			#if defined(_MSC_VER)
				(void)i_element; // workaround for warning C4100: 'i_element': unreferenced formal parameter
			#endif
			i_element->~ELEMENT();
		}

	private:

		UnmovableTypeWrapper(size_t i_size, size_t i_alignment)
			: m_size(i_size), m_alignment(i_alignment)
				{ }

	private:
		size_t const m_size, m_alignment;
	};
}