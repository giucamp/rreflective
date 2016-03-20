
#pragma once
#include "Property.h"

namespace reflective
{
	namespace details
	{
		// DataMemberCopyConstruct: helper class that tries to copy construct
		template <typename TYPE, bool CAN_COPY=std::is_copy_constructible<TYPE>::value > struct DataMemberCopyConstruct;
		template <typename TYPE>
			struct DataMemberCopyConstruct<TYPE,true>
		{
			static bool copy_construct(void * i_dest, const TYPE & i_source)
			{
				new(i_dest) TYPE(i_source);
				return true;
			}
			static const ClassMember::Flags s_can = ClassMember::Flags::can_get_value_by_copy;
		};
		template <typename TYPE>
			struct DataMemberCopyConstruct<TYPE,false>
		{
			static bool copy_construct(void * /*i_dest*/, const TYPE & /*i_source*/)
				{ return false; }
			static const ClassMember::Flags s_can = ClassMember::Flags::none;
		};

		// DataMemberCopyAssign: helper class that tries to copy assign
		template <typename TYPE, bool CAN_COPY=std::is_copy_assignable<TYPE>::value > struct DataMemberCopyAssign;
		template <typename TYPE>
			struct DataMemberCopyAssign<TYPE,true>
		{
			static bool copy_assign(TYPE & i_dest, const TYPE & i_source)
			{
				i_dest = i_source;
				return true;
			}
			static const ClassMember::Flags s_can = ClassMember::Flags::can_set_value_by_copy;
		};
		template <typename TYPE>
			struct DataMemberCopyAssign<TYPE,false>
		{
			static bool copy_assign(TYPE & /*i_dest*/, const TYPE & /*i_source*/)
				{ return false; }
			static const ClassMember::Flags s_can = ClassMember::Flags::none;
		};


		// DataMemberMoveConstruct: helper class that tries to move construct
		template <typename TYPE, bool CAN_MOVE=std::is_move_constructible<TYPE>::value > struct DataMemberMoveConstruct;
		template <typename TYPE>
			struct DataMemberMoveConstruct<TYPE,true>
		{
			static bool move_construct(void * i_dest, TYPE && i_source)
			{
				new(i_dest) TYPE(std::move(i_source));
				return true;
			}
			static const ClassMember::Flags s_can = ClassMember::Flags::can_get_value_by_move;
		};
		template <typename TYPE>
			struct DataMemberMoveConstruct<TYPE,false>
		{
			static bool move_construct(void * /*i_dest*/, TYPE && /*i_source*/)
				{ return false; }
			static const ClassMember::Flags s_can = ClassMember::Flags::none;
		};

		// DataMemberMoveAssign: helper class that tries to move assign
		template <typename TYPE, bool CAN_MOVE=std::is_move_assignable<TYPE>::value > struct DataMemberMoveAssign;
		template <typename TYPE>
			struct DataMemberMoveAssign<TYPE,true>
		{
			static bool move_assign(TYPE & i_dest, TYPE && i_source)
			{
				i_dest = std::move(i_source);
				return true;
			}
			static const ClassMember::Flags s_can = ClassMember::Flags::can_set_value_by_move;
		};
		template <typename TYPE>
			struct DataMemberMoveAssign<TYPE,false>
		{
			static bool move_assign(TYPE & /*i_dest*/, TYPE && /*i_source*/)
				{ return false; }
			static const ClassMember::Flags s_can = ClassMember::Flags::none;
		};

		template <typename OWNER_CLASS, typename PROPERTY_TYPE>
			class DataMemberProperty : public Property
		{
		public:

			using MemberPtr = PROPERTY_TYPE(OWNER_CLASS::*);

			DataMemberProperty(SymbolName i_name, MemberPtr i_member_ptr, ClassMember::Flags i_flags) REFLECTIVE_NOEXCEPT
				: Property(std::move(i_name), get_type<PROPERTY_TYPE>(), 
					i_flags | DataMemberCopyConstruct<PROPERTY_TYPE>::s_can | DataMemberMoveConstruct<PROPERTY_TYPE>::s_can |
						DataMemberCopyAssign<PROPERTY_TYPE>::s_can | DataMemberMoveAssign<PROPERTY_TYPE>::s_can ),
				m_member_ptr(i_member_ptr)
			{
			}

			#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
				DataMemberProperty(const DataMemberProperty &) = delete;
			#endif

		private:

			void * get_value_inplace_impl(ObjPtr i_owner_object) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				PROPERTY_TYPE & property_value = owner_object.*m_member_ptr;
				dbg_object_validate(property_value);

				return &property_value;
			}

			bool get_value_by_copy_impl(ObjPtr i_owner_object, void * i_dest, OutStringBuffer & o_fail_reason) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				const OWNER_CLASS & owner_object = *static_cast<const OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				const PROPERTY_TYPE & source_value = owner_object.*m_member_ptr;
				dbg_object_validate(source_value);

				const bool result = DataMemberCopyConstruct<PROPERTY_TYPE>::copy_construct(i_dest, source_value);
				if (result)
				{
					dbg_object_validate(*static_cast<const PROPERTY_TYPE*>(i_dest));
				}
				else
				{
					o_fail_reason << "the type '"; // << details::get_type_full_name<PROPERTY_TYPE>() << "' does not support copy construction";
				}
				return result;
			}

			bool get_value_by_move_impl(ObjPtr i_owner_object, void * i_dest, OutStringBuffer & o_fail_reason) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				PROPERTY_TYPE & source_value = owner_object.*m_member_ptr;
				dbg_object_validate(source_value);

				const bool result = DataMemberMoveConstruct<PROPERTY_TYPE>::move_construct(i_dest, std::move(source_value));
				if (result)
				{
					dbg_object_validate(*static_cast<const PROPERTY_TYPE*>(i_dest));
				}
				else
				{
					o_fail_reason << "the type "; // << details::get_type_full_name<PROPERTY_TYPE>() << " does not support move construction";
				}
				return result;
			}

			bool set_value_by_copy_impl(ObjPtr i_owner_object, const void * i_source, OutStringBuffer & o_fail_reason) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				PROPERTY_TYPE & dest_value = owner_object.*m_member_ptr;
				dbg_object_validate(dest_value);

				const PROPERTY_TYPE & source_value = *static_cast<const PROPERTY_TYPE*>(i_source);
				dbg_object_validate(source_value);

				const bool result = DataMemberCopyAssign<PROPERTY_TYPE>::copy_assign(dest_value, source_value);
				if (result)
				{
					dbg_object_validate(dest_value);
				}
				else
				{
					o_fail_reason << "the type "; // << details::get_type_full_name<PROPERTY_TYPE>() << " does not support copy assignment";
				}
				return result;
			}

			bool set_value_by_move_impl(ObjPtr i_owner_object, void * i_source, OutStringBuffer & o_fail_reason) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				PROPERTY_TYPE & dest_value = owner_object.*m_member_ptr;
				dbg_object_validate(dest_value);

				PROPERTY_TYPE & source_value = *static_cast<PROPERTY_TYPE*>(i_source);
				dbg_object_validate(source_value);

				const bool result = DataMemberMoveAssign<PROPERTY_TYPE>::move_assign(dest_value, std::move(source_value));
				if (result)
				{
					dbg_object_validate(dest_value);
				}
				else
				{
					o_fail_reason << "the type "; // << details::get_type_full_name<PROPERTY_TYPE>() << " does not support move assignment";
				}
				return result;
			}

		private:
			const MemberPtr m_member_ptr;
		};

		template <typename OWNER_CLASS, typename PROPERTY_TYPE>
			class ConstDataMemberProperty : public Property
		{
		public:

			using MemberPtr = const PROPERTY_TYPE(OWNER_CLASS::*);

			ConstDataMemberProperty(SymbolName i_name, MemberPtr i_member_ptr, ClassMember::Flags i_flags) REFLECTIVE_NOEXCEPT
				: Property(std::move(i_name), get_type<PROPERTY_TYPE>(), 
					i_flags | ClassMember::Flags::can_get_value_by_copy ),
					m_member_ptr(i_member_ptr) { }

			#if defined(_MSC_VER) && _MSC_VER < 1900 // Visual Studio 2013 and below
				ConstDataMemberProperty(const ConstDataMemberProperty &) = delete;
			#endif

		private:

			void * get_value_inplace_impl(ObjPtr i_owner_object) const override
			{
				OWNER_CLASS & owner_object = *static_cast<OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				const PROPERTY_TYPE & property_value = owner_object.*m_member_ptr;
				dbg_object_validate(property_value);

				return const_cast<PROPERTY_TYPE*>(&property_value);
			}

			bool get_value_by_copy_impl(ObjPtr i_owner_object, void * i_dest, OutStringBuffer & o_fail_reason) const override
			{
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());

				const OWNER_CLASS & owner_object = *static_cast<const OWNER_CLASS*>(i_owner_object.object());
				dbg_object_validate(owner_object);

				const PROPERTY_TYPE & source_value = owner_object.*m_member_ptr;
				dbg_object_validate(source_value);

				const bool result = DataMemberCopyConstruct<PROPERTY_TYPE>::copy_construct(i_dest, source_value);
				if (result)
				{
					dbg_object_validate(*static_cast<const PROPERTY_TYPE*>(i_dest));
				}
				else
				{
					o_fail_reason << "the type '"; // << details::get_type_full_name<PROPERTY_TYPE>() << "' does not support copy construction";
				}
				return result;
			}

			bool get_value_by_move_impl(ObjPtr i_owner_object, void * /*i_dest*/, OutStringBuffer & o_fail_reason) const override
			{
				(void)&i_owner_object; // possibly unreferenced parameter
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());
				o_fail_reason << "the property '" << name() << "' is readonly";
				return false;
			}

			bool set_value_by_copy_impl(ObjPtr i_owner_object, const void * /*i_source*/, OutStringBuffer & o_fail_reason) const override
			{
				(void)&i_owner_object; // possibly unreferenced parameter
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());
				o_fail_reason << "the property '" << name() << "' is readonly";
				return false;
			}

			bool set_value_by_move_impl(ObjPtr i_owner_object, void * /*i_source*/, OutStringBuffer & o_fail_reason) const override
			{
				(void)&i_owner_object; // possibly unreferenced parameter
				REFLECTIVE_RUNTIME_CHECK(i_owner_object.type().primary_type() == &get_naked_type<OWNER_CLASS>());
				o_fail_reason << "the property '" << name() << "' is readonly";
				return false;
			}

		private:
			const MemberPtr m_member_ptr;
		};

	} // namespace details

	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		inline details::DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE> make_property(SymbolName i_name, PROPERTY_TYPE (OWNER_CLASS::*i_member_ptr),
			ClassMember::Flags i_flags = ClassMember::Flags::none) REFLECTIVE_NOEXCEPT
	{
		return details::DataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>(i_name, i_member_ptr, i_flags);
	}

	template <typename OWNER_CLASS, typename PROPERTY_TYPE>
		inline details::ConstDataMemberProperty<OWNER_CLASS, PROPERTY_TYPE> make_property(SymbolName i_name, const PROPERTY_TYPE (OWNER_CLASS::*i_member_ptr),
			ClassMember::Flags i_flags = ClassMember::Flags::none) REFLECTIVE_NOEXCEPT
	{
		return details::ConstDataMemberProperty<OWNER_CLASS, PROPERTY_TYPE>(i_name, i_member_ptr, i_flags);
	}
}
