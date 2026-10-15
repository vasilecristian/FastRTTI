#include <string>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** The debug table of inheritance chain names.
 *
 *  Built with DEBUG defined, see tests/CMakeLists.txt. In any other build the
 *  table does not exist and GetInheritanceChainNames() is always empty. */
namespace fastrttitests
{
	class NamedRoot : public fastrtti::IRTTI<NamedRoot>
	{
	};

	class NamedMiddle : public NamedRoot, public fastrtti::IRTTI<NamedMiddle>
	{
	};

	class NamedLeaf : public NamedMiddle, public fastrtti::IRTTI<NamedLeaf>
	{
	};

	bool Contains(const std::string& text, const char* part)
	{
		return text.find(part) != std::string::npos;
	}

	void TestChainNamesHaveOneEntryPerRegisteredType(TestContext& context)
	{
		// Arrange
		NamedLeaf leaf;

		// Act
		const std::vector<std::string> names = leaf.GetInheritanceChainNames();

		// Assert
		context.Check(names.size() == 3, "NamedRoot, NamedMiddle and NamedLeaf each have an entry");
	}

	void TestChainNamesOfSingleTypeHaveOneEntry(TestContext& context)
	{
		// Arrange
		NamedRoot root;

		// Act
		const std::vector<std::string> names = root.GetInheritanceChainNames();

		// Assert
		context.Check(names.size() == 1, "an object with one registered type has one entry");
	}

	void TestChainNamesEndWithMostDerivedType(TestContext& context)
	{
		// Arrange
		NamedLeaf leaf;

		// Act
		const std::vector<std::string> names = leaf.GetInheritanceChainNames();

		// Assert
		context.Check(!names.empty() && Contains(names.back(), "NamedLeaf"), "the last entry names the most derived type");
		context.Check(!names.empty() && Contains(names.front(), "NamedRoot"), "the first entry names the innermost base");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "ChainNamesHaveOneEntryPerRegisteredType", &fastrttitests::TestChainNamesHaveOneEntryPerRegisteredType },
		{ "ChainNamesOfSingleTypeHaveOneEntry", &fastrttitests::TestChainNamesOfSingleTypeHaveOneEntry },
		{ "ChainNamesEndWithMostDerivedType", &fastrttitests::TestChainNamesEndWithMostDerivedType },
	};

	return fastrttitests::RunTests(tests);
}
