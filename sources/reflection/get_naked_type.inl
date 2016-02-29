
namespace reflective
{
	namespace details
	{
		template <typename TYPE>
			SymbolName get_type_full_name()
		{
			return typeid(TYPE).name();
		}
	}

	template <typename TYPE>
		const ReflectingType<TYPE> * create_type()
	{
		const ReflectingType<TYPE> * result = details::SymbolTraits<TYPE, details::GetSymbolTypeId<TYPE>::s_type_id >::create();
		GlobalRegistry::instance().register_type(*result, typeid(TYPE));
		return result;
	}
		
	namespace details
	{
		template <typename TYPE> class TypeContainer
		{
		private:
			static const ReflectingType<TYPE> * s_type;
		public:

			/** Returns the reflective::Type object */
			static const ReflectingType<TYPE> & get()
			{
				const ReflectingType<TYPE> * type = s_type;
				if (type == nullptr)
				{
					type = create_type<TYPE>();
					s_type = type;
				}
				return *type;
			}
		};
		
		template <typename TYPE>
			const ReflectingType<TYPE> * TypeContainer<TYPE>::s_type = &TypeContainer<TYPE>::get();
	}

	template <typename TYPE>
		inline const ReflectingType<TYPE> & get_naked_type()
	{
		static_assert(!std::is_pointer<TYPE>::value && !std::is_reference<TYPE>::value && !std::is_rvalue_reference<TYPE>::value, 
			"get_naked_type can't be used pointers or references. Use get_type instead");
		return details::TypeContainer< details::CleanType<TYPE> >::get();
	}
}
