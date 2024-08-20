/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (c) 2021-2024, BELLSOFT. All rights reserved.
 *
 * This file provides compatibility macros for building
 * string functions without additional glibc includes.
 */

#ifndef _GLIBC_CMP_H
#define _GLIBC_CMP_H

#define GLRO(name) _##name
#define PROCINFO_DECL

#define USE_MULTIARCH

#define MODULE_NAME libc
#define MODULE_libc 1

#define HAVE_AARCH64_BTI 0
#define HAVE_AARCH64_PAC_RET 0
#define HAVE_AARCH64_SVE_ASM 1

/* Used in aarch64 cpu features */
#ifndef HWCAP2_MOPS
# define HWCAP2_MOPS		(1UL << 43)
#endif

#include <libc-symbols.h>

#endif /* _GLIBC_CMP_H */
