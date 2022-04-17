/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/
#ifndef RTTI_H
#define RTTI_H

#include <map>
#include <string>
#include <array>

#include <iostream>

#if defined(_DEBUG) || defined(DEBUG)
#include <typeinfo>
#endif //DEBUG


#if !defined(RTTI_CHAIN_MAX_SIZE)
#define RTTI_CHAIN_MAX_SIZE 32
#endif

#if !defined(RTTI_ASSERT)
#define RTTI_ASSERT
#endif




namespace fastrtti
{

    /** The RTTI class simulates the run time type identification system.
     * This is the base class for Interface RTTI (aka IRTTI) template.
     * You cannot create objects of this type directly. You need to 
     * create a clas that derive this.*/
    class RTTI
    {

    protected:

        /** used plain table to be the fastest RTTI. */
        std::array<intptr_t, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainID;
        std::array<void*, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainPTR;
        int m_inheritanceChainCounter;

#if defined(_DEBUG) || defined(DEBUG)
        /** This is the table with function names. Only for debugging purpose.
         *  Only in debug mode is used!*/ 
        std::array<std::string, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainNames;
#endif //DEBUG
        
        /** The constructor.*/
        RTTI():m_inheritanceChainCounter(-1)
        {
            for (auto& id : m_inheritanceChainID)
            {
                id = -1;
            }

            for (auto& ptr : m_inheritanceChainPTR)
            {
                ptr = 0;
            }
        }

     public:

        /** The destructor. */
        virtual ~RTTI()
        {
        }


    
        /** This function return the pointer of type typeID in case exists.
        * @param typeID is an int meaning the type ID of the object.
        * @return void* a pointer to an instance of class that have type typeID, Or NULL if 
        *         this instance does not have type typeID.*/
        inline void* GetPtrKindOf(intptr_t typeID)
        {   
            for(int i=0; i<=m_inheritanceChainCounter; i++)
            {
                if(m_inheritanceChainID[i] == typeID)
                    return m_inheritanceChainPTR[i];
            }
            return nullptr;    
        }

        /** This function detects if this instance is of type with typeID.
        * @param typeID is an int meaning the type ID of the object.
        * @return true if this instance have type typeID, Or false if
        *         this instance does not have type typeID.*/
        inline bool IsKindOf(intptr_t typeID)
        {
            return GetPtrKindOf(typeID);
        }

        const std::vector<std::string> GetInheritanceChainNames()
        {
#if defined(_DEBUG) || defined(DEBUG)
            return std::vector<std::string>(m_inheritanceChainNames.begin(), m_inheritanceChainNames.begin() + m_inheritanceChainCounter);
#else
            return std::vector<std::string>();
#endif
        }

		/** This function is a helper. Use this only in DEBUG mode as assert.
        * @return bool that always is true.*/
        inline bool IsRTTI()
        {   
            return true;    
        }

    };


    
    

    /** The IRTTI class template is the interface that must be used for all classes
     * that need to have RTTI features.*/                  
    template<typename T>
    class IRTTI : public virtual RTTI
    {
        static intptr_t m_typeID;
    public:

        /** This function returns the type ID  of this object.
         * @return the type ID  of this object. */
        static intptr_t GetTypeID();

    protected:

        /** The IRTTI constructor. */
        IRTTI()
        {
            m_inheritanceChainCounter++;
            RTTI_ASSERT(m_inheritanceChainCounter < RTTI_CHAIN_MAX_SIZE);
            m_inheritanceChainID[m_inheritanceChainCounter] = GetTypeID();
            m_inheritanceChainPTR[m_inheritanceChainCounter] = (T*)this; //the cast to T* is very important.
#if defined(_DEBUG) || defined(DEBUG)
            m_inheritanceChainNames[m_inheritanceChainCounter] = typeid((T*)this).name();
#endif //DEBUG
        }


        /** The IRTTI destructor. */
        virtual ~IRTTI()
        {
        }
    };

    template<typename T>
    intptr_t IRTTI<T>::m_typeID(0);

    template<typename T>
    intptr_t IRTTI<T>::GetTypeID()
    {
		//std::cout << (intptr_t)&m_typeID << std::endl;
        return (intptr_t)&m_typeID;
    }

    /** Static function that check if the pointer objToTest is kind of T class
     * @param objToTest is an RTTI pointer.
     * @return true if objToTest typeID is equal with T::IRTTI::GetTypeID() */
    template<typename T>
    static bool ObjectIsKindOf(RTTI *objToTest)
    {
        if(objToTest == NULL) 
            return false;

        return objToTest->GetPtrKindOf(IRTTI<T>::GetTypeID()) != NULL;
    }


    /** Static function that is used to cast using RTTI feature.
     * @param objToTest is an RTTI pointer.
     * @return the pointer of type T or null if the object cannot be casted.*/
    template<typename T>
    static T* dyna_cast(RTTI* objToCast)
    {
        if(objToCast == NULL) 
            return NULL;

        return (T*)objToCast->GetPtrKindOf(IRTTI<T>::GetTypeID());
    }
    

    /** Basic Types. This template is used to declare the basic types as Int or Double */
    template<typename T>
    class BasicTypeRTTI: public IRTTI<T>
    {
    public:
        T m_val;

        BasicTypeRTTI(T val): m_val(val) {}

        BasicTypeRTTI(): m_val(0) {}
    };

} //namespace fastrtti

#endif //RTTI_H
