/*
 * File:    sys_defs.h
 * Created: 2002-06-29 (Saturday), 11:15
 * Author: Vytautas Shaltenis, a.k.a. rtfb
 *
 * Project: Dave Gnukem
 *
 * Description: Main system definitions.
 *
 */

#ifndef SYS_DEFS_H_KRANKLYS__
#define SYS_DEFS_H_KRANKLYS__



extern "C"
{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>
#ifdef WIN32
#include <direct.h> // getcwd etc
#else
#include <unistd.h>
#endif
}

#include <math.h>
#include <float.h>


// This should be defined in limits.h, but current MinGW distribution does
// not have it. So i make it same as in vc6 and bcb5
#ifndef OPEN_MAX
#define OPEN_MAX	32
#endif




#define SYS_MAX_FULL_PATH	1024	// Don't forget these are with
#define SYS_MAX_PATH		128	// terminating zeroes. So the real
#define SYS_MAX_FILE		32	// length is (SYS_MAX_* -1)
#define SYS_MAX_EXT		5


#endif   // #ifndef SYS_DEFS_H_KRANKLYS__
