#pragma once

#include "Globals.hpp"
#include <vector>

class GlobalsRegistry
{
  public:
    void Add(GlobalRef* pRef);
    void CheckVars();
  public:
    std::vector<GlobalRef*> mGlobals;
};
