/*!
\file    djstring.h
\brief   Some string helper functions
\author  David Joffe

Copyright (C) 1998-2002 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/
/*--------------------------------------------------------------------------*/
#ifndef _DJSTRING_H_
#define _DJSTRING_H_

//! Make a deep copy of string. Must be deleted with "delete[]".
extern char * djStrDeepCopy( const char * src );
//! Make a deep copy of string. Must be deleted with "delete[]".
extern char * djStrDup( const char * src );
extern char * djStrDeepCopy( const char * src, int n );

//! Convert string in-place to uppercase
extern void   djStrToLower( char * str );

//! For returning sections of strings using given delimiters. i is 1-based
//! eg djStrPart("a,b;c,d", 3, ",;") should return "c"
//! You are responsible for deleting the string it returns
extern char * djStrPart( const char *str, int i, const char *delim );

#endif
