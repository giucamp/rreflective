
namespace reflective
{
	namespace details
	{
					// ScalarDefaultConstructor

		template <typename TYPE, bool IS_DEFAULT_CONSTRUCTIBLE >
			class ScalarDefaultConstructor;

		template <typename TYPE >
			class ScalarDefaultConstructor < TYPE, false >
		{
		public:
			static SpecialFunctions::ScalarTor get() { return nullptr; }
		};

		template <typename TYPE >
			class ScalarDefaultConstructor < TYPE, true >
		{
			static void _function(void * i_objects_start, void * i_objects_end)
			{
				TYPE * const start = static_cast<TYPE*>(i_objects_start);
				TYPE * const end = static_cast<TYPE*>(i_objects_end);
				REFLECTIVE_ASSERT(is_valid_range(start, end), "The object range is invalid");
				for (TYPE * curr = start; curr < end; curr++)
				{
					new (curr) TYPE();
				}
			}

		public:
			static SpecialFunctions::ScalarTor get() { return _function; }
		};

				// ScalarCopyConstructor

		template <typename TYPE, bool IS_COPY_CONSTRUCTIBLE >
			class ScalarCopyConstructor;

		template <typename TYPE >
			class ScalarCopyConstructor < TYPE, false >
		{
		public:
			static SpecialFunctions::CopyConstructor get() { return nullptr; }
		};

		template <typename TYPE >
			class ScalarCopyConstructor < TYPE, true >
		{
			static void _function(void * i_objects_start, void * i_objects_end, const void * i_source_start )
			{
				TYPE * const dest_start = static_cast<TYPE*>(i_objects_start);
				TYPE * const dest_end = static_cast<TYPE*>(i_objects_end);
				const TYPE * const source_start = static_cast<const TYPE*>(i_source_start);

				REFLECTIVE_ASSERT(is_valid_range(dest_start, dest_end), "The object range is invalid");
				const TYPE * source = source_start;
				for (TYPE * dest = dest_start; dest < dest_end; dest++, source++)
				{
					new(dest) TYPE(*source);
				}
			}

		public:
			static SpecialFunctions::CopyConstructor get() { return _function; }
		};


				// ScalarMoveConstructor

		template <typename TYPE, bool IS_MOVE_CONSTRUCTIBLE >
			class ScalarMoveConstructor;

		template <typename TYPE >
			class ScalarMoveConstructor < TYPE, false >
		{
		public:
			static SpecialFunctions::MoveConstructor get() { return nullptr; }
		};

		template <typename TYPE >
			class ScalarMoveConstructor < TYPE, true >
		{
			static void _function(void * i_objects_start, void * i_objects_end, void * i_source_start )
			{
				TYPE * const dest_start = static_cast<TYPE*>(i_objects_start);
				TYPE * const dest_end = static_cast<TYPE*>(i_objects_end);
				TYPE * const source_start = static_cast<TYPE*>(i_source_start);

				REFLECTIVE_ASSERT(is_valid_range(dest_start, dest_end), "The object range is invalid");
				TYPE * source = source_start;
				for (TYPE * dest = dest_start; dest < dest_end; dest++, source++)
				{
					new(dest) TYPE(std::move(*source));
				}
			}

		public:
			static SpecialFunctions::MoveConstructor get() { return _function; }
		};

				// ScalarDestructor

		template <typename TYPE, bool IS_DESTRUCTIBLE >
			class ScalarDestructor;

		template <typename TYPE >
			class ScalarDestructor < TYPE, false >
		{
		public:
			static SpecialFunctions::ScalarTor get() { return nullptr; }
		};

		template <typename TYPE >
			class ScalarDestructor < TYPE, true >
		{
			static void _function(void * i_objects_start, void * i_objects_end)
			{
				TYPE * const start = static_cast<TYPE*>(i_objects_start);
				TYPE * const end = static_cast<TYPE*>(i_objects_end);
				REFLECTIVE_ASSERT(is_valid_range(start, end), "The object range is invalid");
				for (TYPE * curr = start; curr < end; curr++)
				{
					curr->~TYPE();
				}
			}

		public:
			static SpecialFunctions::ScalarTor get() { return _function; }
		};
	}

	template <typename TYPE>
		inline SpecialFunctions SpecialFunctions::from_type()
	{
		return SpecialFunctions(
			details::ScalarDefaultConstructor<TYPE, std::is_default_constructible<TYPE>::value >::get(),
			details::ScalarCopyConstructor<TYPE, std::is_copy_constructible<TYPE>::value >::get(),
			details::ScalarMoveConstructor<TYPE, std::is_move_constructible<TYPE>::value >::get(),
			details::ScalarDestructor<TYPE, std::is_destructible<TYPE>::value >::get() );
	}
}



