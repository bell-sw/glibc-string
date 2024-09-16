# Collection of glibc optimized asm string implementations

This is a small part of the glibc that only includes cpu features detection,
GNU indirect functions that select an appropriate implementation,
and the following optimized asm string versions:

x86-64-v2
=========

* `memchr`: avx2, avx2_rtm, evex, evex_rtm, sse2

* `memcmp`: avx2_movbe, avx2_movbe_rtm, evex_movbe, sse2

* `memrchr`: avx2, avx2_rtm, evex, sse2

* `memset`: avx2_erms, avx2_erms_rtm, avx2_unaligned, avx2_unaligned_erms,
  avx2_unaligned_erms_rtm, avx2_unaligned_rtm, avx512_erms, avx512_no_vzeroupper,
  avx512_unaligned, avx512_unaligned_erms, erms, evex_unaligned, evex_unaligned_erms,
  sse2_unaligned, sse2_unaligned_erms

* `wmemset`: avx2_unaligned, avx2_unaligned_rtm, avx512_unaligned, evex_unaligned
  sse2_unaligned

* `memcpy`: avx512_no_vzeroupper, avx512_unaligned, avx512_unaligned_erms,
  avx_unaligned, avx_unaligned_erms, avx_unaligned_erms_rtm,
  avx_unaligned_rtm, erms, evex_unaligned, evex_unaligned_erms,
  sse2_unaligned, sse2_unaligned_erms, ssse3

* `memmove`: avx512_no_vzeroupper, avx512_unaligned, avx512_unaligned_erms,
  avx_unaligned, avx_unaligned_erms, avx_unaligned_erms_rtm,
  avx_unaligned_rtm, erms, evex_unaligned, evex_unaligned_erms,
  sse2_unaligned, sse2_unaligned_erms, ssse3

* `stpcpy`: avx2, avx2_rtm, evex, sse2_unaligned

* `stpncpy`: avx2, avx2_rtm, evex, sse2_unaligned

* `strcat`: avx2, avx2_rtm, evex, sse2_unaligned

* `strncat`: avx2, avx2_rtm, evex, sse2_unaligned

* `strchr`: avx2, avx2_rtm, evex, sse2

* `strchrnul`: avx2_rtm, avx2, evex, sse2

* `strrchr`: avx2, avx2_rtm, evex, sse2

* `strcmp`: avx2, avx2_rtm, evex, sse2, sse2_unaligned, sse42

* `strcpy`: avx2, avx2_rtm, evex, sse2_unaligned

* `strlen`: avx2, avx2_rtm, evex, sse2

* `strnlen`: avx2, avx2_rtm, evex, sse2

* `strncmp`: avx2, avx2_rtm, evex, sse2, sse42

* `strncpy`: avx2, avx2_rtm, evex, sse2_unaligned

* `wcslen`: avx2, avx2_rtm, evex, sse4_1

It can be used in programs without any code changes:
```sh
LD_PRELOAD=libglibc-string.so myprogram
```

Or linked to a program directly, either statically or dynamically.

The library size: ~132K.


aarch64
=======
* `memcmp`: advanced SIMD, unaligned accesses
* `memchr`:
  - generic: advanced SIMD, MTE compatible
  - nosimd: AmpereComputing eMAG
* `memrchr`: advanced SIMD, MTE compatible
* `memcpy`, `memmove`:
  - generic (advanced SIMD, unaligned accesses)
  - A64FX
  - SVE (Linux kernels >= v6.2 or 5.14)
  - ThunderX
  - ThunderX2
  - MOPS
* `memset`:
  - generic (unaligned accesses)
  - A64FX
  - AmpereComputing eMAG
  - Kunpeng920
  - ZVA size == 64
  - MOPS
* `strchr`: advanced SIMD, MTE compatible
* `strrchr`: advanced SIMD, MTE compatible
* `strchrnul`: advanced SIMD, MTE compatible
* `strcpy`: advanced SIMD, MTE compatible
* `stpcpy`: advanced SIMD, MTE compatible
* `strcmp`: MTE compatible
* `strncmp`: MTE compatible
* `strlen`:
  - generic: advanced SIMD, MTE compatible
  - asimd: advanced SIMD, not MTE compatible
* `strnlen`: advanced SIMD, MTE compatible

The library size: ~68K.
