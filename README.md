# Collection of glibc-string asm implementations

This is a small part of the glibc that only includes cpu features detection,
GNU indirect functions that select the appropriate implementation,
and optimized asm string versions.

It can be used in programs without any code changes:
```sh
LD_PRELOAD=libglibc-string.so myprogram
```

Or linked to a program directly, either statically or dynamically.

The library installed size: ~244K.

* `memchr`: avx2, avx2_rtm, evex, evex_rtm, sse2

* `memcmp`: avx2_movbe, avx2_movbe_rtm, evex_movbe, sse2, sse4_1, ssse3

* `memrchr`: avx2, avx2_rtm, evex, sse2

* `memset`: avx2_erms, avx2_erms_rtm, avx2_unaligned, avx2_unaligned_erms,
  avx2_unaligned_erms_rtm, avx2_unaligned_rtm, avx512_erms, avx512_no_vzeroupper, avx512_unaligned
  avx512_unaligned_erms, erms, evex_erms, evex_unaligned, evex_unaligned_erms,
  sse2_unaligned, sse2_unaligned_erms

* `wmemset`: avx2_unaligned, avx2_unaligned_rtm, avx512_unaligned, evex_unaligned
  sse2_unaligned

* `memmove`: avx512_no_vzeroupper, avx512_unaligned, avx512_unaligned_erms,
  avx_unaligned, avx_unaligned_erms, avx_unaligned_erms_rtm,
  avx_unaligned_rtm, erms, evex_unaligned, evex_unaligned_erms,
  sse2_unaligned, sse2_unaligned_erms, ssse3, ssse3_back

* `stpcpy`: avx2, avx2_rtm, evex, sse2, sse2_unaligned, ssse3

* `stpncpy`: avx2, avx2_rtm, evex, sse2 (C impl), sse2_unaligned, ssse3

* `strcat`: avx2, avx2_rtm, evex, sse2, sse2_unaligned, ssse3

* `strncat`: ssse3, sse2_unaligned, avx2, evex, sse2 (C impl), avx2_rtm

* `strchr`: avx2, avx2_rtm, evex, sse2, sse2_no_bsf

* `strchrnul`: avx2_rtm, avx2, evex, sse2

* `strrchr`: avx2, avx2_rtm, evex, sse2

* `strcmp`: avx2, avx2_rtm, evex, sse2, sse2_unaligned, sse42, ssse3

* `strcpy`: avx2, avx2_rtm, evex, sse2, sse2_unaligned, ssse3

* `strlen`: avx2, avx2_rtm, evex, sse2

* `strnlen`: avx2, avx2_rtm, evex, sse2

* `strncmp`: avx2, avx2_rtm, evex, sse2, sse42, ssse3

* `strncpy`: avx2, avx2_rtm, evex, sse2 (C impl), sse2_unaligned, ssse3
