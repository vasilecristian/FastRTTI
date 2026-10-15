#ifndef FASTRTTI_TESTS_TYPE_IDENTITY_TYPES_H
#define FASTRTTI_TESTS_TYPE_IDENTITY_TYPES_H

#include <memory>

#include "FastRTTI/RTTI.hpp"

/** Types and functions shared by TypeIdentityTests.cpp and TypeIdentityOtherUnit.cpp,
 *  two translation units of one executable. */
namespace fastrttitests
{
	/** A type both translation units see. */
	class SharedType : public fastrtti::IRTTI<SharedType>
	{
	};

	/** The library's type id type, which differs between versions of the header. */
	typedef decltype(fastrtti::IRTTI<SharedType>::GetTypeID()) TypeId;

	/** @return the type id of SharedType as the other translation unit sees it. */
	TypeId GetSharedTypeIdInOtherUnit();

	/** @return the type id of the other unit's own anonymous-namespace Impl. */
	TypeId GetAnonymousImplTypeIdInOtherUnit();

	/** @return an object of the other unit's anonymous-namespace Impl. */
	std::unique_ptr<fastrtti::RTTI> CreateAnonymousImplInOtherUnit();
}

#endif //FASTRTTI_TESTS_TYPE_IDENTITY_TYPES_H
