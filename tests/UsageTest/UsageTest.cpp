#include <cstdio>
#include <memory>
#include <string>

#include "FastRTTI/RTTI.hpp"

/** Usage examples of FastRTTI, written as tests. Each one arranges an object, acts
 *  on it through the library and asserts on the result, returning true when it
 *  passed. The file stays self-contained so the sample projects build it as is. */
namespace usagetest
{
    class I1 : public fastrtti::IRTTI<I1>
    {
    public:
        virtual std::string GetI1Name() const = 0;
    };

    class I2 : public fastrtti::IRTTI<I2>
    {
    public:
        virtual std::string GetI2Name() const = 0;
    };

    class I3 : public fastrtti::IRTTI<I3>
    {
    public:
        virtual std::string GetI3Name() const = 0;
    };

    class A
        : public virtual I1
        , public virtual I2
        , public virtual I3
        , public fastrtti::IRTTI<A>
    {
    public:
        std::string GetAName() const { return "A"; }
        std::string GetI1Name() const override { return "I1"; }
        std::string GetI2Name() const override { return "I2"; }
        std::string GetI3Name() const override { return "I3"; }
    };

    class B : public fastrtti::IRTTI<B> { public: std::string GetBName() const { return "B"; } };

    class C : public fastrtti::IRTTI<C> { public: std::string GetCName() const { return "C"; } };

    class D : public fastrtti::IRTTI<D> { public: std::string GetDName() const { return "D"; } };

    class E : public fastrtti::IRTTI<E> { public: std::string GetEName() const { return "E"; } };

    class AB : public A, public B, public fastrtti::IRTTI<AB> {};

    class CD : public C, public D, public fastrtti::IRTTI<CD> {};

    class AB_CD : public AB, public CD, public fastrtti::IRTTI<AB_CD> {};

    class AB_CD_E : public AB, public CD, public E, public fastrtti::IRTTI<AB_CD_E> {};

    template<typename T>
    bool IsKindOf(fastrtti::RTTI& object)
    {
        return object.IsKindOf(fastrtti::IRTTI<T>::GetTypeID());
    }

    bool TestObjectIsKindOfEveryTypeInItsHierarchy()
    {
        // Arrange
        std::unique_ptr<AB_CD_E> object(new AB_CD_E());

        // Act
        const bool isKindOfInterfaces = IsKindOf<I1>(*object) && IsKindOf<I2>(*object) && IsKindOf<I3>(*object);
        const bool isKindOfBases = IsKindOf<A>(*object) && IsKindOf<B>(*object) && IsKindOf<C>(*object)
            && IsKindOf<D>(*object) && IsKindOf<E>(*object);
        const bool isKindOfIntermediates = IsKindOf<AB>(*object) && IsKindOf<CD>(*object);
        const bool isKindOfItself = IsKindOf<AB_CD_E>(*object);

        // Assert
        return isKindOfInterfaces && isKindOfBases && isKindOfIntermediates && isKindOfItself;
    }

    bool TestDynaCastReachesEveryImplementation()
    {
        // Arrange
        std::unique_ptr<AB_CD_E> object(new AB_CD_E());

        // Act
        I1* i1 = fastrtti::dyna_cast<I1>(object.get());
        I2* i2 = fastrtti::dyna_cast<I2>(object.get());
        I3* i3 = fastrtti::dyna_cast<I3>(object.get());
        A* a = fastrtti::dyna_cast<A>(object.get());
        E* e = fastrtti::dyna_cast<E>(object.get());

        // Assert
        return i1 != nullptr && i1->GetI1Name() == "I1"
            && i2 != nullptr && i2->GetI2Name() == "I2"
            && i3 != nullptr && i3->GetI3Name() == "I3"
            && a != nullptr && a->GetAName() == "A"
            && e != nullptr && e->GetEName() == "E";
    }

    bool TestObjectReachedThroughBasePointerKeepsItsIdentity()
    {
        // Arrange
        std::unique_ptr<AB_CD_E> object(new AB_CD_E());
        A* base = object.get();

        // Act
        AB_CD_E* mostDerived = fastrtti::dyna_cast<AB_CD_E>(base);
        E* sibling = fastrtti::dyna_cast<E>(base);

        // Assert
        return mostDerived == object.get() && sibling == static_cast<E*>(object.get());
    }

    bool TestObjectIsNotKindOfTypeOutsideItsHierarchy()
    {
        // Arrange
        std::unique_ptr<AB> ab(new AB());
        std::unique_ptr<CD> cd(new CD());
        std::unique_ptr<AB_CD> abcd(new AB_CD());

        // Act
        const bool isAbKindOfA = IsKindOf<A>(*ab);
        const bool isAbKindOfE = IsKindOf<E>(*ab);
        const bool isCdKindOfA = IsKindOf<A>(*cd);
        const bool isCdKindOfE = IsKindOf<E>(*cd);
        E* abcdAsE = fastrtti::dyna_cast<E>(abcd.get());

        // Assert
        return isAbKindOfA && !isAbKindOfE && !isCdKindOfA && !isCdKindOfE && abcdAsE == nullptr;
    }

    bool TestObjectIsKindOfAgreesWithMemberIsKindOf()
    {
        // Arrange
        std::unique_ptr<AB_CD_E> object(new AB_CD_E());
        std::unique_ptr<AB> ab(new AB());

        // Act
        const bool isObjectKindOfE = fastrtti::ObjectIsKindOf<E>(object.get());
        const bool isAbKindOfE = fastrtti::ObjectIsKindOf<E>(ab.get());

        // Assert
        return isObjectKindOfE == IsKindOf<E>(*object) && isAbKindOfE == IsKindOf<E>(*ab);
    }

    bool TestNullObjectIsNeitherKindOfNorCastable()
    {
        // Arrange
        fastrtti::RTTI* nullObject = nullptr;

        // Act
        const bool isKindOfA = fastrtti::ObjectIsKindOf<A>(nullObject);
        A* castResult = fastrtti::dyna_cast<A>(nullObject);

        // Assert
        return !isKindOfA && castResult == nullptr;
    }
}

int main()
{
    struct UsageCase
    {
        const char* m_name;
        bool (*m_function)();
    };

    const UsageCase usageCases[] =
    {
        { "ObjectIsKindOfEveryTypeInItsHierarchy", &usagetest::TestObjectIsKindOfEveryTypeInItsHierarchy },
        { "DynaCastReachesEveryImplementation", &usagetest::TestDynaCastReachesEveryImplementation },
        { "ObjectReachedThroughBasePointerKeepsItsIdentity", &usagetest::TestObjectReachedThroughBasePointerKeepsItsIdentity },
        { "ObjectIsNotKindOfTypeOutsideItsHierarchy", &usagetest::TestObjectIsNotKindOfTypeOutsideItsHierarchy },
        { "ObjectIsKindOfAgreesWithMemberIsKindOf", &usagetest::TestObjectIsKindOfAgreesWithMemberIsKindOf },
        { "NullObjectIsNeitherKindOfNorCastable", &usagetest::TestNullObjectIsNeitherKindOfNorCastable },
    };

    int failedCount = 0;
    int caseCount = 0;
    for (const UsageCase& usageCase : usageCases)
    {
        const bool hasPassed = usageCase.m_function();
        std::printf("[ %s ] %s\n", hasPassed ? "  OK  " : "FAILED", usageCase.m_name);
        if (!hasPassed)
        {
            ++failedCount;
        }
        ++caseCount;
    }

    std::printf("%d of %d usage tests failed\n", failedCount, caseCount);
    return failedCount == 0 ? 0 : 1;
}
