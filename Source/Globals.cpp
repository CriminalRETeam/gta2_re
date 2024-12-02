#include "Globals.hpp"

void GlobalsRegistry::Add(GlobalRef* pRef)
{
    mGlobals.push_back(pRef);
}

extern "C"
{
    __declspec(dllexport) GlobalsRegistry* __cdecl GetGlobalsRegistry()
    {
        static GlobalsRegistry globalsReg;
        return &globalsReg;
    }
}

GlobalRef::GlobalRef(void* pVar, u32 addr) : mVar(pVar), mOgAddr(addr)
{
    GetGlobalsRegistry()->Add(this);
}
