// RTTITest.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <time.h>
#include <vector>
#include <chrono>
#include <stdio.h>

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
        delete m_stringA;
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
        delete m_stringB;
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
        delete m_stringC;
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
        delete m_stringD;
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
        delete m_stringE;
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
        delete m_stringAB;
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
        delete m_stringCD;
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
        delete m_stringAB_CD;
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
        delete m_stringAB_CD_E;
    }
};

int main(int argc, char* argv[])
{
	for (int a = 0; a < 10; a++)
	{
		std::vector<A*> vec;

		int size = 160000;

		for (int i = 0; i < size; i++)
		{
			vec.push_back(new AB_CD_E());
		}

        unsigned long t0 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

#ifdef USE_FAST_RTTI
		for (int i = 0; i < size; i++)
		{
			A* a = vec[i];
			AB_CD_E* ab_cd_e = dyna_cast<AB_CD_E>(a);
		}
        unsigned long elapsedFAST = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() -t0;
		printf("FAST RTTI elapsed %d\n", elapsedFAST);
#endif

        t0 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

#ifdef USE_REAL_RTTI
		for (int i = 0; i < size; i++)
		{
			A* a = vec[i];
			AB_CD_E* ab_cd_e = dynamic_cast<AB_CD_E*>(a);
		}
        unsigned long elapsedREAL = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - t0;
		printf("REAL RTTI elapsed %d\n", elapsedREAL);
#endif

		for (int i = 0; i < size; i++)
		{
			delete vec[i];
		}
		vec.clear();
	}

    printf("Press any key!");
    getchar();
	return 0;
}

