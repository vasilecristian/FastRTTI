#include "TypeIdentityTypes.h"

namespace fastrttitests
{
	namespace
	{
		/** Same spelling as the Impl in TypeIdentityTests.cpp, but a different type:
		 *  an anonymous namespace is unique to its translation unit. */
		class Impl : public fastrtti::IRTTI<Impl>
		{
		};
	}

	TypeId GetSharedTypeIdInOtherUnit()
	{
		return fastrtti::IRTTI<SharedType>::GetTypeID();
	}

	TypeId GetAnonymousImplTypeIdInOtherUnit()
	{
		return fastrtti::IRTTI<Impl>::GetTypeID();
	}

	std::unique_ptr<fastrtti::RTTI> CreateAnonymousImplInOtherUnit()
	{
		return std::unique_ptr<fastrtti::RTTI>(new Impl());
	}
}
