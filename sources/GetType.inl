
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
		const reflecting_type<TYPE> * create_type()
	{
		return details::_SymbolTraits<TYPE, details::_GetSymbolTypeId<TYPE>::s_type_id >::create();
	}

	namespace details
	{
		template <typename TYPE> class TypeContainer
		{
		private:
			static const reflecting_type<TYPE> * s_type;
		public:

			/** Returns the reflective::Type object */
			static const reflecting_type<TYPE> & get()
			{
				const reflecting_type<TYPE> * type = s_type;
				if (s_type == nullptr)
				{
					type = create_type<TYPE>();
					s_type = type;
				}
				return *type;
			}
		};
		
		template <typename TYPE>
			const reflecting_type<TYPE> * TypeContainer<TYPE>::s_type = create_type<TYPE>();
	}

	template <typename TYPE>
		inline const reflecting_type<TYPE> & get_type()
	{
		return details::TypeContainer< std::decay<TYPE> >::get();
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
