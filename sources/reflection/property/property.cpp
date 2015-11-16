
namespace reflective
{
	bool Property::get_value(ObjPtr i_owner_object, void * i_dest) const
	{
		return get_value_impl(i_owner_object, i_dest);
	}

	bool Property::set_value(ObjPtr i_owner_object, const void * i_source) const
	{
		return set_value_impl(i_owner_object, i_source);
	}

	const void * Property::get_value_inplace(ObjPtr i_owner_object) const
	{
		return get_value_inplace_impl(i_owner_object);
	}

	void * Property::get_editable_value_inplace(ObjPtr i_owner_object) const
	{
		return get_value_inplace_impl(i_owner_object);
	}
}