#include <string>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** dyna_cast, ObjectIsKindOf, IsKindOf and GetPtrKindOf across the hierarchy shapes
 *  the library supports: single inheritance, multiple inheritance with pointer
 *  adjustment, virtual and non-virtual diamonds, and class templates. */
namespace fastrttitests
{
	// Single inheritance.

	class Animal : public fastrtti::IRTTI<Animal>
	{
	public:

		virtual std::string GetSound() const
		{
			return "...";
		}
	};

	class Dog : public Animal, public fastrtti::IRTTI<Dog>
	{
	public:

		std::string GetSound() const override
		{
			return "woof";
		}
	};

	class Cat : public Animal, public fastrtti::IRTTI<Cat>
	{
	};

	class Puppy : public Dog, public fastrtti::IRTTI<Puppy>
	{
	};

	/** Derives from a registered class without registering itself. */
	class StrayDog : public Dog
	{
	};

	// Multiple inheritance, with data in every base so a wrong pointer adjustment shows.

	class Named : public fastrtti::IRTTI<Named>
	{
	public:

		Named()
			: m_nameLength(11)
		{
		}

		int m_nameLength;
	};

	class Priced : public fastrtti::IRTTI<Priced>
	{
	public:

		Priced()
			: m_price(250)
		{
		}

		int m_price;
	};

	class Product : public Named, public Priced, public fastrtti::IRTTI<Product>
	{
	};

	// A virtual diamond: Disk and Cache share one Storage subobject.

	class Storage : public fastrtti::IRTTI<Storage>
	{
	public:

		virtual int GetCapacity() const = 0;
	};

	class Disk : public virtual Storage, public fastrtti::IRTTI<Disk>
	{
	};

	class Cache : public virtual Storage, public fastrtti::IRTTI<Cache>
	{
	};

	class HybridDrive : public Disk, public Cache, public fastrtti::IRTTI<HybridDrive>
	{
	public:

		int GetCapacity() const override
		{
			return 2048;
		}
	};

	// A non-virtual diamond: Left and Right each have their own Counter subobject.

	class Counter : public fastrtti::IRTTI<Counter>
	{
	};

	class Left : public Counter, public fastrtti::IRTTI<Left>
	{
	};

	class Right : public Counter, public fastrtti::IRTTI<Right>
	{
	};

	class LeftAndRight : public Left, public Right, public fastrtti::IRTTI<LeftAndRight>
	{
	};

	// A class template: every instantiation is a type of its own.

	template<typename T>
	class Box : public fastrtti::IRTTI<Box<T>>
	{
	};

	void TestDowncastToActualTypeSucceeds(TestContext& context)
	{
		// Arrange
		Dog dog;
		Animal* animal = &dog;

		// Act
		Dog* castResult = fastrtti::dyna_cast<Dog>(animal);

		// Assert
		context.Check(castResult == &dog, "an Animal pointer to a Dog casts back to that Dog");
	}

	void TestDowncastToSiblingTypeFails(TestContext& context)
	{
		// Arrange
		Dog dog;
		Animal* animal = &dog;

		// Act
		Cat* castResult = fastrtti::dyna_cast<Cat>(animal);

		// Assert
		context.Check(castResult == nullptr, "a Dog is not a Cat");
	}

	void TestDowncastBeyondActualTypeFails(TestContext& context)
	{
		// Arrange
		Dog dog;

		// Act
		Puppy* castResult = fastrtti::dyna_cast<Puppy>(&dog);

		// Assert
		context.Check(castResult == nullptr, "a Dog is not a Puppy, the class derived from it");
	}

	void TestCastToIntermediateTypeSucceeds(TestContext& context)
	{
		// Arrange
		Puppy puppy;
		Animal* animal = &puppy;

		// Act
		Dog* castResult = fastrtti::dyna_cast<Dog>(animal);

		// Assert
		context.Check(castResult == static_cast<Dog*>(&puppy), "a Puppy reached as an Animal casts to its Dog part");
	}

	void TestUpcastSucceeds(TestContext& context)
	{
		// Arrange
		Puppy puppy;

		// Act
		Animal* castResult = fastrtti::dyna_cast<Animal>(&puppy);

		// Assert
		context.Check(castResult == static_cast<Animal*>(&puppy), "a Puppy casts up to its Animal part");
	}

	void TestCastToOwnTypeReturnsSameObject(TestContext& context)
	{
		// Arrange
		Cat cat;

		// Act
		Cat* castResult = fastrtti::dyna_cast<Cat>(&cat);

		// Assert
		context.Check(castResult == &cat, "an object casts to its own type");
	}

	void TestVirtualCallThroughCastReachesOverride(TestContext& context)
	{
		// Arrange
		Puppy puppy;
		fastrtti::RTTI* object = &puppy;

		// Act
		Animal* animal = fastrtti::dyna_cast<Animal>(object);

		// Assert
		context.Check(animal != nullptr && animal->GetSound() == "woof", "a call through the cast reaches the override");
	}

	void TestUnregisteredDerivedTypeIsOnlyKindOfItsBases(TestContext& context)
	{
		// Arrange
		StrayDog strayDog;

		// Act
		const bool isKindOfStrayDog = fastrtti::ObjectIsKindOf<StrayDog>(&strayDog);
		Dog* asDog = fastrtti::dyna_cast<Dog>(&strayDog);

		// Assert
		context.Check(!isKindOfStrayDog, "a class that does not derive IRTTI of itself is not known by its own type");
		context.Check(asDog == static_cast<Dog*>(&strayDog), "it is still known by the registered types it derives from");
	}

	void TestCastAdjustsPointerToSecondaryBase(TestContext& context)
	{
		// Arrange
		Product product;

		// Act
		Priced* priced = fastrtti::dyna_cast<Priced>(&product);

		// Assert
		context.Check(priced == static_cast<Priced*>(&product), "the cast lands on the Priced subobject, as static_cast does");
		context.Check(priced != nullptr && priced->m_price == 250, "the data read through the cast is the Priced data");
	}

	void TestCrossCastBetweenUnrelatedBasesSucceeds(TestContext& context)
	{
		// Arrange
		Product product;
		Named* named = &product;

		// Act
		Priced* priced = fastrtti::dyna_cast<Priced>(named);

		// Assert
		context.Check(priced == static_cast<Priced*>(&product), "a Named pointer cross-casts to the Priced side of the same object");
	}

	void TestCastFromSecondaryBaseToMostDerivedSucceeds(TestContext& context)
	{
		// Arrange
		Product product;
		Priced* priced = &product;

		// Act
		Product* castResult = fastrtti::dyna_cast<Product>(priced);

		// Assert
		context.Check(castResult == &product, "a pointer to a secondary base casts back to the whole object");
	}

	void TestVirtualDiamondHasOneSharedBase(TestContext& context)
	{
		// Arrange
		HybridDrive drive;
		Disk* disk = &drive;
		Cache* cache = &drive;

		// Act
		Storage* fromDisk = fastrtti::dyna_cast<Storage>(disk);
		Storage* fromCache = fastrtti::dyna_cast<Storage>(cache);

		// Assert
		context.Check(fromDisk == static_cast<Storage*>(&drive), "Storage is reached from the Disk side");
		context.Check(fromDisk == fromCache, "both sides reach the same Storage subobject");
		context.Check(fromDisk != nullptr && fromDisk->GetCapacity() == 2048, "a call through the shared base reaches the override");
	}

	void TestVirtualDiamondCrossCastsBetweenSides(TestContext& context)
	{
		// Arrange
		HybridDrive drive;
		Disk* disk = &drive;

		// Act
		Cache* cache = fastrtti::dyna_cast<Cache>(disk);

		// Assert
		context.Check(cache == static_cast<Cache*>(&drive), "the Disk side cross-casts to the Cache side");
	}

	/** dynamic_cast would refuse this cast as ambiguous. FastRTTI returns the first
	 *  registered subobject, and this test pins that down so a change is noticed. */
	void TestNonVirtualDiamondCastReturnsFirstBaseSubobject(TestContext& context)
	{
		// Arrange
		LeftAndRight object;

		// Act
		Counter* counter = fastrtti::dyna_cast<Counter>(&object);

		// Assert
		Counter* leftCounter = static_cast<Counter*>(static_cast<Left*>(&object));
		context.Check(counter == leftCounter, "the cast returns the Counter of Left, the first base constructed");
	}

	void TestTemplateInstantiationsAreDistinctTypes(TestContext& context)
	{
		// Arrange
		Box<int> intBox;

		// Act
		Box<int>* asIntBox = fastrtti::dyna_cast<Box<int>>(&intBox);
		Box<float>* asFloatBox = fastrtti::dyna_cast<Box<float>>(&intBox);

		// Assert
		context.Check(asIntBox == &intBox, "a Box<int> casts to Box<int>");
		context.Check(asFloatBox == nullptr, "a Box<int> is not a Box<float>");
	}

	void TestObjectIsKindOfEveryRegisteredBase(TestContext& context)
	{
		// Arrange
		Puppy puppy;
		fastrtti::RTTI* object = &puppy;

		// Act
		const bool isAnimal = fastrtti::ObjectIsKindOf<Animal>(object);
		const bool isDog = fastrtti::ObjectIsKindOf<Dog>(object);
		const bool isPuppy = fastrtti::ObjectIsKindOf<Puppy>(object);
		const bool isCat = fastrtti::ObjectIsKindOf<Cat>(object);

		// Assert
		context.Check(isAnimal && isDog && isPuppy, "a Puppy is an Animal, a Dog and a Puppy");
		context.Check(!isCat, "a Puppy is not a Cat");
	}

	void TestGetPtrKindOfMatchesDynaCast(TestContext& context)
	{
		// Arrange
		Product product;

		// Act
		void* asPriced = product.GetPtrKindOf(fastrtti::IRTTI<Priced>::GetTypeID());
		void* asDog = product.GetPtrKindOf(fastrtti::IRTTI<Dog>::GetTypeID());

		// Assert
		context.Check(asPriced == static_cast<void*>(static_cast<Priced*>(&product)), "GetPtrKindOf returns the same subobject dyna_cast does");
		context.Check(asDog == nullptr, "GetPtrKindOf returns null for a type the object does not have");
	}

	void TestIsKindOfMatchesObjectIsKindOf(TestContext& context)
	{
		// Arrange
		Dog dog;

		// Act
		const bool isAnimal = dog.IsKindOf(fastrtti::IRTTI<Animal>::GetTypeID());
		const bool isCat = dog.IsKindOf(fastrtti::IRTTI<Cat>::GetTypeID());

		// Assert
		context.Check(isAnimal == fastrtti::ObjectIsKindOf<Animal>(&dog), "IsKindOf agrees with ObjectIsKindOf for a base");
		context.Check(isCat == fastrtti::ObjectIsKindOf<Cat>(&dog), "IsKindOf agrees with ObjectIsKindOf for an unrelated type");
		context.Check(isAnimal && !isCat, "a Dog is an Animal and not a Cat");
	}

	void TestIsRttiIsTrue(TestContext& context)
	{
		// Arrange
		Dog dog;

		// Act
		const bool isRtti = dog.IsRTTI();

		// Assert
		context.Check(isRtti, "IsRTTI is true for every registered object");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "DowncastToActualTypeSucceeds", &fastrttitests::TestDowncastToActualTypeSucceeds },
		{ "DowncastToSiblingTypeFails", &fastrttitests::TestDowncastToSiblingTypeFails },
		{ "DowncastBeyondActualTypeFails", &fastrttitests::TestDowncastBeyondActualTypeFails },
		{ "CastToIntermediateTypeSucceeds", &fastrttitests::TestCastToIntermediateTypeSucceeds },
		{ "UpcastSucceeds", &fastrttitests::TestUpcastSucceeds },
		{ "CastToOwnTypeReturnsSameObject", &fastrttitests::TestCastToOwnTypeReturnsSameObject },
		{ "VirtualCallThroughCastReachesOverride", &fastrttitests::TestVirtualCallThroughCastReachesOverride },
		{ "UnregisteredDerivedTypeIsOnlyKindOfItsBases", &fastrttitests::TestUnregisteredDerivedTypeIsOnlyKindOfItsBases },
		{ "CastAdjustsPointerToSecondaryBase", &fastrttitests::TestCastAdjustsPointerToSecondaryBase },
		{ "CrossCastBetweenUnrelatedBasesSucceeds", &fastrttitests::TestCrossCastBetweenUnrelatedBasesSucceeds },
		{ "CastFromSecondaryBaseToMostDerivedSucceeds", &fastrttitests::TestCastFromSecondaryBaseToMostDerivedSucceeds },
		{ "VirtualDiamondHasOneSharedBase", &fastrttitests::TestVirtualDiamondHasOneSharedBase },
		{ "VirtualDiamondCrossCastsBetweenSides", &fastrttitests::TestVirtualDiamondCrossCastsBetweenSides },
		{ "NonVirtualDiamondCastReturnsFirstBaseSubobject", &fastrttitests::TestNonVirtualDiamondCastReturnsFirstBaseSubobject },
		{ "TemplateInstantiationsAreDistinctTypes", &fastrttitests::TestTemplateInstantiationsAreDistinctTypes },
		{ "ObjectIsKindOfEveryRegisteredBase", &fastrttitests::TestObjectIsKindOfEveryRegisteredBase },
		{ "GetPtrKindOfMatchesDynaCast", &fastrttitests::TestGetPtrKindOfMatchesDynaCast },
		{ "IsKindOfMatchesObjectIsKindOf", &fastrttitests::TestIsKindOfMatchesObjectIsKindOf },
		{ "IsRttiIsTrue", &fastrttitests::TestIsRttiIsTrue },
	};

	return fastrttitests::RunTests(tests);
}
