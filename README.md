# FastRTTI
This is a simple library based on templates that can be used to implement Run-Time Type Information(RTTI).

## Why I need it.
1. Because I need to serialize/deserialize the objects. 
2. Because I cannot use the compiler RTTI. The RTTI makes the code easy to hack.
2. Because I need it in my tests.
 
This is still somewhat a work in progress; it's stable, but there are probably places where can be improved.

## Licensing 
Public Domain. If my licensing is wrong, please let me know. Use at your own risk for whatever you want. Apparently licensing is hard and complicated. If your country doesn't have a public domain, feel free to say you found this on the side of the road. 


## Overview
`FastRTTI` have two simple classes that can used: IRTTI and RTTI . 
The speed tests `FastRTTI` VS `compiler/real RTTI` results are:
```c++
Tested on Visual Studio 2013 (Express) / Visual Studio 2019
FAST RTTI elapsed 8000
REAL RTTI elapsed 34002
FAST RTTI elapsed 8000
REAL RTTI elapsed 33002
FAST RTTI elapsed 9001
REAL RTTI elapsed 33002
FAST RTTI elapsed 7000
REAL RTTI elapsed 34002
FAST RTTI elapsed 8001
REAL RTTI elapsed 33001
FAST RTTI elapsed 7001
REAL RTTI elapsed 33002
FAST RTTI elapsed 10001
REAL RTTI elapsed 33002
FAST RTTI elapsed 9001
REAL RTTI elapsed 33002
FAST RTTI elapsed 8001
REAL RTTI elapsed 33002
FAST RTTI elapsed 7001
REAL RTTI elapsed 31001
Press any key!
```

```c++
Tested on macos:
FAST RTTI elapsed 18628
REAL RTTI elapsed 14150
FAST RTTI elapsed 11968
REAL RTTI elapsed 14172
FAST RTTI elapsed 12078
REAL RTTI elapsed 14358
FAST RTTI elapsed 12086
REAL RTTI elapsed 14193
FAST RTTI elapsed 12157
REAL RTTI elapsed 14372
FAST RTTI elapsed 12045
REAL RTTI elapsed 14001
FAST RTTI elapsed 11996
REAL RTTI elapsed 14139
FAST RTTI elapsed 12087
REAL RTTI elapsed 14112
FAST RTTI elapsed 11973
REAL RTTI elapsed 14128
FAST RTTI elapsed 12039
REAL RTTI elapsed 14469
Press any key!
```


## Example: 

```c++
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
    cout << "The inheritance chain list (available only in debug) \nfor AB_CD_E* obj1 = new AB_CD_E() object is: " << endl;
    for (auto& name : obj1->GetInheritanceChainNames())
    {
        cout << name << endl;
    }
    cout << "Is object of A type? " << (obj1->IsKindOf(IRTTI<A>::GetTypeID()) ? "Yes" : "No") << endl;
    cout << "Is object of E type? " << (obj1->IsKindOf(IRTTI<E>::GetTypeID()) ? "Yes" : "No") << endl;
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
```

The outpus is:
```
The inheritance chain list (available only in debug)
for AB_CD_E* obj1 = new AB_CD_E() object is:
class A *
class B *
class AB *
class C *
class D *
class CD *
class E *
Is object of A type? Yes
Is object of E type? Yes

The inheritance chain list (available only in debug)
for A* obj2 = new AB_CD_E() object is:
class A *
class B *
class AB *
class C *
class D *
class CD *
class E *
Is object of A type? Yes
Is object of E type? Yes

The inheritance chain list (available only in debug)
for AB* obj3 = new AB() object is :
class A *
class B *
Is object of A type? Yes
Is object of E type? No

The inheritance chain list (available only in debug)
for CD* obj4 = new CD() object is:
class C *
class D *
Is object of A type? No
Is object of E type? No

Press any key!
```
