#ifndef _LD_CONFIG_H
#define _LD_CONFIG_H

/* Use the internal textdomain used for libc messages.  */
#define PACKAGE _libc_intl_domainname
#define DEFAULT_CHARMAP "ANSI_X3.4-1968" /* ASCII */

/* This must be one higer than the last used LC_xxx category value.  */
//#define __LC_LAST	13

#define RELEASE "development"
#define VERSION "2.33.9000"

#endif
