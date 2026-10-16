/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/
#ifndef RTTI_H
#define RTTI_H

#include <map>
#include <string>
#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

#include <iostream>


/** Slots reserved per object for the inheritance chain: one per IRTTI<T> base.
 *
 *  A hierarchy deeper than this does not corrupt memory: RegisterInChain refuses
 *  the registrations that do not fit and reports them through RTTI_ASSERT. The
 *  refused types, the most derived ones, are then not reachable through dyna_cast.
 *
 *  It changes the size of fastrtti::RTTI, so it MUST hold the same value in every
 *  translation unit of a program. Set it from the build system, never per file. */
#if !defined(RTTI_CHAIN_MAX_SIZE)
#define RTTI_CHAIN_MAX_SIZE 32
#endif

/** Diagnostic hook, invoked as RTTI_ASSERT(false); when RegisterInChain refuses a
 *  registration because the chain is already full.
 *
 *  It asserts in a debug build and does nothing otherwise. Define it yourself, before
 *  including this header, to route it into your own assert or log. It is used as a
 *  statement, so both an expression and a do { } while (0) block work. */
#if !defined(RTTI_ASSERT)
#if defined(FASTRTTI_DEBUG) || defined(_DEBUG) || defined(DEBUG)
#include <cassert>
#define RTTI_ASSERT(condition) assert(condition)
#else
#define RTTI_ASSERT(condition) ((void)0)
#endif
#endif

/** Whether the debug table of inheritance chain names is compiled in.
 *
 *  One definition, read by both the member declaration and the write in
 *  RegisterInChain, so the two cannot drift apart.
 *
 *  Define FASTRTTI_CHAIN_NAMES as 0 or 1 yourself to force it either way. It adds a
 *  member to fastrtti::RTTI, so it MUST hold the same value in every translation unit
 *  of a program. Set it from the build system, never per file. */
#if !defined(FASTRTTI_CHAIN_NAMES)
#if defined(FASTRTTI_DEBUG) || defined(_DEBUG) || defined(DEBUG)
#define FASTRTTI_CHAIN_NAMES 1
#else
#define FASTRTTI_CHAIN_NAMES 0
#endif
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
        /** Type ids are a fixed 64 bits rather than intptr_t: on a 32 bit target the
         *  latter would truncate the hash and raise the collision probability by
         *  orders of magnitude. A collision is worse than a failed cast: dyna_cast<A>
         *  would hand back a B subobject reinterpreted as A*. */
        std::array<std::uint64_t, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainID;
        std::array<void*, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainPTR;
        int m_inheritanceChainCounter;

#if FASTRTTI_CHAIN_NAMES
        /** Names of the registered subobjects, for reading a chain in a debugger.
         *
         *  const char* rather than std::string: every entry comes from
         *  IRTTI<T>::GetTypeName(), which returns a string literal, so there is
         *  nothing to own and nothing to copy. */
        std::array<const char*, RTTI_CHAIN_MAX_SIZE> m_inheritanceChainNames;
#endif //FASTRTTI_CHAIN_NAMES

        /** Clears the inheritance chain. Every constructor starts from an empty chain,
         *  the copy and move ones included - see the copy constructor below.
         *
         *  Names are cleared to "" rather than null, so a name read by mistake past
         *  m_inheritanceChainCounter is blank instead of undefined behaviour. */
        void ResetChain()
        {
            m_inheritanceChainCounter = -1;

            for (auto& id : m_inheritanceChainID)
            {
                id = static_cast<std::uint64_t>(-1);
            }

            for (auto& ptr : m_inheritanceChainPTR)
            {
                ptr = 0;
            }

#if FASTRTTI_CHAIN_NAMES
            for (auto& name : m_inheritanceChainNames)
            {
                name = "";
            }
#endif //FASTRTTI_CHAIN_NAMES
        }

        /** The constructor.*/
        RTTI()
        {
            ResetChain();
        }

        /** A copy must NOT inherit the source's chain. m_inheritanceChainPTR stores the
         *  addresses of the subobjects of the object it was built in, so copied entries
         *  would resolve every dyna_cast on the copy back into the source, and dangle
         *  once the source is gone. A std::vector of IRTTI objects hit this on every
         *  reallocation.
         *
         *  Starting from an empty chain works because RTTI is a VIRTUAL base: the most
         *  derived class initialises it exactly once, before any IRTTI<T> subobject.
         *  Each IRTTI<T> copy constructor then registers itself against the new object,
         *  rebuilding the chain in the order a fresh construction would. The same holds
         *  for a sliced copy, which ends up with only the types it really has.
         *  tests/UnitTests/CopySemanticsTests.cpp covers these cases. */
        RTTI(const RTTI&)
        {
            ResetChain();
        }

        /** Moving an identity means nothing, so a move rebuilds exactly like a copy. */
        RTTI(RTTI&&)
        {
            ResetChain();
        }

        /** Assignment transfers a VALUE. The chain describes THIS object's identity and
         *  layout, which assignment does not change, so it is deliberately untouched.
         *  Copying it would make the target resolve into the source, and assigning a
         *  derived object through a base reference would make the target claim the
         *  derived type. */
        RTTI& operator=(const RTTI&)
        {
            return *this;
        }

        RTTI& operator=(RTTI&&)
        {
            return *this;
        }

     public:

        /** The destructor. */
        virtual ~RTTI()
        {
        }


    
        /** This function return the pointer of type typeID in case exists.
        * @param typeID is the type ID of the object.
        * @return void* a pointer to an instance of class that have type typeID, Or NULL if 
        *         this instance does not have type typeID.*/
        inline void* GetPtrKindOf(std::uint64_t typeID)
        {   
            for(int i=0; i<=m_inheritanceChainCounter; i++)
            {
                if(m_inheritanceChainID[i] == typeID)
                {
                    return m_inheritanceChainPTR[i];
                }
            }
            return nullptr;    
        }

        /** This function detects if this instance is of type with typeID.
        * @param typeID is the type ID of the object.
        * @return true if this instance have type typeID, Or false if
        *         this instance does not have type typeID.*/
        inline bool IsKindOf(std::uint64_t typeID)
        {
            return GetPtrKindOf(typeID) != nullptr;
        }

        /** Returns owning copies of the chain names, so the caller can outlive the object.
         * @return one name per registered type, innermost base first. Empty unless
         *         FASTRTTI_CHAIN_NAMES is on. */
        const std::vector<std::string> GetInheritanceChainNames()
        {
#if FASTRTTI_CHAIN_NAMES
            // m_inheritanceChainCounter is the INDEX of the last entry written, so the
            // end of the range is one past it. It is -1 when nothing has registered,
            // which yields an empty vector.
            return std::vector<std::string>(m_inheritanceChainNames.begin(), m_inheritanceChainNames.begin() + m_inheritanceChainCounter + 1);
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
    public:

        /** This function returns the type ID of this object.
         *
         *  The id is derived from T itself, not from the address of a per-type static.
         *  A template static gets a different address in every module it is linked
         *  into (every DLL on Windows, every shared library built with hidden
         *  visibility), so an address based id made every dyna_cast across a module
         *  boundary return nullptr. tests/UnitTests/CrossModule covers this.
         *
         *  The compiler's per-type signature string is hashed rather than typeid(),
         *  so the library works with the compiler's own RTTI switched off.
         *
         *  Same-named types in anonymous namespaces of different translation units get
         *  distinct ids, see HashTypeSignature.
         *
         *  Limitation: types the signature cannot tell apart share an id. That is the
         *  case with clang for same-named local classes in different functions. Give
         *  such types distinct names. tests/UnitTests/TypeIdentityTests.cpp covers this.
         *
         *  @return the type ID of this object. */
        static std::uint64_t GetTypeID();

        /** The name of T, for the debug inheritance chain table.
         *
         *  The compiler's per-type signature is used rather than typeid((T*)this),
         *  which does not compile at all where the compiler's own RTTI is off (clang
         *  and gcc with -fno-rtti), the very builds this library is meant for.
         *
         *  Not wrapped in FASTRTTI_CHAIN_NAMES: a member function of a template that is
         *  never called is never instantiated, so this costs nothing when unused.
         *
         *  @return a name for T. A string literal with static storage duration, so the
         *          pointer stays valid for the whole program. */
        static const char* GetTypeName()
        {
#if defined(_MSC_VER)
            return __FUNCSIG__;
#else
            return __PRETTY_FUNCTION__;
#endif
        }

    private:

        /** FNV-1a over the compiler's signature for this instantiation, which has T
         *  baked into it and is identical in every module built by the same compiler. */
        static std::uint64_t HashTypeSignature()
        {
            const char* const signature = GetTypeName();
            std::uint64_t hash = 1469598103934665603ULL;   // FNV-1a 64 bit offset basis
            for (const char* c = signature; *c != 0; ++c)
            {
                hash ^= static_cast<std::uint64_t>(static_cast<unsigned char>(*c));
                hash *= 1099511628211ULL;                  // FNV-1a 64 bit prime
            }

            // Every compiler spells an anonymous namespace the same in every translation
            // unit, so two units' same-named types would share an id. Such a type never
            // leaves its unit, and IRTTI<T> of it has internal linkage, so each unit has
            // its own copy of this static. Its address tells the units apart, and only
            // these types pay for it: every other id stays the same in every module.
            if (IsInAnonymousNamespace(signature))
            {
                static const char unitTag = 0;
                hash ^= static_cast<std::uint64_t>(reinterpret_cast<std::uintptr_t>(&unitTag));
                hash *= 1099511628211ULL;
            }
            return hash;
        }

        /** @return true when the signature names an anonymous namespace, as the type
         *  itself or in one of its template arguments, as MSVC, clang or gcc spell it. */
        static bool IsInAnonymousNamespace(const char* signature)
        {
            return std::strstr(signature, "`anonymous namespace'") != nullptr
                || std::strstr(signature, "(anonymous namespace)") != nullptr
                || std::strstr(signature, "{anonymous}") != nullptr;
        }

    protected:

        /** Appends this subobject to the chain of the object being constructed.
         * @param address is what dyna_cast<T> returns for the object. */
        void RegisterInChain(T* address)
        {
            // Refuse to grow past the reserve rather than writing off the end of the
            // arrays. The cost of refusing is that dyna_cast to THIS type returns
            // nullptr, a failure callers already handle. The write it replaces was
            // silent memory corruption. The chain fills from the innermost base
            // outwards, so it is the most derived registrations that are lost.
            if ((m_inheritanceChainCounter + 1) >= static_cast<int>(RTTI_CHAIN_MAX_SIZE))
            {
                RTTI_ASSERT(false);
                return;
            }

            m_inheritanceChainCounter++;
            m_inheritanceChainID[m_inheritanceChainCounter] = GetTypeID();
            m_inheritanceChainPTR[m_inheritanceChainCounter] = address;
#if FASTRTTI_CHAIN_NAMES
            m_inheritanceChainNames[m_inheritanceChainCounter] = GetTypeName();
#endif //FASTRTTI_CHAIN_NAMES
        }

        /** The IRTTI constructor. */
        IRTTI()
        {
            RegisterInChain((T*)this); //the cast to T* is very important.
        }

        /** Registers address instead of this, for a T that is not a class derived from
         *  IRTTI<T> but a value held by one, as in BasicTypeRTTI. Casting this to T*
         *  there would point at the IRTTI subobject, not at the value. */
        explicit IRTTI(T* address)
        {
            RegisterInChain(address);
        }

        /** Registers against the NEW object instead of inheriting the source's entry.
         *  RTTI's copy constructor has already emptied the chain by this point. */
        IRTTI(const IRTTI&)
        {
            RegisterInChain((T*)this);
        }

        IRTTI(IRTTI&&)
        {
            RegisterInChain((T*)this);
        }

        /** Assignment leaves the chain alone - see RTTI::operator=. */
        IRTTI& operator=(const IRTTI&)
        {
            return *this;
        }

        IRTTI& operator=(IRTTI&&)
        {
            return *this;
        }


        /** The IRTTI destructor. */
        virtual ~IRTTI()
        {
        }
    };

    template<typename T>
    std::uint64_t IRTTI<T>::GetTypeID()
    {
        // Hashed once per type per module. GetTypeID() is called by every dyna_cast,
        // so hashing on each call would put a walk over a long string on the hot path.
        // The initialisation of a function local static is thread safe from C++11
        // (Visual Studio 2015 onwards).
        static const std::uint64_t id = HashTypeSignature();
        return id;
    }

    /** Static function that check if the pointer objToTest is kind of T class
     * @param objToTest is an RTTI pointer.
     * @return true if objToTest typeID is equal with T::IRTTI::GetTypeID() */
    template<typename T>
    static bool ObjectIsKindOf(RTTI *objToTest)
    {
        if(objToTest == NULL) 
        {
            return false;
        }

        return objToTest->GetPtrKindOf(IRTTI<T>::GetTypeID()) != NULL;
    }


    /** Static function that is used to cast using RTTI feature.
     * @param objToTest is an RTTI pointer.
     * @return the pointer of type T or null if the object cannot be casted.*/
    template<typename T>
    static T* dyna_cast(RTTI* objToCast)
    {
        if(objToCast == NULL) 
        {
            return NULL;
        }

        return (T*)objToCast->GetPtrKindOf(IRTTI<T>::GetTypeID());
    }
    

    /** Basic Types. This template is used to declare the basic types as Int or Double.
     *  dyna_cast<T> on it returns the address of m_val. */
    template<typename T>
    class BasicTypeRTTI: public IRTTI<T>
    {
    public:
        T m_val;

        // Taking the address of m_val before it is initialised is fine: only the
        // address is stored, it is not read through until m_val is constructed.
        BasicTypeRTTI(T val): IRTTI<T>(&m_val), m_val(val) {}

        BasicTypeRTTI(): IRTTI<T>(&m_val), m_val(0) {}

        /** A copy registers its OWN m_val, not the source's - see IRTTI's copy constructor. */
        BasicTypeRTTI(const BasicTypeRTTI& other): RTTI(), IRTTI<T>(&m_val), m_val(other.m_val) {}

        BasicTypeRTTI(BasicTypeRTTI&& other): RTTI(), IRTTI<T>(&m_val), m_val(other.m_val) {}

        BasicTypeRTTI& operator=(const BasicTypeRTTI& other)
        {
            m_val = other.m_val;
            return *this;
        }

        BasicTypeRTTI& operator=(BasicTypeRTTI&& other)
        {
            m_val = other.m_val;
            return *this;
        }
    };

} //namespace fastrtti

#endif //RTTI_H
