// RTTITest.cpp : Benchmarks FastRTTI's dyna_cast against the compiler's dynamic_cast.
//
// Each run is Arrange (create the objects), Act (time the casts) and Assert (every
// cast must have succeeded). Build with USE_FAST_RTTI, USE_REAL_RTTI or both.

#include <stdio.h>
#include <vector>
#include <chrono>

#ifdef USE_FAST_RTTI
#include "FastRTTI/RTTI.hpp"
using namespace fastrtti;
#endif


class A
#ifdef USE_FAST_RTTI
    : public IRTTI<A>
#endif
{
public:

    int   m_intA;
    char* m_stringA;

    A()
    {
        m_intA = 0; 
        m_stringA = new char[1024];
        sprintf(m_stringA, "class A '%d'\n", m_intA);
    };

    virtual int GetInt()
    {
        return m_intA;
    }

    virtual char* GetString()
    {
        return m_stringA;
    }

    virtual ~A()
    {
        delete[] m_stringA;
    }
};


class B
#ifdef USE_FAST_RTTI
    : public IRTTI<B>
#endif
{
public:

    int   m_intB;
    char* m_stringB;

    B()
    {
        m_intB = 0; 
        m_stringB = new char[1024];
        sprintf(m_stringB, "class B '%d'\n", m_intB);
    };

    virtual int GetInt()
    {
        return m_intB;
    }

    virtual char* GetString()
    {
        return m_stringB;
    }

    virtual ~B()
    {
        delete[] m_stringB;
    }
};


class C
#ifdef USE_FAST_RTTI
    : public IRTTI<C>
#endif
{
public:

    int   m_intC;
    char* m_stringC;

    C()
    {
        m_intC = 0; 
        m_stringC = new char[1024];
        sprintf(m_stringC, "class C '%d'\n", m_intC);
    };

    virtual int GetInt()
    {
        return m_intC;
    }

    virtual char* GetString()
    {
        return m_stringC;
    }

    virtual ~C()
    {
        delete[] m_stringC;
    }
};


class D
#ifdef USE_FAST_RTTI
    : public IRTTI<D>
#endif
{
public:

    int   m_intD;
    char* m_stringD;

    D()
    {
        m_intD = 0; 
        m_stringD = new char[1024];
        sprintf(m_stringD, "class D '%d'\n", m_intD);
    };

    virtual int GetInt()
    {
        return m_intD;
    }

    virtual char* GetString()
    {
        return m_stringD;
    }

    virtual ~D()
    {
        delete[] m_stringD;
    }
};


class E
#ifdef USE_FAST_RTTI
    : public IRTTI<E>
#endif
{
public:

    int   m_intE;
    char* m_stringE;

    E()
    {
        m_intE = 0; 
        m_stringE = new char[1024];
        sprintf(m_stringE, "class E '%d'\n", m_intE);
    };

    virtual int GetInt()
    {
        return m_intE;
    }

    virtual char* GetString()
    {
        return m_stringE;
    }

    virtual ~E()
    {
        delete[] m_stringE;
    }
};

class AB: public A, public B
#ifdef USE_FAST_RTTI
    , public IRTTI<AB>
#endif
{
public:

    int   m_intAB;
    char* m_stringAB;

    AB()
    {
        m_intAB = 0; 
        m_stringAB = new char[1024];
        sprintf(m_stringAB, "class AB '%d'\n", m_intAB);
    };

    virtual int GetInt()
    {
        return m_intAB;
    }

    virtual char* GetString()
    {
        return m_stringAB;
    }

    virtual ~AB()
    {
        delete[] m_stringAB;
    }
};

class CD: public C, public D
#ifdef USE_FAST_RTTI
    , public IRTTI<CD>
#endif
{
public:

    int   m_intCD;
    char* m_stringCD;

    CD()
    {
        m_intCD = 0; 
        m_stringCD = new char[1024];
        sprintf(m_stringCD, "class CD '%d'\n", m_intCD);
    };

    virtual int GetInt()
    {
        return m_intCD;
    }

    virtual char* GetString()
    {
        return m_stringCD;
    }

    virtual ~CD()
    {
        delete[] m_stringCD;
    }
};

class AB_CD: public AB, public CD
#ifdef USE_FAST_RTTI
    , public IRTTI<AB_CD>
#endif
{
public:

    int   m_intAB_CD;
    char* m_stringAB_CD;

    AB_CD()
    {
        m_intAB_CD = 0; 
        m_stringAB_CD = new char[1024];
        sprintf(m_stringAB_CD, "class AB_CD '%d'\n", m_intAB_CD);
    };

    virtual int GetInt()
    {
        return m_intAB_CD;
    }

    virtual char* GetString()
    {
        return m_stringAB_CD;
    }

    virtual ~AB_CD()
    {
        delete[] m_stringAB_CD;
    }
};


class AB_CD_E: public AB, public CD, public E
#ifdef USE_FAST_RTTI
    , public IRTTI<AB_CD_E>
#endif
{
public:

    int   m_intAB_CD_E;
    char* m_stringAB_CD_E;

    AB_CD_E()
    {
        m_intAB_CD_E = 0; 
        m_stringAB_CD_E = new char[1024];
        sprintf(m_stringAB_CD_E, "class AB_CD_E '%d'\n", m_intAB_CD_E);
    };

    virtual int GetInt()
    {
        return m_intAB_CD_E;
    }

    virtual char* GetString()
    {
        return m_stringAB_CD_E;
    }

    virtual ~AB_CD_E()
    {
        delete[] m_stringAB_CD_E;
    }
};

namespace
{
    const int k_objectCount = 160000;
    const int k_runCount = 10;

    long long GetNowInMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    /** Arrange: the objects, reached through a base pointer as a real cast site sees them.
     * @return the objects, to be released with DestroyObjects. */
    std::vector<A*> CreateObjects()
    {
        std::vector<A*> objects;
        objects.reserve(k_objectCount);
        for (int i = 0; i < k_objectCount; i++)
        {
            objects.push_back(new AB_CD_E());
        }
        return objects;
    }

    void DestroyObjects(std::vector<A*>& objects)
    {
        for (A* object : objects)
        {
            delete object;
        }
        objects.clear();
    }

#ifdef USE_FAST_RTTI
    /** Act: down-casts every object with dyna_cast.
     * @return how many casts succeeded. Counting them also keeps the optimizer
     *         from removing the loop being measured. */
    int CountFastCasts(const std::vector<A*>& objects)
    {
        int succeededCount = 0;
        for (A* object : objects)
        {
            if (dyna_cast<AB_CD_E>(object) != nullptr)
            {
                succeededCount++;
            }
        }
        return succeededCount;
    }
#endif

#ifdef USE_REAL_RTTI
    /** Act: down-casts every object with dynamic_cast.
     * @return how many casts succeeded. */
    int CountRealCasts(const std::vector<A*>& objects)
    {
        int succeededCount = 0;
        for (A* object : objects)
        {
            if (dynamic_cast<AB_CD_E*>(object) != nullptr)
            {
                succeededCount++;
            }
        }
        return succeededCount;
    }
#endif

    /** Acts with castFunction, prints its time and asserts that every cast succeeded.
     * @return true when every cast succeeded. */
    bool MeasureCasts(const char* label, int (*castFunction)(const std::vector<A*>&), const std::vector<A*>& objects)
    {
        // Act
        const long long startTime = GetNowInMicroseconds();
        const int succeededCount = castFunction(objects);
        const long long elapsedTime = GetNowInMicroseconds() - startTime;
        printf("%s elapsed %lld\n", label, elapsedTime);

        // Assert
        const bool haveAllSucceeded = succeededCount == static_cast<int>(objects.size());
        if (!haveAllSucceeded)
        {
            printf("FAILED: %s cast %d of %d objects\n", label, succeededCount, static_cast<int>(objects.size()));
        }
        return haveAllSucceeded;
    }
}

int main()
{
    bool haveAllCastsSucceeded = true;

    for (int run = 0; run < k_runCount; run++)
    {
        // Arrange
        std::vector<A*> objects = CreateObjects();

        // Act and Assert
#ifdef USE_FAST_RTTI
        haveAllCastsSucceeded = MeasureCasts("FAST RTTI", &CountFastCasts, objects) && haveAllCastsSucceeded;
#endif
#ifdef USE_REAL_RTTI
        haveAllCastsSucceeded = MeasureCasts("REAL RTTI", &CountRealCasts, objects) && haveAllCastsSucceeded;
#endif

        DestroyObjects(objects);
    }

    printf("Press any key!");
    getchar();
    return haveAllCastsSucceeded ? 0 : 1;
}
