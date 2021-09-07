#include "glibc-cmp.h"

#if IS_IN (libc)

# ifdef SHARED
#  undef libc_hidden_builtin_def
#  define libc_hidden_builtin_def(name)
# endif

# define USE_SSSE3 1
# define STRCMP __strcmp_ssse3
# include "../strcmp.S"
#endif
