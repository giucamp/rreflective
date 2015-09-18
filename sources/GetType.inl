
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

		template <typename TYPE, SymbolTypeId SYMBOL_TYPE > 
			struct DefaultTypeFactory;

		// DefaultTypeFactory< TYPE, SymbolTypeId::primitive_type_symbol >
		template <typename TYPE> 
			struct DefaultTypeFactory< TYPE, SymbolTypeId::primitive_type_symbol >
		{
			static inline const Type * create()
			{
				Type * new_type = new Type(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, get_special_functions<TYPE>());
				setup_type( *new_type, static_cast<TYPE*>(nullptr) );
				return new_type;
			}
		};

		// DefaultTypeFactory< TYPE, SymbolTypeId::class_symbol >
		template <typename TYPE>
			struct DefaultTypeFactory< TYPE, SymbolTypeId::class_symbol >
		{
			static inline const Class * create()
			{
				Class * class_obj = new Class(get_type_full_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, get_special_functions<TYPE>());
				setup_type( *class_obj, static_cast<TYPE*>(nullptr) );
				return class_obj;
			}
		};

		// DefaultTypeFactory< TYPE, SymbolTypeId::enum_symbol >
		template <typename TYPE>
		struct DefaultTypeFactory< TYPE, SymbolTypeId::enum_symbol >
		{
			static inline const Enum< std::underlying_type<TYPE> > * create()
			{
				Enum< std::underlying_type<TYPE> > * enum_obj = new Enum< std::underlying_type<TYPE> >(get_type_full_name<TYPE>(), get_special_functions<TYPE>());
				setup_type( *enum_obj, static_cast<TYPE*>(nullptr) );
				return enum_obj;
			}
		};
	}

	template <typename TYPE>
		const Type * create_type()
	{
		return details::DefaultTypeFactory<TYPE,
			std::is_class<TYPE>::value ? SymbolTypeId::class_symbol :
			(std::is_enum<TYPE>::value ? SymbolTypeId::enum_symbol : SymbolTypeId::primitive_type_symbol) >::create();
	}

	namespace details
	{
		template <typename TYPE>
		class TypeContainer
		{
		private:
			static const Type * s_type;
		public:

			/** Returns the reflective::Type object */
			static const Type & get()
			{
				const Type * type = s_type;
				if (s_type == nullptr)
				{
					type = create_type<TYPE>();
					s_type = type;
				}
				return *type;
			}
		};

		template <typename TYPE>
		const Type * TypeContainer<TYPE>::s_type = create_type<TYPE>();
	}

	template <typename TYPE>
		inline const Type & get_type()
	{
		return details::TypeContainer<TYPE>::get();
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
