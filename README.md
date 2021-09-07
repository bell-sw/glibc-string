# Collection of glibc-string asm implementations

This is a small part of the glibc that only includes cpu features detection,
GNU indirect functions that select the appropriate implementation,
and optimized asm string versions.

It can be used in programs without any code changes:
```sh
LD_PRELOAD=libglibc-string.so myprogram
```

Or linked to a program directly, either statically or dynamically.

The library installed size: ~212K.

## memchr
* __memchr_avx2
* __memchr_avx2_rtm
* __memchr_evex
* __memchr_evex_rtm
* __memchr_sse2

## memcmp
* __memcmp_avx2_movbe
* __memcmp_avx2_movbe_rtm
* __memcmp_evex_movbe
* __memcmp_sse2
* __memcmp_sse4_1
* __memcmp_ssse3

## memrchr
* __memrchr_avx2
* __memrchr_avx2_rtm
* __memrchr_evex
* __memrchr_sse2

## memset
* __memset_avx2_erms
* __memset_avx2_erms_rtm
* __memset_avx2_unaligned
* __memset_avx2_unaligned_erms
* __memset_avx2_unaligned_erms_rtm
* __memset_avx2_unaligned_rtm
* __memset_avx512_erms
* __memset_avx512_no_vzeroupper
* __memset_avx512_unaligned
* __memset_avx512_unaligned_erms
* __memset_erms
* __memset_evex_erms
* __memset_evex_unaligned
* __memset_evex_unaligned_erms
* __memset_sse2_unaligned
* __memset_sse2_unaligned_erms

## wmemset
* __wmemset_avx2_unaligned
* __wmemset_avx2_unaligned_rtm
* __wmemset_avx512_unaligned
* __wmemset_evex_unaligned
* __wmemset_sse2_unaligned

## memmove
* __memmove_avx512_no_vzeroupper
* __memmove_avx512_unaligned
* __memmove_avx512_unaligned_erms
* __memmove_avx_unaligned
* __memmove_avx_unaligned_erms
* __memmove_avx_unaligned_erms_rtm
* __memmove_avx_unaligned_rtm
* __memmove_erms
* __memmove_evex_unaligned
* __memmove_evex_unaligned_erms
* __memmove_sse2_unaligned
* __memmove_sse2_unaligned_erms
* __memmove_ssse3
* __memmove_ssse3_back

## stpcpy
* __stpcpy_avx2
* __stpcpy_avx2_rtm
* __stpcpy_evex
* __stpcpy_sse2
* __stpcpy_sse2_unaligned
* __stpcpy_ssse3

## stpncpy
* __stpncpy_avx2
* __stpncpy_avx2_rtm
* __stpncpy_evex
* __stpncpy_sse2 (C impl)
* __stpncpy_sse2_unaligned
* __stpncpy_ssse3

## strcat
* __strcat_avx2
* __strcat_avx2_rtm
* __strcat_evex
* __strcat_sse2
* __strcat_sse2_unaligned
* __strcat_ssse3

## strchr
* __strchr_avx2
* __strchr_avx2_rtm
* __strchr_evex
* __strchr_sse2
* __strchr_sse2_no_bsf

## strcmp
* __strcmp_avx2
* __strcmp_avx2_rtm
* __strcmp_evex
* __strcmp_sse2
* __strcmp_sse2_unaligned
* __strcmp_sse42
* __strcmp_ssse3

## strcpy
* __strcpy_avx2
* __strcpy_avx2_rtm
* __strcpy_evex
* __strcpy_sse2
* __strcpy_sse2_unaligned
* __strcpy_ssse3

## strlen
* __strlen_avx2
* __strlen_avx2_rtm
* __strlen_evex
* __strlen_sse2

## strncmp
* __strncmp_avx2
* __strncmp_avx2_rtm
* __strncmp_evex
* __strncmp_sse2
* __strncmp_sse42
* __strncmp_ssse3

## strncpy
* __strncpy_avx2
* __strncpy_avx2_rtm
* __strncpy_evex
* __strncpy_sse2 (C impl)
* __strncpy_sse2_unaligned
* __strncpy_ssse3
