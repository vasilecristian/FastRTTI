#include <atomic>
#include <thread>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** Concurrent use. Every object owns its chain and only reads shared state, so
 *  objects can be built and cast on several threads at once. The first use of a
 *  type's id is the one shared write, which is what the first test races on.
 *
 *  Plain runs catch wrong results. Build with -fsanitize=thread to catch the races
 *  themselves. */
namespace fastrttitests
{
	static const int k_threadCount = 8;
	static const int k_iterationCount = 2000;

	/** Used by no other test, so its id is first requested by the racing threads. */
	class FreshBase : public fastrtti::IRTTI<FreshBase>
	{
	};

	class FreshDerived : public FreshBase, public fastrtti::IRTTI<FreshDerived>
	{
	};

	class SharedBase : public fastrtti::IRTTI<SharedBase>
	{
	};

	class SharedDerived : public SharedBase, public fastrtti::IRTTI<SharedDerived>
	{
	};

	/** Runs work on k_threadCount threads released at the same moment, so they
	 *  overlap as much as possible. */
	template<typename Work>
	void RunConcurrently(Work work)
	{
		std::atomic<bool> isReleased(false);
		std::vector<std::thread> threads;
		for (int i = 0; i < k_threadCount; ++i)
		{
			threads.emplace_back([&isReleased, &work]()
			{
				while (!isReleased.load())
				{
					std::this_thread::yield();
				}
				work();
			});
		}

		isReleased.store(true);
		for (std::thread& thread : threads)
		{
			thread.join();
		}
	}

	void TestConcurrentConstructionAndCastingIsConsistent(TestContext& context)
	{
		// Arrange
		std::atomic<int> failedCastCount(0);

		// Act
		RunConcurrently([&failedCastCount]()
		{
			for (int i = 0; i < k_iterationCount; ++i)
			{
				FreshDerived object;
				fastrtti::RTTI* asRtti = &object;
				if (fastrtti::dyna_cast<FreshDerived>(asRtti) != &object
					|| fastrtti::dyna_cast<FreshBase>(asRtti) != static_cast<FreshBase*>(&object))
				{
					++failedCastCount;
				}
			}
		});

		// Assert
		context.Check(failedCastCount.load() == 0, "objects built and cast on several threads at once all cast correctly");
	}

	void TestConcurrentCastsOnSharedObjectAgree(TestContext& context)
	{
		// Arrange
		SharedDerived sharedObject;
		std::atomic<int> failedCastCount(0);

		// Act
		RunConcurrently([&sharedObject, &failedCastCount]()
		{
			for (int i = 0; i < k_iterationCount; ++i)
			{
				if (fastrtti::dyna_cast<SharedBase>(&sharedObject) != static_cast<SharedBase*>(&sharedObject))
				{
					++failedCastCount;
				}
			}
		});

		// Assert
		context.Check(failedCastCount.load() == 0, "one object cast from several threads at once always gives the same result");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "ConcurrentConstructionAndCastingIsConsistent", &fastrttitests::TestConcurrentConstructionAndCastingIsConsistent },
		{ "ConcurrentCastsOnSharedObjectAgree", &fastrttitests::TestConcurrentCastsOnSharedObjectAgree },
	};

	return fastrttitests::RunTests(tests);
}
