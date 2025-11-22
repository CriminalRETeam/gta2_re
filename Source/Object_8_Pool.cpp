#include "Object_8_Pool.hpp"
#include "Globals.hpp"

DEFINE_GLOBAL(Object_8_Pool*, gObject_8_Pool_6F8F78, 0x6F8F78);

MATCH_FUNC(0x52a750)
Object_8_Pool::~Object_8_Pool()
{
    field_0 = 0;
}