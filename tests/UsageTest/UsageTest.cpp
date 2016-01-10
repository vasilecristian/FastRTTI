#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>

#include "FastRTTI/RTTI.hpp"
using namespace fastrtti;
using namespace std;


class A : public IRTTI<A>{};

class B : public IRTTI<B>{};

class C : public IRTTI<C>{};

class D : public IRTTI<D>{};

class E : public IRTTI<E>{};

class AB : public virtual A, public virtual B, public IRTTI<AB>{};

class CD : public virtual C, public virtual D, public IRTTI<CD>{};

class AB_CD : public virtual AB, public virtual CD, public IRTTI<AB_CD>{};

class AB_CD_E : public virtual AB, public virtual CD, public virtual E, public IRTTI<AB_CD_E>{};

int main(int argc, char* argv[])
{
    AB_CD_E* obj1 = new AB_CD_E();
    cout << "The inheritance chain (only in debug) \nfor AB_CD_E* obj1 = new AB_CD_E() object is: " << endl;
    for (auto& name : obj1->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << obj1->IsKindOf(IRTTI<A>::GetTypeID()) << endl;
    cout << "Is object of E type? " << obj1->IsKindOf(IRTTI<E>::GetTypeID()) << endl;
    delete obj1;
    cout << endl;

    A* obj2 = new AB_CD_E();
    cout << "The inheritance chain (only in debug) \nfor A* obj2 = new AB_CD_E() object is: " << endl;
    for (auto& name : obj2->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << obj2->IsKindOf(IRTTI<A>::GetTypeID()) << endl;
    cout << "Is object of E type? " << obj2->IsKindOf(IRTTI<E>::GetTypeID()) << endl;
    delete obj2;
    cout << endl;


    AB* obj3 = new AB();
    cout << "The inheritance chain (only in debug) \nfor AB* obj3 = new AB() object is : " << endl;
    for (auto& name : obj3->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << obj3->IsKindOf(IRTTI<A>::GetTypeID()) << endl;
    cout << "Is object of E type? " << obj3->IsKindOf(IRTTI<E>::GetTypeID()) << endl;
    delete obj3;
    cout << endl;

    CD* obj4 = new CD();
    cout << "The inheritance chain (only in debug) \nfor CD* obj4 = new CD() object is: " << endl;
    for (auto& name : obj4->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << obj4->IsKindOf(IRTTI<A>::GetTypeID()) << endl;
    cout << "Is object of E type? " << obj4->IsKindOf(IRTTI<E>::GetTypeID()) << endl;
    delete obj4;
    cout << endl;

    cout << "Press any key!" << endl;
    getchar();
    return 0;
}



