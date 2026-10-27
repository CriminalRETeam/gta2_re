#pragma once

// VC6 leaks the variable of a for loop into the enclosing scope (the loop below is legal in
// standard C++ and an error "redefinition" in VC6). Including this header after all other
// includes of a .cpp restores the standard scope. Never include it from a header: it would
// change the game headers' inline functions.
#ifndef BOT_FOR_SCOPE_FIX
#define BOT_FOR_SCOPE_FIX
#define for if (false) {} else for
#endif
