#include <utility>
#include <vector>

#include "FastRTTI/RTTI.hpp"
#include "TestHarness.h"

/** Copies, moves and assignments.
 *
 *  The inheritance chain stores the address of the object each subobject was
 *  registered in. A copy that inherits those entries resolves every dyna_cast back
 *  into the source object, and dangles once the source is gone. The checks below
 *  only compare addresses, so they stay safe to run against a header that has
 *  that defect. */
namespace fastrttitests
{
	class Readable : public fastrtti::IRTTI<Readable>
	{
	};

	class Writable : public fastrtti::IRTTI<Writable>
	{
	};

	/** Two interfaces and the class implementing them, the usual shape of a
	 *  capability lookup through dyna_cast. */
	class Document
		: public virtual Readable
		, public virtual Writable
		, public fastrtti::IRTTI<Document>
	{
	public:

		explicit Document(int pageCount)
			: m_pageCount(pageCount)
		{
		}

		int m_pageCount;
	};

	/** True when every type in Document's chain resolves into document itself. */
	bool CastsIntoItself(Document& document)
	{
		return fastrtti::dyna_cast<Document>(&document) == &document
			&& fastrtti::dyna_cast<Readable>(&document) == static_cast<Readable*>(&document)
			&& fastrtti::dyna_cast<Writable>(&document) == static_cast<Writable*>(&document);
	}

	void TestCopyConstructedObjectCastsIntoItself(TestContext& context)
	{
		// Arrange
		Document original(3);

		// Act
		Document copy(original);

		// Assert
		context.Check(CastsIntoItself(copy), "every cast on a copy resolves into the copy, not the original");
		context.Check(CastsIntoItself(original), "the original still casts into itself");
	}

	void TestMoveConstructedObjectCastsIntoItself(TestContext& context)
	{
		// Arrange
		Document source(3);

		// Act
		Document moved(std::move(source));

		// Assert
		context.Check(CastsIntoItself(moved), "every cast on a moved-to object resolves into that object");
	}

	void TestVectorReallocationKeepsElementsCastable(TestContext& context)
	{
		// Arrange
		const int documentCount = 64;
		std::vector<Document> documents;

		// Act: growing one element at a time reallocates several times, and every
		// reallocation copies or moves each element into new storage.
		for (int i = 0; i < documentCount; ++i)
		{
			documents.emplace_back(i);
		}

		// Assert
		bool haveAllElementsCastIntoThemselves = true;
		for (Document& document : documents)
		{
			if (!CastsIntoItself(document))
			{
				haveAllElementsCastIntoThemselves = false;
			}
		}
		context.Check(haveAllElementsCastIntoThemselves, "every element casts into itself after the vector reallocated");
	}

	void TestCopyAssignedObjectKeepsItsOwnChain(TestContext& context)
	{
		// Arrange
		Document target(1);
		const Document source(2);

		// Act
		target = source;

		// Assert
		context.Check(CastsIntoItself(target), "after copy assignment the target still casts into itself");
		context.Check(target.m_pageCount == 2, "copy assignment still copies the value");
	}

	void TestMoveAssignedObjectKeepsItsOwnChain(TestContext& context)
	{
		// Arrange
		Document target(1);
		Document source(2);

		// Act
		target = std::move(source);

		// Assert
		context.Check(CastsIntoItself(target), "after move assignment the target still casts into itself");
		context.Check(target.m_pageCount == 2, "move assignment still transfers the value");
	}

	void TestSelfAssignedObjectStaysCastable(TestContext& context)
	{
		// Arrange
		Document document(4);
		const Document& sameDocument = document;

		// Act
		document = sameDocument;

		// Assert
		context.Check(CastsIntoItself(document), "after self assignment the object still casts into itself");
	}

	void TestSlicedCopyIsOnlyKindOfItsOwnType(TestContext& context)
	{
		// Arrange
		Document document(5);

		// Act: copying only the Readable part of a Document.
		Readable sliced(document);

		// Assert
		context.Check(!fastrtti::ObjectIsKindOf<Document>(&sliced), "a Readable sliced off a Document is not a Document");
		context.Check(fastrtti::dyna_cast<Readable>(&sliced) == &sliced, "it casts into itself as a Readable");
	}

	void TestAssignmentThroughBaseReferenceKeepsTargetType(TestContext& context)
	{
		// Arrange
		Readable plainReadable;
		Readable& target = plainReadable;
		const Document document(6);

		// Act: assigning only the Readable part of a Document.
		target = document;

		// Assert
		context.Check(!fastrtti::ObjectIsKindOf<Document>(&plainReadable), "a plain Readable does not become a Document by assignment");
		context.Check(fastrtti::dyna_cast<Readable>(&plainReadable) == &plainReadable, "it still casts into itself");
	}
}

int main()
{
	const std::vector<fastrttitests::TestCase> tests =
	{
		{ "CopyConstructedObjectCastsIntoItself", &fastrttitests::TestCopyConstructedObjectCastsIntoItself },
		{ "MoveConstructedObjectCastsIntoItself", &fastrttitests::TestMoveConstructedObjectCastsIntoItself },
		{ "VectorReallocationKeepsElementsCastable", &fastrttitests::TestVectorReallocationKeepsElementsCastable },
		{ "CopyAssignedObjectKeepsItsOwnChain", &fastrttitests::TestCopyAssignedObjectKeepsItsOwnChain },
		{ "MoveAssignedObjectKeepsItsOwnChain", &fastrttitests::TestMoveAssignedObjectKeepsItsOwnChain },
		{ "SelfAssignedObjectStaysCastable", &fastrttitests::TestSelfAssignedObjectStaysCastable },
		{ "SlicedCopyIsOnlyKindOfItsOwnType", &fastrttitests::TestSlicedCopyIsOnlyKindOfItsOwnType },
		{ "AssignmentThroughBaseReferenceKeepsTargetType", &fastrttitests::TestAssignmentThroughBaseReferenceKeepsTargetType },
	};

	return fastrttitests::RunTests(tests);
}
