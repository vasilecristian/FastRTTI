/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/
#ifndef RTTI_H
#define RTTI_H

#include <map>

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

#if defined(_DEBUG) || defined(DEBUG)
        /** This is the table with function names. Only for debugging purpose.
         *  Only in debug mode is used!
         */ 
        std::string m_inheritanceChainNames[RTTI_CHAIN_MAX_SIZE];
#endif //DEBUG
        
        /**
        * The constructor.
        */
        RTTI():m_inheritanceChainCounter(-1)
        {
            memset(m_inheritanceChainID, -1, sizeof(int)*RTTI_CHAIN_MAX_SIZE);
            memset(m_inheritanceChainPTR, NULL, sizeof(void*)*RTTI_CHAIN_MAX_SIZE);   
        }

     public:

        /**
        * The destructor.
        */
        virtual ~RTTI()
        {
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
            for(int i=0; i<=m_inheritanceChainCounter; i++)
            {
                if(m_inheritanceChainID[i] == typeID)
                    return m_inheritanceChainPTR[i];
            }
            return NULL;    
        }


		/**
        * This function is a helper. Use this only in DEBUG mode as assert.
        *
        * @return bool that always is true.
        */
        inline bool IsRTTI()
        {   
            return true;    
        }

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
            m_inheritanceChainCounter++;
            RTTI_ASSERT(m_inheritanceChainCounter < RTTI_CHAIN_MAX_SIZE);
            m_inheritanceChainID[m_inheritanceChainCounter] = GetTypeID();
            m_inheritanceChainPTR[m_inheritanceChainCounter] = (T*)this; //the cast to T* is very important.
#if defined(_DEBUG) || defined(DEBUG)
            m_inheritanceChainNames[m_inheritanceChainCounter] = typeid((T*)this).name();
#endif //DEBUG
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
    static T* fastrtti_dynamic_cast(RTTI* objToCast)
    {
        if(objToCast == NULL) 
            return NULL;

        return (T*)objToCast->IsKindOf(T::IRTTI::GetTypeID());
    }
    #define custom_dynamic_cast fastrtti_dynamic_cast


    /**
     * Basic Types. This template is used to declare the basic types as Int or Double
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