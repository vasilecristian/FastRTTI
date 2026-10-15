#include <vector>

#include "TypeIdentityTypes.h"
#include "TestHarness.h"

/** Type ids: one per type, the same everywhere for one type, and never shared by
 *  two different types, including types that are spelled the same. */
namespace fastrttitests
{
	namespace
	{
		/** Same spelling as the Impl in TypeIdentityOtherUnit.cpp, but a different type. */
		class Impl : public fastrtti::IRTTI<Impl>
		{
		};
	}

	namespace audio
	{
		class Channel : public fastrtti::IRTTI<Channel>
		{
		};
	}

	namespace video
	{
		class Channel : public fastrtti::IRTTI<Channel>
		{
		};
	}

	class First : public fastrtti::IRTTI<First>
	{
	};

	class Second : public fastrtti::IRTTI<Second>
	{
	};

	template<typename T>
	class Wrapper : public fastrtti::IRTTI<Wrapper<T>>
	{
	};

	TypeId GetIdOfLocalClassInFirstFunction()
	{
		class Local : public fastrtti::IRTTI<Local>
		{
		};
		return fastrtti::IRTTI<Local>::GetTypeID();
	}

	TypeId GetIdOfLocalClassInSecondFunction()
	{
		class Local : public fastrtti::IRTTI<Local>
		{
		};
		return fastrtti::IRTTI<Local>::GetTypeID();
	}

	void TestTypeIdIsStableAcrossCalls(TestContext& context)
	{
		// Arrange
		const TypeId firstCall = fastrtti::IRTTI<First>::GetTypeID();

		// Act
		const TypeId secondCall = fastrtti::IRTTI<First>::GetTypeID();

		// Assert
		context.Check(firstCall == secondCall, "a type has the same id on every call");
	}

	void TestDistinctTypesHaveDistinctIds(TestContext& context)
	{
		// Arrange
		const TypeId firstId = fastrtti::IRTTI<First>::GetTypeID();

		// Act
		const TypeId secondId = fastrtti::IRTTI<Second>::GetTypeID();
		const TypeId sharedId = fastrtti::IRTTI<SharedType>::GetTypeID();

		// Assert
		context.Check(firstId != secondId && firstId != sharedId && secondId != sharedId, "three types have three ids");
	}

	void TestSameTypeHasSameIdInEveryTranslationUnit(TestContext& context)
	{
		// Arrange
		const TypeId idInThisUnit = fastrtti::IRTTI<SharedType>::GetTypeID();

		// Act
		const TypeId idInOtherUnit = GetSharedTypeIdInOtherUnit();

		// Assert
		context.Check(idInThisUnit == idInOtherUnit, "SharedType has one id across translation units");
	}

	void TestTemplateInstantiationsHaveDistinctIds(TestContext& context)
	{
		// Arrange
		const TypeId intId = fastrtti::IRTTI<Wrapper<int>>::GetTypeID();

		// Act
		const TypeId floatId = fastrtti::IRTTI<Wrapper<float>>::GetTypeID();
		const TypeId nestedId = fastrtti::IRTTI<Wrapper<Wrapper<int>>>::GetTypeID();

		// Assert
		context.Check(intId != floatId && intId != nestedId && floatId != nestedId, "every instantiation of a template has its own id");
	}

	void TestSameNameInDifferentNamespacesHasDistinctIds(TestContext& context)
	{
		// Arrange
		const TypeId audioId = fastrtti::IRTTI<audio::Channel>::GetTypeID();

		// Act
		const TypeId videoId = fastrtti::IRTTI<video::Channel>::GetTypeID();

		// Assert
		context.Check(audioId != videoId, "audio::Channel and video::Channel have different ids");
	}

	void TestLocalClassesInDifferentFunctionsHaveDistinctIds(TestContext& context)
	{
		// Arrange
		const TypeId firstId = GetIdOfLocalClassInFirstFunction();

		// Act
		const TypeId secondId = GetIdOfLocalClassInSecondFunction();

		// Assert
		context.Check(firstId != secondId, "two local classes named Local in different functions have different ids");
	}

	void TestAnonymousNamespaceTypesInDifferentUnitsHaveDistinctIds(TestContext& context)
	{
		// Arrange
		const TypeId idInThisUnit = fastrtti::IRTTI<Impl>::GetTypeID();

		// Act
		const TypeId idInOtherUnit = GetAnonymousImplTypeIdInOtherUnit();

		// Assert
		context.Check(idInThisUnit != idInOtherUnit, "the Impl of each translation unit has its own id");
	}

	void TestAnonymousNamespaceTypeFromOtherUnitDoesNotCastToLocalOne(TestContext& context)
	{
		// Arrange
		std::unique_ptr<fastrtti::RTTI> otherUnitImpl = CreateAnonymousImplInOtherUnit();

		// Act
		Impl* castResult = fastrtti::dyna_cast<Impl>(otherUnitImpl.get());

		// Assert
		context.Check(castResult == nullptr, "the other unit's Impl is not this unit's Impl");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "TypeIdIsStableAcrossCalls", &fastrttitests::TestTypeIdIsStableAcrossCalls },
		{ "DistinctTypesHaveDistinctIds", &fastrttitests::TestDistinctTypesHaveDistinctIds },
		{ "SameTypeHasSameIdInEveryTranslationUnit", &fastrttitests::TestSameTypeHasSameIdInEveryTranslationUnit },
		{ "TemplateInstantiationsHaveDistinctIds", &fastrttitests::TestTemplateInstantiationsHaveDistinctIds },
		{ "SameNameInDifferentNamespacesHasDistinctIds", &fastrttitests::TestSameNameInDifferentNamespacesHasDistinctIds },
		{ "LocalClassesInDifferentFunctionsHaveDistinctIds", &fastrttitests::TestLocalClassesInDifferentFunctionsHaveDistinctIds },
		{ "AnonymousNamespaceTypesInDifferentUnitsHaveDistinctIds", &fastrttitests::TestAnonymousNamespaceTypesInDifferentUnitsHaveDistinctIds },
		{ "AnonymousNamespaceTypeFromOtherUnitDoesNotCastToLocalOne", &fastrttitests::TestAnonymousNamespaceTypeFromOtherUnitDoesNotCastToLocalOne },
	};

	return fastrttitests::RunTests(tests);
}
