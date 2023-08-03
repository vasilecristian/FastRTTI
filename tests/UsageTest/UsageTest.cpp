#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "FastRTTI/RTTI.hpp"
using namespace fastrtti;
using namespace std;

class I1 : public IRTTI<I1>
{
public:
    virtual void PrintI1() = 0;
};
class I2 : public IRTTI<I2>
{
public:
    virtual void PrintI2() = 0;
};
class I3 : public IRTTI<I3>
{
public:
    virtual void PrintI3() = 0;
};

class A 
    : public virtual I1
    , public virtual I2
    , public virtual I3
    , public IRTTI<A> 
{ 
public: 
    void PrintA() { cout << "class name is 'A'!" << endl; } 
    void PrintI1() override { cout << "class name is 'I1'!" << endl; }
    void PrintI2() override { cout << "class name is 'I2'!" << endl; }
    void PrintI3() override { cout << "class name is 'I3'!" << endl; }
};

class B : public IRTTI<B> { public: void PrintB() { cout << "class name is 'B'!" << endl; } };

class C : public IRTTI<C> { public: void PrintC() { cout << "class name is 'C'!" << endl; } };

class D : public IRTTI<D> { public: void PrintD() { cout << "class name is 'D'!" << endl; } };

class E : public IRTTI<E> { public: void PrintE() { cout << "class name is 'E'!" << endl; } };

class AB : public A, public B, public IRTTI<AB>{};

class CD : public C, public D, public IRTTI<CD>{};

class AB_CD : public AB, public CD, public IRTTI<AB_CD>{};

class AB_CD_E : public AB, public CD, public E, public IRTTI<AB_CD_E>{};

int main(int argc, char* argv[])
{
    AB_CD_E* obj1 = new AB_CD_E();
    cout << "The inheritance chain list (available only in debug) \nfor AB_CD_E* obj1 = new AB_CD_E() object is: " << endl;
    for (auto& name : obj1->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of I1 type? " << (obj1->IsKindOf(IRTTI<I1>::GetTypeID()) ? "Yes" : "No") << endl;
    ((I1*)obj1)->PrintI1();
    cout << "Is object of I2 type? " << (obj1->IsKindOf(IRTTI<I2>::GetTypeID()) ? "Yes" : "No") << endl;
    ((I2*)obj1)->PrintI2();
    cout << "Is object of I3 type? " << (obj1->IsKindOf(IRTTI<I3>::GetTypeID()) ? "Yes" : "No") << endl;
    ((I3*)obj1)->PrintI3();
    cout << "Is object of A type? " << (obj1->IsKindOf(IRTTI<A>::GetTypeID()) ? "Yes" : "No") << endl;
    ((A*)obj1)->PrintA();
    cout << "Is object of E type? " << (obj1->IsKindOf(IRTTI<E>::GetTypeID()) ? "Yes" : "No") << endl;
    ((E*)obj1)->PrintE();
    delete obj1;
    cout << endl;

    A* obj2 = new AB_CD_E();
    cout << "The inheritance chain list (available only in debug) \nfor A* obj2 = new AB_CD_E() object is: " << endl;
    for (auto& name : obj2->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << (obj2->IsKindOf(IRTTI<A>::GetTypeID()) ? "Yes" : "No") << endl;
    cout << "Is object of E type? " << (obj2->IsKindOf(IRTTI<E>::GetTypeID()) ? "Yes" : "No") << endl;
    delete obj2;
    cout << endl;


    AB* obj3 = new AB();
    cout << "The inheritance chain list (available only in debug) \nfor AB* obj3 = new AB() object is : " << endl;
    for (auto& name : obj3->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << (obj3->IsKindOf(IRTTI<A>::GetTypeID()) ? "Yes" : "No") << endl;
    cout << "Is object of E type? " << (obj3->IsKindOf(IRTTI<E>::GetTypeID()) ? "Yes" : "No") << endl;
    delete obj3;
    cout << endl;

    CD* obj4 = new CD();
    cout << "The inheritance chain list (available only in debug) \nfor CD* obj4 = new CD() object is: " << endl;
    for (auto& name : obj4->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << (obj4->IsKindOf(IRTTI<A>::GetTypeID()) ? "Yes" : "No") << endl;
    cout << "Is object of E type? " << (obj4->IsKindOf(IRTTI<E>::GetTypeID()) ? "Yes" : "No") << endl;
    delete obj4;
    cout << endl;

    cout << "Press any key!" << endl;
    getchar();
    return 0;
}



