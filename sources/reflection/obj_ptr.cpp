

namespace reflective
{
	ObjPtr ObjPtr::cast_to(QualifiedTypePtr i_dest_type) const
	{
		if (m_object != nullptr)
		{
			auto source_primary_type = m_type.primary_type();
			auto dest_primary_type = i_dest_type.primary_type();
			auto result = source_primary_type->try_dynamic_cast(m_object, *dest_primary_type);
			if (result != nullptr)
			{
				return ObjPtr(result, i_dest_type);
			}			
		}
		return ObjPtr();
	}

	ObjPtr ObjPtr::upcast_to(QualifiedTypePtr i_dest_type) const
	{
		if (m_object != nullptr)
		{
			auto source_primary_type = m_type.primary_type();
			auto dest_primary_type = i_dest_type.primary_type();
			auto result = source_primary_type->try_dynamic_cast(m_object, *dest_primary_type);
			if (result != nullptr)
			{
				return ObjPtr(result, i_dest_type);
			}
		}
		return ObjPtr();
	}
}