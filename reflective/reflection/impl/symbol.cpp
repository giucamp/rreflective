

namespace reflective
{
	void Symbol::setup_class(Class & i_class)
	{
		i_class.set_properites(make_bulk_list<Property>(
			make_property<ThisClass>("name", &ThisClass::m_name),
			make_property<ThisClass>("attributes", &ThisClass::m_attributes)
		));
	}
}
