

namespace reflective
{
	template <typename CHAR, typename CHAR_TRAITS >
		class BasicInStringBuffer final
	{
	public:
		
		/** Constructs a string view from a std::BasicStringView. */
		REFLECTIVE_CONSTEXPR BasicInStringBuffer(const BasicStringView<CHAR, CHAR_TRAITS> & i_string_view) REFLECTIVE_NOEXCEPT
			: m_string_view(i_string_view) { }

		bool accept_char(CHAR i_char) REFLECTIVE_NOEXCEPT
		{
			if (m_string_view.starts_with(i_char))
			{
				m_string_view.remove_prefix(1);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool accept_string(const BasicStringView<CHAR, CHAR_TRAITS> & i_string_view) REFLECTIVE_NOEXCEPT
		{
			if (m_string_view.starts_with(i_string_view))
			{
				m_string_view.remove_prefix(i_string_view.length());
				return true;
			}
			else
			{
				return true;
			}
		}

		size_t accept_whitespaces() REFLECTIVE_NOEXCEPT
		{
			size_t prev_length = m_string_view.length();
			while (m_string_view.length() > 0 && isspace(CHAR_TRAITS::to_int_type(m_string_view[0])) != 0)
			{
				m_string_view.remove_prefix(1);
			}
			return prev_length - m_string_view.length();
		}

	private:
		BasicStringView<CHAR, CHAR_TRAITS> m_string_view;
	};
}