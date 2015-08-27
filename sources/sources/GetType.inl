
namespace reflective
{
	template <typename TYPE>
		void setup_type(Type & i_type)
	{

	}

	namespace details
	{
		template <typename TYPE>
			SymbolName get_type_name()
		{
			return "MyType";
		}

		template <typename TYPE, SymbolType SYMBOL_TYPE > 
			struct DefaultTypeFactory;

		// DefaultTypeFactory< TYPE, SymbolType::primitive_type_symbol >
		template <typename TYPE> 
			struct DefaultTypeFactory< TYPE, SymbolType::primitive_type_symbol >
		{
			static inline const Type * create()
			{
				return new Type(get_type_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, get_special_functions<TYPE>());
			}
		};

		// DefaultTypeFactory< TYPE, SymbolType::class_symbol >
		template <typename TYPE>
			struct DefaultTypeFactory< TYPE, SymbolType::class_symbol >
		{
			static inline const Class * create()
			{
				Class * class_obj = new Class(get_type_name<TYPE>(), sizeof(TYPE), std::alignment_of<TYPE>::value, get_special_functions<TYPE>());
				setup_type<TYPE>(*class_obj);
				return class_obj;
			}
		};
	}

	template <typename TYPE>
		const Type * create_type()
	{
		return details::DefaultTypeFactory<TYPE,
			std::is_class<TYPE>::value ? SymbolType::class_symbol :
			(std::is_enum<TYPE>::value ? SymbolType::enum_symbol : SymbolType::primitive_type_symbol) >::create();
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

			/** Returns the reflective::Type object */
			static const Type & fast_get()
			{
				assert(s_type != nullptr); /** The type has not yet been set. Probably you are calling get_type<TYPE>()
										   during the initialization of global variables. */
				return *s_type;
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
}
