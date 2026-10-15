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
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "DefaultConstructedValueIsZero", &fastrttitests::TestDefaultConstructedValueIsZero },
		{ "ValueConstructorStoresValue", &fastrttitests::TestValueConstructorStoresValue },
		{ "WrapperIsKindOfItsValueType", &fastrttitests::TestWrapperIsKindOfItsValueType },
		{ "DynaCastToValueTypeReachesStoredValue", &fastrttitests::TestDynaCastToValueTypeReachesStoredValue },
	};

	return fastrttitests::RunTests(tests);
}
