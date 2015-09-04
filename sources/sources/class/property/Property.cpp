
namespace reflective
{
	bool Property::get_value(const void * i_owner_object, void * i_dest, TextOutBuffer * i_error_stream) const
	{
		if (!has_flags(Flags::writeonly_member))
		{
			return get_value_impl(i_owner_object, i_dest, i_error_stream);
		}

		if (i_error_stream != nullptr)
		{
			*i_error_stream << "Can't get the value of a writeonly property";
		}
		return false;	
	}

	bool Property::set_value(const void * i_owner_object, const void * i_source, TextOutBuffer * i_error_stream) const
	{
		return set_value_impl(i_owner_object, i_source, i_error_stream);
	}

	const void * Property::get_value_inplace(const void * i_owner_object, TextOutBuffer * i_error_stream) const
	{
		return get_value_inplace_impl(const_cast<void*>(i_owner_object), i_error_stream);
	}

	void * Property::get_editable_value_inplace(void * i_owner_object, TextOutBuffer * i_error_stream) const
	{
		return get_value_inplace_impl(i_owner_object, i_error_stream);
	}
}