#include <string>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** A debug build with the compiler's own RTTI switched off (-fno-rtti or /GR-), as
 *  mobile and console toolchains build it. The library exists to replace RTTI there,
 *  so its debug build has to compile without typeid. */
namespace fastrttitests
{
	class Vehicle : public fastrtti::IRTTI<Vehicle>
	{
	};

	class Truck : public Vehicle, public fastrtti::IRTTI<Truck>
	{
	};

	void TestDynaCastWorksWithoutCompilerRtti(TestContext& context)
	{
		// Arrange
		Truck truck;

		// Act
		Vehicle* vehicle = fastrtti::dyna_cast<Vehicle>(&truck);

		// Assert
		context.Check(vehicle == static_cast<Vehicle*>(&truck), "Truck casts to its Vehicle subobject");
	}

	void TestChainNamesAreRecordedWithoutCompilerRtti(TestContext& context)
	{
		// Arrange
		Truck truck;

		// Act
		const std::vector<std::string> names = truck.GetInheritanceChainNames();

		// Assert
		context.Check(!names.empty(), "the debug chain names are filled in without the compiler's RTTI");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "DynaCastWorksWithoutCompilerRtti", &fastrttitests::TestDynaCastWorksWithoutCompilerRtti },
		{ "ChainNamesAreRecordedWithoutCompilerRtti", &fastrttitests::TestChainNamesAreRecordedWithoutCompilerRtti },
	};

	return fastrttitests::RunTests(tests);
}
