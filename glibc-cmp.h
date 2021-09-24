/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (c) 2021, BELLSOFT. All rights reserved.
 *
 * This file provides compatibility macros for building
 * string functions without additiona glibc includes.
 */

#ifndef _GLIBC_CMP_H
#define _GLIBC_CMP_H

#define GLRO(name) _##name
#define PROCINFO_DECL

#define USE_MULTIARCH

#define MODULE_NAME libc
#define MODULE_libc 1

#include <libc-symbols.h>

#endif /* _GLIBC_CMP_H */
