#ifndef FASTRTTI_TESTS_CROSS_MODULE_TYPES_H
#define FASTRTTI_TESTS_CROSS_MODULE_TYPES_H

#include "FastRTTI/RTTI.hpp"

/** Types shared by the test executable and the CrossModulePlugin shared library,
 *  the situation of a plugin handing objects to its host. */

#if defined(_WIN32)
	#if defined(CROSSMODULE_PLUGIN_BUILD)
		#define CROSSMODULE_PLUGIN_API __declspec(dllexport)
	#else
		#define CROSSMODULE_PLUGIN_API __declspec(dllimport)
	#endif
#else
	#define CROSSMODULE_PLUGIN_API __attribute__((visibility("default")))
#endif

namespace fastrttitests
{
	/** A capability interface both modules know about. */
	class Purchasable : public fastrtti::IRTTI<Purchasable>
	{
	public:

		virtual int GetPrice() const = 0;
	};

	/** The library's type id type, which differs between versions of the header. */
	typedef decltype(fastrtti::IRTTI<Purchasable>::GetTypeID()) TypeId;

	/** Creates an object of a type only the plugin knows, implementing Purchasable.
	 * @return the object, to be released with DestroyPluginProduct. */
	CROSSMODULE_PLUGIN_API fastrtti::RTTI* CreatePluginProduct();

	/** Releases an object made by CreatePluginProduct, in the module that allocated it. */
	CROSSMODULE_PLUGIN_API void DestroyPluginProduct(fastrtti::RTTI* product);

	/** @return the type id of Purchasable as the plugin sees it. */
	CROSSMODULE_PLUGIN_API TypeId GetPurchasableTypeIdInPlugin();

	/** Casts object to Purchasable inside the plugin.
	 * @return its price, or -1 when the plugin does not see it as Purchasable. */
	CROSSMODULE_PLUGIN_API int GetPriceInPlugin(fastrtti::RTTI* object);

	/** The price every plugin product reports. */
	static const int k_pluginProductPrice = 42;
}

#endif //FASTRTTI_TESTS_CROSS_MODULE_TYPES_H
