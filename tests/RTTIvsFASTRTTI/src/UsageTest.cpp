#include <stdio.h>
#include <time.h>
#include <vector>


#include "FastRTTI/RTTI.hpp"
using namespace fastrtti;


class A : public IRTTI<A>
{
public:

    int   m_intA;
    char* m_stringA;

    A()
    {
        m_intA = 0; 
        m_stringA = new char[1024];
        sprintf_s(m_stringA, 1024,  "class A '%d'\n", m_intA);
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


class B : public IRTTI<B>
{
public:

    int   m_intB;
    char* m_stringB;

    B()
    {
        m_intB = 0; 
        m_stringB = new char[1024];
        sprintf_s(m_stringB, 1024, "class B '%d'\n", m_intB);
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


class C: public IRTTI<C>
{
public:

    int   m_intC;
    char* m_stringC;

    C()
    {
        m_intC = 0; 
        m_stringC = new char[1024];
        sprintf_s(m_stringC, 1024, "class C '%d'\n", m_intC);
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
    : public IRTTI<D>
{
public:

    int   m_intD;
    char* m_stringD;

    D()
    {
        m_intD = 0; 
        m_stringD = new char[1024];
        sprintf_s(m_stringD, 1024, "class D '%d'\n", m_intD);
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
    : public IRTTI<E>
{
public:

    int   m_intE;
    char* m_stringE;

    E()
    {
        m_intE = 0; 
        m_stringE = new char[1024];
        sprintf_s(m_stringE, 1024,  "class E '%d'\n", m_intE);
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

class AB: public virtual A, public virtual B
    , public IRTTI<AB>
{
public:

    int   m_intAB;
    char* m_stringAB;

    AB()
    {
        m_intAB = 0; 
        m_stringAB = new char[1024];
        sprintf_s(m_stringAB, 1024,  "class AB '%d'\n", m_intAB);
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

class CD: public virtual C, public virtual D
    , public IRTTI<CD>
{
public:

    int   m_intCD;
    char* m_stringCD;

    CD()
    {
        m_intCD = 0; 
        m_stringCD = new char[1024];
        sprintf_s(m_stringCD, 1024,  "class CD '%d'\n", m_intCD);
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

class AB_CD: public virtual AB, public virtual CD
    , public IRTTI<AB_CD>
{
public:

    int   m_intAB_CD;
    char* m_stringAB_CD;

    AB_CD()
    {
        m_intAB_CD = 0; 
        m_stringAB_CD = new char[1024];
        sprintf_s(m_stringAB_CD, 1024,  "class AB_CD '%d'\n", m_intAB_CD);
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


class AB_CD_E: public virtual AB, public virtual CD, public virtual E
    , public IRTTI<AB_CD_E>
{
public:

    int   m_intAB_CD_E;
    char* m_stringAB_CD_E;

    AB_CD_E()
    {
        m_intAB_CD_E = 0; 
        m_stringAB_CD_E = new char[1024];
        sprintf_s(m_stringAB_CD_E, 1024,  "class AB_CD_E '%d'\n", m_intAB_CD_E);
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
	
    printf("Press any key!");
    getchar();
	return 0;
}

