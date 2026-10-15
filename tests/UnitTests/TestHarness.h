#ifndef FASTRTTI_TESTS_TEST_HARNESS_H
#define FASTRTTI_TESTS_TEST_HARNESS_H

#include <cstdio>
#include <vector>

/** A minimal test runner, so the suite needs nothing beyond the standard library.
 *
 *  Every test is a function taking a TestContext and written as Arrange, Act,
 *  Assert. A failed check is recorded and reported, it does not stop the test. */
namespace fastrttitests
{
	/** Collects the checks made by one test. */
	class TestContext
	{
	public:

		TestContext()
			: m_failedCheckCount(0)
		{
		}

		/** Records a failed check when condition is false.
		 * @param condition is the outcome the test expects to hold.
		 * @param description says what was expected, and is printed on failure. */
		void Check(bool condition, const char* description)
		{
			if (!condition)
			{
				++m_failedCheckCount;
				std::printf("    FAILED: %s\n", description);
			}
		}

		bool HasPassed() const
		{
			return m_failedCheckCount == 0;
		}

	private:

		int m_failedCheckCount;
	};

	typedef void (*TestFunction)(TestContext& context);

	struct TestCase
	{
		const char* m_name;
		TestFunction m_function;
	};

	/** Runs every test in order and prints a summary.
	 * @return 0 when every test passed and 1 otherwise, ready to return from main. */
	inline int RunTests(const std::vector<TestCase>& tests)
	{
		int failedTestCount = 0;

		for (const TestCase& test : tests)
		{
			// Flushed before the test runs, so a crash still names the test that caused it.
			std::printf("[ RUN    ] %s\n", test.m_name);
			std::fflush(stdout);

			TestContext context;
			test.m_function(context);

			if (context.HasPassed())
			{
				std::printf("[     OK ] %s\n", test.m_name);
			}
			else
			{
				++failedTestCount;
				std::printf("[ FAILED ] %s\n", test.m_name);
			}
		}

		std::printf("%d of %d tests failed\n", failedTestCount, static_cast<int>(tests.size()));
		return failedTestCount == 0 ? 0 : 1;
	}
}

#endif //FASTRTTI_TESTS_TEST_HARNESS_H
