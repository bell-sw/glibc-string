#include "glibc-cmp.h"

# ifdef SHARED
#  undef libc_hidden_builtin_def
#  define libc_hidden_builtin_def(name)
# endif

#if IS_IN (libc)
# define USE_SSSE3 1
# define STRCMP __strcmp_ssse3
# include "../strcmp.S"
#endif
