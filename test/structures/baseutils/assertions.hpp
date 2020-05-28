#pragma once
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <list>
#include <stack>
#include <vector>
using namespace std;

#define UNDONE(...) assert(false)  //__VA_ARGS__

// ASSERT is debug only
//

#if defined(__clang_major__)
#define ASSERT(...)                                      \
  _Pragma("clang diagnostic push");                      \
  _Pragma("clang diagnostic ignored \"-Wtype-limits\""); \
  assert(__VA_ARGS__);                                   \
  _Pragma("clang diagnostic pop");
#else
// GCC or others, currently not supported by GCC:
#define ASSERT(...)                                    \
  _Pragma("GCC diagnostic push");                      \
  _Pragma("GCC diagnostic ignored \"-Wtype-limits\""); \
  assert(__VA_ARGS__);                                 \
  _Pragma("GCC diagnostic pop");
#endif
