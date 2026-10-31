#include "Object_3C_Pool.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(TObject_3C_Pool*, gObject_3C_Pool, 0x6F8F7C);

MATCH_FUNC(0x52a770)
Object_3C_Pool::~Object_3C_Pool()
{
    mpHead = NULL;
}
