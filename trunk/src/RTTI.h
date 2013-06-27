/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/
#ifndef RTTI_H
#define RTTI_H

#include <map>

#if !defined(RTTI_CHAIN_MAX_SIZE)
#define RTTI_CHAIN_MAX_SIZE 32
#endif

namespace fastrtti
{

    /**
     * The RTTI class simulates the run time type identification system.
     * This is the base class for Interface RTTI (aka IRTTI) template.
     * You cannot create objects of this type directly. You need to 
     * derive this.
     */
    class RTTI
    {

    protected:

        /**
         * Unique incremental ID generator.
         * Each class will increment this at the first instance created.
         */
        static int s_unicIDIncrement;

        /**
         * used plain table to be the fastest RTTI.
         */
        int m_inheritanceChainID[RTTI_CHAIN_MAX_SIZE];
        void* m_inheritanceChainPTR[RTTI_CHAIN_MAX_SIZE];
        int m_inheritanceChainCounter;
        
        /**
        * The constructor.
        */
        RTTI():m_inheritanceChainCounter(-1)
        {
            memset(m_inheritanceChainID, -1, RTTI_CHAIN_MAX_SIZE);
            memset(m_inheritanceChainPTR, -1, RTTI_CHAIN_MAX_SIZE);   
        }

     public:

        /**
        * The destructor.
        */
        virtual ~RTTI()
        {
            //m_inheritanceChain.clear();
        }


    
        /**
        * This function detects if this instance is of type with typeID.
        *
        * @param typeID is an int meaning the type ID of the object.
        * @return void* a pointer to an instance of class that have type typeID, Or NULL if 
        *         this instance does not have type typeID.
        */
        inline void* IsKindOf(int typeID)
        {   
            for(int i=0; i<m_inheritanceChainCounter; ++i)
            {
                if(m_inheritanceChainID[i] == typeID)
                    return m_inheritanceChainPTR[i];
            }
            return NULL;    
            //return m_inheritanceChain[typeID];
        }

        /**
        * This function prints in console the content of inheritance chain.
        */
        inline void PrintInheritanceChain()
        {   
            //std::map<int, void*>::iterator inheritanceChainIT;
            //for ( inheritanceChainIT=m_inheritanceChain.begin() ; inheritanceChainIT != m_inheritanceChain.end(); inheritanceChainIT++ )
            //    printf("'%d' = %d = %d\n", (*inheritanceChainIT).first, (*inheritanceChainIT).second, (*inheritanceChainIT).first);
        }

        /**
        * Return the std::map with the inheritance chain.
        */
        //inline const std::map<int, void*> GetInheritanceChain()
        //{
        //    return m_inheritanceChain;
        //}

    };


    
    

    /**
     * The IRTTI class template is the RTTI interface that must be used for all classes
     * that need to have RTTI features.
     */                  
    template<typename T>
    class IRTTI : public virtual RTTI
    {
    public:

        /**
         * This function returns the type ID  of this object.
         *
         * @return the type ID  of this object.
         */
        static int GetTypeID()
        {
            static int m_typeID = 0;
            static bool m_typeIDReady = false;
            
            if(!m_typeIDReady)
            {
                m_typeID = s_unicIDIncrement++;
                m_typeIDReady = true;
            }
            return m_typeID;
        }


    protected:

        /**
         * The IRTTI constructor.
         */
        IRTTI()
        {
            m_inheritanceChainID[++m_inheritanceChainCounter] = GetTypeID();
            m_inheritanceChainPTR[m_inheritanceChainCounter] = this;
            
            //m_inheritanceChain[GetTypeID()] = (T*)this;
        }

        /**
         * The IRTTI destructor.
         */
        virtual ~IRTTI()
        {
        }
    };



    /**
     * Static function that check if the pointer objToTest is kind of T class
     *
     * @param objToTest is an RTTI pointer.
     * @return true if objToTest typeID is equal with T::IRTTI::GetTypeID()
     */
    template<typename T>
    static bool ObjectIsKindOf(RTTI *objToTest)
    {
        if(objToTest == NULL) 
            return false;

        return objToTest->IsKindOf(T::IRTTI::GetTypeID()) != NULL;
    }


    /**
     * Static function that is used to cast using RTTI feature.
     *
     * @param objToTest is an RTTI pointer.
     * @return the pointer of type T or null if the object cannot be casted.
     */
    template<typename T>
    static T* custom_dynamic_cast(RTTI* objToCast)
    {
        if(objToCast == NULL) 
            return NULL;

        return (T*)objToCast->IsKindOf(T::IRTTI::GetTypeID());
    }


    /**
     * Basic Types
     */
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