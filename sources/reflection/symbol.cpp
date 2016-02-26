

namespace reflective
{
	void Symbol::setup_class(Class & i_class)
	{
		List<Property> properties =
		{
			make_property<ThisClass>("name", &ThisClass::m_name),
			make_property<ThisClass>("attributes", &ThisClass::m_attributes),
		};
		i_class.set_properites(std::move(properties));
	}
}
