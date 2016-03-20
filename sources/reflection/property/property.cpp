
namespace reflective
{
	bool Property::get_value_by_copy(ObjPtr i_owner_object, void * i_dest, OutStringBuffer & o_fail_reason) const
	{
		return get_value_by_copy_impl(i_owner_object, i_dest, o_fail_reason);
	}

	bool Property::get_value_by_move(ObjPtr i_owner_object, void * i_dest, OutStringBuffer & o_fail_reason) const
	{
		return get_value_by_move_impl(i_owner_object, i_dest, o_fail_reason);
	}

	bool Property::set_value_by_copy(ObjPtr i_owner_object, const void * i_source, OutStringBuffer & o_fail_reason) const
	{
		return set_value_by_copy_impl(i_owner_object, i_source, o_fail_reason);
	}

	bool Property::set_value_by_move(ObjPtr i_owner_object, void * i_source, OutStringBuffer & o_fail_reason) const
	{
		return set_value_by_move_impl(i_owner_object, i_source, o_fail_reason);
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