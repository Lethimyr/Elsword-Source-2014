/** @file   KProfile.cpp
    @desc   implementation of KProfile
*/

#define _TOSTR1(x)    #x
#define _TOSTR(x)     _TOSTR1(x)
#include _TOSTR(PROJECT_PCH)
#include "KProfile.h"

/*static*/ KTimer   KProfile::s_timer;
