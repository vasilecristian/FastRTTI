#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** BasicTypeRTTI, the wrapper that gives a plain value such as an int a type id. */
namespace fastrttitests
{
	void TestDefaultConstructedValueIsZero(TestContext& context)
	{
		// Arrange and Act
		fastrtti::BasicTypeRTTI<int> value;

		// Assert
		context.Check(value.m_val == 0, "a default constructed wrapper holds zero");
	}

	void TestValueConstructorStoresValue(TestContext& context)
	{
		// Arrange and Act
		fastrtti::BasicTypeRTTI<double> value(2.5);

		// Assert
		context.Check(value.m_val == 2.5, "the wrapper holds the value it was given");
	}

	void TestWrapperIsKindOfItsValueType(TestContext& context)
	{
		// Arrange
		fastrtti::BasicTypeRTTI<int> value(5);
		fastrtti::RTTI* object = &value;

		// Act
		const bool isInt = fastrtti::ObjectIsKindOf<int>(object);
		const bool isFloat = fastrtti::ObjectIsKindOf<float>(object);

		// Assert
		context.Check(isInt, "a wrapped int is kind of int");
		context.Check(!isFloat, "a wrapped int is not kind of float");
	}

	void TestDynaCastToValueTypeReachesStoredValue(TestContext& context)
	{
		// Arrange
		fastrtti::BasicTypeRTTI<int> value(5);
		fastrtti::RTTI* object = &value;

		// Act
		int* castResult = fastrtti::dyna_cast<int>(object);

		// Assert: only the address is compared, the result is never read through.
		context.Check(castResult == &value.m_val, "dyna_cast<int> on a wrapped int points at the stored int");
	}

	void TestDynaCastOnCopyReachesCopysOwnValue(TestContext& context)
	{
		// Arrange
		const fastrtti::BasicTypeRTTI<int> source(5);

		// Act
		fastrtti::BasicTypeRTTI<int> copy(source);
		int* castResult = fastrtti::dyna_cast<int>(&copy);

		// Assert
		context.Check(castResult == &copy.m_val, "dyna_cast<int> on a copy points at the copy's int, not the source's");
		context.Check(copy.m_val == 5, "the copy holds the source's value");
	}

	void TestDynaCastOnMovedToReachesItsOwnValue(TestContext& context)
	{
		// Arrange
		fastrtti::BasicTypeRTTI<std::vector<int>> source(std::vector<int>(3, 7));

		// Act
		fastrtti::BasicTypeRTTI<std::vector<int>> movedTo(std::move(source));
		std::vector<int>* castResult = fastrtti::dyna_cast<std::vector<int>>(&movedTo);

		// Assert
		context.Check(castResult == &movedTo.m_val, "dyna_cast on a moved-to wrapper points at its own value, not the source's");
		context.Check(movedTo.m_val.size() == 3, "the moved-to wrapper holds the source's value");
	}

	void TestAssignmentKeepsEachWrappersOwnValue(TestContext& context)
	{
		// Arrange
		const fastrtti::BasicTypeRTTI<int> source(5);
		fastrtti::BasicTypeRTTI<int> target(1);

		// Act
		target = source;
		int* castResult = fastrtti::dyna_cast<int>(&target);

		// Assert
		context.Check(castResult == &target.m_val, "dyna_cast<int> on an assigned wrapper still points at its own int");
		context.Check(target.m_val == 5, "the assigned wrapper holds the source's value");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "DefaultConstructedValueIsZero", &fastrttitests::TestDefaultConstructedValueIsZero },
		{ "ValueConstructorStoresValue", &fastrttitests::TestValueConstructorStoresValue },
		{ "WrapperIsKindOfItsValueType", &fastrttitests::TestWrapperIsKindOfItsValueType },
		{ "DynaCastToValueTypeReachesStoredValue", &fastrttitests::TestDynaCastToValueTypeReachesStoredValue },
		{ "DynaCastOnCopyReachesCopysOwnValue", &fastrttitests::TestDynaCastOnCopyReachesCopysOwnValue },
		{ "DynaCastOnMovedToReachesItsOwnValue", &fastrttitests::TestDynaCastOnMovedToReachesItsOwnValue },
		{ "AssignmentKeepsEachWrappersOwnValue", &fastrttitests::TestAssignmentKeepsEachWrappersOwnValue },
	};

	return fastrttitests::RunTests(tests);
}
