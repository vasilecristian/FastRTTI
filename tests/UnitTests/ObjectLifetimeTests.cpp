#include <array>
#include <memory>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** How the chain follows an object through its life: built up base by base during
 *  construction, owned by each object separately, and released through RTTI. */
namespace fastrttitests
{
	class Assembly;

	/** Records, from inside its own constructor, what the object already is. */
	class Component : public fastrtti::IRTTI<Component>
	{
	public:

		Component()
			: m_wasComponentDuringConstruction(fastrtti::ObjectIsKindOf<Component>(this))
			, m_wasAssemblyDuringConstruction(fastrtti::ObjectIsKindOf<Assembly>(this))
		{
		}

		bool m_wasComponentDuringConstruction;
		bool m_wasAssemblyDuringConstruction;
	};

	class Assembly : public Component, public fastrtti::IRTTI<Assembly>
	{
	};

	/** Sets a flag when destroyed, to see which destructor ran. */
	class Tracked : public fastrtti::IRTTI<Tracked>
	{
	public:

		explicit Tracked(bool& wasDestroyed)
			: m_wasDestroyed(wasDestroyed)
		{
		}

		~Tracked()
		{
			m_wasDestroyed = true;
		}

	private:

		bool& m_wasDestroyed;
	};

	class Widget : public fastrtti::IRTTI<Widget>
	{
	};

	class Button : public Widget, public fastrtti::IRTTI<Button>
	{
	};

	class Label : public Widget, public fastrtti::IRTTI<Label>
	{
	};

	void TestBaseConstructorSeesOnlyTypesBuiltSoFar(TestContext& context)
	{
		// Arrange and Act: the checks are made by Component's constructor.
		Assembly assembly;

		// Assert
		context.Check(assembly.m_wasComponentDuringConstruction, "inside its constructor the object is already a Component");
		context.Check(!assembly.m_wasAssemblyDuringConstruction, "inside Component's constructor it is not an Assembly yet, as with dynamic_cast");
		context.Check(fastrtti::ObjectIsKindOf<Assembly>(&assembly), "once constructed it is an Assembly");
	}

	void TestDeleteThroughRttiPointerRunsDerivedDestructor(TestContext& context)
	{
		// Arrange
		bool wasDestroyed = false;
		std::unique_ptr<fastrtti::RTTI> object(new Tracked(wasDestroyed));

		// Act
		object.reset();

		// Assert
		context.Check(wasDestroyed, "deleting through RTTI* runs the most derived destructor");
	}

	void TestObjectsOfSameTypeHaveIndependentChains(TestContext& context)
	{
		// Arrange
		std::array<Button, 8> buttons;

		// Act
		bool haveAllCastIntoThemselves = true;
		for (Button& button : buttons)
		{
			if (fastrtti::dyna_cast<Button>(&button) != &button || fastrtti::dyna_cast<Widget>(&button) != static_cast<Widget*>(&button))
			{
				haveAllCastIntoThemselves = false;
			}
		}

		// Assert
		context.Check(haveAllCastIntoThemselves, "every element of an array casts into itself");
	}

	void TestObjectsOfDifferentTypesHaveIndependentChains(TestContext& context)
	{
		// Arrange
		Button button;
		Label label;

		// Act
		Label* buttonAsLabel = fastrtti::dyna_cast<Label>(&button);
		Button* labelAsButton = fastrtti::dyna_cast<Button>(&label);

		// Assert
		context.Check(buttonAsLabel == nullptr && labelAsButton == nullptr, "sibling types do not see each other's registrations");
	}

	void TestObjectsInEveryStorageDurationCastIntoThemselves(TestContext& context)
	{
		// Arrange
		static Button staticButton;
		Button stackButton;
		std::unique_ptr<Button> heapButton(new Button());

		// Act
		Button* fromStatic = fastrtti::dyna_cast<Button>(&staticButton);
		Button* fromStack = fastrtti::dyna_cast<Button>(&stackButton);
		Button* fromHeap = fastrtti::dyna_cast<Button>(heapButton.get());

		// Assert
		context.Check(fromStatic == &staticButton, "a static object casts into itself");
		context.Check(fromStack == &stackButton, "a stack object casts into itself");
		context.Check(fromHeap == heapButton.get(), "a heap object casts into itself");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "BaseConstructorSeesOnlyTypesBuiltSoFar", &fastrttitests::TestBaseConstructorSeesOnlyTypesBuiltSoFar },
		{ "DeleteThroughRttiPointerRunsDerivedDestructor", &fastrttitests::TestDeleteThroughRttiPointerRunsDerivedDestructor },
		{ "ObjectsOfSameTypeHaveIndependentChains", &fastrttitests::TestObjectsOfSameTypeHaveIndependentChains },
		{ "ObjectsOfDifferentTypesHaveIndependentChains", &fastrttitests::TestObjectsOfDifferentTypesHaveIndependentChains },
		{ "ObjectsInEveryStorageDurationCastIntoThemselves", &fastrttitests::TestObjectsInEveryStorageDurationCastIntoThemselves },
	};

	return fastrttitests::RunTests(tests);
}
