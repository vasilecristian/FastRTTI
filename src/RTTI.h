/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/
#ifndef RTTI_H
#define RTTI_H

#include <map>

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
        * unic incremental ID generator.
        */
        static int s_unicIDIncrement;

        /**
        * Is the map used as inheritance chain. The map must be fast because it's key is an int.
        * All the pears from this map represent all the types of an instance.
        */
        std::map<int, void*> m_inheritanceChain;
        
        /**
        * The constructor.
        */
        RTTI();

     public:
        /**
        * The destructor.
        */
        virtual ~RTTI();

    
        /**
        * This function detects if this instance is of type type.
        *
        * @param type is an int meaning the type ID of the object.
        * @return void* a pointer to an instance of class that have type type, Or NULL if 
        *         this instance does not have type type.
        */
        void* IsKindOf(int type);

        /**
        * This function prints in console the content of inheritance chain.
        */
        virtual void PrintInheritanceChain();

        /**
        * Return the std::map with the inheritance chain.
        */
        const std::map<int, void*> GetInheritanceChain();

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
            m_inheritanceChain[GetTypeID()] = (T*)this;
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