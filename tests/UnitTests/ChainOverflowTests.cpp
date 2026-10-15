// Configuration of the header under test, so it has to come before it is included.
// Four slots, so the five level hierarchy below overflows the chain by one.
#define RTTI_CHAIN_MAX_SIZE 4
// The library's bound check is routed here, so a test can see whether it fired.
#define RTTI_ASSERT(condition) fastrttitests::RecordChainAssert(condition)

namespace fastrttitests
{
	void RecordChainAssert(bool condition);
}

#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** A hierarchy deeper than RTTI_CHAIN_MAX_SIZE.
 *
 *  Registering past the end of the chain used to write past the end of its fixed
 *  arrays. The registration that does not fit has to be refused instead, and the
 *  ones that do fit have to keep working. */
namespace fastrttitests
{
	namespace
	{
		int g_failedChainAssertCount = 0;
	}

	void RecordChainAssert(bool condition)
	{
		if (!condition)
		{
			++g_failedChainAssertCount;
		}
	}

	class Level1 : public fastrtti::IRTTI<Level1>
	{
	};

	class Level2 : public Level1, public fastrtti::IRTTI<Level2>
	{
	};

	class Level3 : public Level2, public fastrtti::IRTTI<Level3>
	{
	};

	class Level4 : public Level3, public fastrtti::IRTTI<Level4>
	{
	};

	/** The fifth registration, one more than the chain holds. */
	class Level5 : public Level4, public fastrtti::IRTTI<Level5>
	{
	};

	void TestHierarchyThatExactlyFillsChainIsFullyCastable(TestContext& context)
	{
		// Arrange
		g_failedChainAssertCount = 0;
		Level4 object;

		// Act
		Level1* level1 = fastrtti::dyna_cast<Level1>(&object);
		Level4* level4 = fastrtti::dyna_cast<Level4>(&object);

		// Assert
		context.Check(level1 == static_cast<Level1*>(&object), "the innermost type resolves when the chain is exactly full");
		context.Check(level4 == &object, "the last slot is usable");
		context.Check(g_failedChainAssertCount == 0, "a full chain is not reported as an overflow");
	}

	void TestTypesWithinCapacityStillCastCorrectly(TestContext& context)
	{
		// Arrange
		Level5 object;

		// Act
		Level1* level1 = fastrtti::dyna_cast<Level1>(&object);
		Level2* level2 = fastrtti::dyna_cast<Level2>(&object);
		Level3* level3 = fastrtti::dyna_cast<Level3>(&object);
		Level4* level4 = fastrtti::dyna_cast<Level4>(&object);

		// Assert
		context.Check(level1 == static_cast<Level1*>(&object), "Level1 resolves to the Level1 subobject");
		context.Check(level2 == static_cast<Level2*>(&object), "Level2 resolves to the Level2 subobject");
		context.Check(level3 == static_cast<Level3*>(&object), "Level3 resolves to the Level3 subobject");
		context.Check(level4 == static_cast<Level4*>(&object), "Level4 resolves to the Level4 subobject");
	}

	void TestRegistrationBeyondCapacityIsRefused(TestContext& context)
	{
		// Arrange
		Level5 object;

		// Act
		Level5* level5 = fastrtti::dyna_cast<Level5>(&object);

		// Assert
		context.Check(level5 == nullptr, "the registration that does not fit is refused, not written past the chain");
	}

	void TestOverflowIsReportedToAssertHook(TestContext& context)
	{
		// Arrange
		g_failedChainAssertCount = 0;

		// Act
		Level5 object;

		// Assert
		context.Check(g_failedChainAssertCount > 0, "the overflow reaches RTTI_ASSERT");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "HierarchyThatExactlyFillsChainIsFullyCastable", &fastrttitests::TestHierarchyThatExactlyFillsChainIsFullyCastable },
		{ "TypesWithinCapacityStillCastCorrectly", &fastrttitests::TestTypesWithinCapacityStillCastCorrectly },
		{ "RegistrationBeyondCapacityIsRefused", &fastrttitests::TestRegistrationBeyondCapacityIsRefused },
		{ "OverflowIsReportedToAssertHook", &fastrttitests::TestOverflowIsReportedToAssertHook },
	};

	return fastrttitests::RunTests(tests);
}
