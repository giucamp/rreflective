

namespace reflective
{
	template <typename CHAR, typename CHAR_TRAITS >
		class BasicInStringBuffer final
	{
	public:

		/** Constructs an empty buffer */
		REFLECTIVE_CONSTEXPR BasicInStringBuffer() REFLECTIVE_NOEXCEPT = default;

		/** Copy construct */
		REFLECTIVE_CONSTEXPR BasicInStringBuffer(const BasicInStringBuffer & i_source) REFLECTIVE_NOEXCEPT = default;

		/** Constructs a string view from a std::BasicStringView. */
		REFLECTIVE_CONSTEXPR BasicInStringBuffer(const BasicStringView<CHAR, CHAR_TRAITS> & i_string_view) REFLECTIVE_NOEXCEPT
			: m_view(i_string_view) { }

		BasicInStringBuffer & operator = (const BasicInStringBuffer & i_source) REFLECTIVE_NOEXCEPT = default;

	private:
		BasicStringView<CHAR, CHAR_TRAITS> m_view;
		std::exception_ptr m_exception;
	};
}