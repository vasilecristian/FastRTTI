#include <memory>
#include <vector>

#include "CrossModule/CrossModuleTypes.h"
#include "TestHarness.h"

/** Objects crossing a shared library boundary.
 *
 *  A type id has to mean the same type in every module. An id taken from the
 *  address of a per-type template static does not: each DLL, and each shared
 *  library built with hidden visibility, has its own copy of that static. */
namespace fastrttitests
{
	typedef std::unique_ptr<fastrtti::RTTI, void (*)(fastrtti::RTTI*)> PluginProductPtr;

	/** Known only to the executable. The plugin reaches it through Purchasable. */
	class HostProduct : public Purchasable, public fastrtti::IRTTI<HostProduct>
	{
	public:

		int GetPrice() const override
		{
			return 7;
		}
	};

	void TestObjectFromHostCastsInPlugin(TestContext& context)
	{
		// Arrange
		HostProduct product;

		// Act
		const int priceSeenByPlugin = GetPriceInPlugin(&product);

		// Assert
		context.Check(priceSeenByPlugin == 7, "an object made in the executable casts to Purchasable inside the plugin");
	}

	void TestTypeIdIsTheSameInEveryModule(TestContext& context)
	{
		// Arrange
		const TypeId idInThisModule = fastrtti::IRTTI<Purchasable>::GetTypeID();

		// Act
		const TypeId idInPlugin = GetPurchasableTypeIdInPlugin();

		// Assert
		context.Check(idInPlugin == idInThisModule, "Purchasable has the same type id in the plugin and in the executable");
	}

	void TestObjectFromPluginCastsToSharedInterface(TestContext& context)
	{
		// Arrange
		PluginProductPtr product(CreatePluginProduct(), &DestroyPluginProduct);

		// Act
		Purchasable* purchasable = fastrtti::dyna_cast<Purchasable>(product.get());

		// Assert
		context.Check(purchasable != nullptr, "an object made in the plugin casts to Purchasable in the executable");
		context.Check(purchasable != nullptr && purchasable->GetPrice() == k_pluginProductPrice, "the cast reaches the plugin's implementation");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "TypeIdIsTheSameInEveryModule", &fastrttitests::TestTypeIdIsTheSameInEveryModule },
		{ "ObjectFromPluginCastsToSharedInterface", &fastrttitests::TestObjectFromPluginCastsToSharedInterface },
		{ "ObjectFromHostCastsInPlugin", &fastrttitests::TestObjectFromHostCastsInPlugin },
	};

	return fastrttitests::RunTests(tests);
}
