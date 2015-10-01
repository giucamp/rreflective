
namespace reflective
{
	inline void setup_type(Type & /*i_type*/, void * )
	{

	}

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
		const SymbolName full_name(result->full_name().c_str());
		Registry::instance().register_type(full_name, *result);
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
		return details::TypeContainer< details::CleanType<TYPE> >::get();
	}

	inline const Namespace & root_namespace()
	{
		return edit_root_namespace();
	}

	inline Namespace & edit_root_namespace()
	{
		static Namespace root(SymbolName{});
		return root;
	}
}
