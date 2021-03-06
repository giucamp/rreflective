


namespace reflective_externals
{
	// reflection of std::vector
	template< typename TYPE, typename ALLOCATOR >
		void init_type( reflective::Type * volatile * o_result,
			std::vector< TYPE, ALLOCATOR > * null_pointer_1,
			std::vector< TYPE, ALLOCATOR > * null_pointer_2 );

	// reflection of std::map
	template< typename KEY, typename VALUE, typename PREDICATE, typename ALLOCATOR >
		void init_type( reflective::Type * volatile * o_result,
			std::map< KEY, VALUE, PREDICATE, ALLOCATOR > * null_pointer_1,
			std::map< KEY, VALUE, PREDICATE, ALLOCATOR > * null_pointer_2 );

} // namespace reflective_externals
