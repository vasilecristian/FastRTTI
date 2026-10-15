#include "CrossModule/CrossModuleTypes.h"

namespace fastrttitests
{
	/** Known only inside the plugin. The host reaches it through Purchasable. */
	class PluginProduct : public Purchasable, public fastrtti::IRTTI<PluginProduct>
	{
	public:

		int GetPrice() const override
		{
			return k_pluginProductPrice;
		}
	};

	fastrtti::RTTI* CreatePluginProduct()
	{
		return new PluginProduct();
	}

	void DestroyPluginProduct(fastrtti::RTTI* product)
	{
		delete product;
	}

	int GetPriceInPlugin(fastrtti::RTTI* object)
	{
		Purchasable* purchasable = fastrtti::dyna_cast<Purchasable>(object);
		if (purchasable == nullptr)
		{
			return -1;
		}
		return purchasable->GetPrice();
	}

	TypeId GetPurchasableTypeIdInPlugin()
	{
		return fastrtti::IRTTI<Purchasable>::GetTypeID();
	}
}
