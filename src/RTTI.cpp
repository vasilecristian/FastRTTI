/************************************************************************/
/*File created on 09.2012 by Cristian Vasile (vasile.cristian@gmail.com)*/
/************************************************************************/

#include "RTTI.h"

namespace fastrtti
{
    int RTTI::s_unicIDIncrement = 0;

    RTTI::RTTI()
    {
    }
    RTTI::~RTTI()
    {
        m_inheritanceChain.clear();
    }

    void* RTTI::IsKindOf(int type)                          
    {   
        return m_inheritanceChain[type];
    }

    void RTTI::PrintInheritanceChain()                          
    {   
        printf("=== ================== ===\n");

        std::map<int, void*>::iterator inheritanceChainIT;
        for ( inheritanceChainIT=m_inheritanceChain.begin() ; inheritanceChainIT != m_inheritanceChain.end(); inheritanceChainIT++ )
            printf("'%d' = %d = %d\n", (*inheritanceChainIT).first, (*inheritanceChainIT).second, (*inheritanceChainIT).first);
    }

    const std::map<int, void*> RTTI::GetInheritanceChain()
    {
        return m_inheritanceChain;
    }


} //namespace fastrtti

