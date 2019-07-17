#pragma once

namespace igneous {
#if defined(_DEBUG) || defined(DEBUG)
#define IG_DEBUG 1
#else
#define IG_DEBUG 0
#endif
} // end namespace igneous
