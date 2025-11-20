#include <reent.h>

// Create a single reentrancy structure
struct _reent _global_reent;

// Make _impure_ptr point to it
struct _reent *_impure_ptr = &_global_reent;