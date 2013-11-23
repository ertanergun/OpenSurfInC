#ifndef UTILS_H
#define UTILS_H

#include "ipoint.h"
#include "cxtypes.h"

//! Round float to nearest integer
_inline int fRound(float flt)
{
  return (int) floor(flt+0.5f);
}

#endif