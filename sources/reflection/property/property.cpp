
namespace reflective
{
	bool Property::get_value(ObjPtr i_owner_object, void * i_dest) const
	{
		if (has_flags(ClassMember::Flags::can_get_value))
		{
			return get_value_impl(i_owner_object, i_dest);
		}
		else
		{
			return false;
		}
	}

	bool Property::set_value(ObjPtr i_owner_object, const void * i_source, OutStringBuffer & o_fail_reason) const
	{
		if (has_flags(ClassMember::Flags::can_set_value))
		{
			return set_value_impl(i_owner_object, i_source, o_fail_reason);
		}
		else
		{
			o_fail_reason << "The property '" << name() << "' does not support setting the value";
			return false;
		}
	}

	const void * Property::get_value_inplace(ObjPtr i_owner_object) const
	{
		if (has_flags(ClassMember::Flags::can_get_value_inplace))
		{
			return get_value_inplace_impl(i_owner_object);
		}
		else
		{
			return false;
		}
	}

	void * Property::get_editable_value_inplace(ObjPtr i_owner_object) const
	{
		if (has_flags(ClassMember::Flags::can_set_value_inplace))
		{
			return get_value_inplace_impl(i_owner_object);
		}
		else
		{
			return false;
		}		
	}
}