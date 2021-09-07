/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (c) 2021, BELLSOFT. All rights reserved.
 */

#ifndef _GLIBC_CMP_H
#define _GLIBC_CMP_H

#define GLRO(name) _##name
#define PROCINFO_DECL
#define bool int
#define true 1
#define false 0

#define USE_MULTIARCH

#define MODULE_NAME libc
#define MODULE_libc 1
#define MODULE_libm 0
#define MODULE_libpthread 0
#define MODULE_libsupport 0
#define MODULE_libanl 0
#define MODULE_libnsl 0
#define MODULE_librt 0
#define MODULE_libnss 0
#define MODULE_libnss_files 0
#define MODULE_libutil 0
#define MODULE_testsuite 0
#define MODULE_libresolv 0
#define MODULE_libdl 0
#define MODULE_librt 0
#define MODULE_libmvec 0
#define MODULE_nonlib 0
#define MODULE_ldconfig 0

#include <libc-symbols.h>

#endif /* _GLIBC_CMP_H */
