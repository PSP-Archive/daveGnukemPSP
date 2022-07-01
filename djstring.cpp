/*
djstring.cpp

Copyright (C) 1998-2002 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mmgr/mmgr.h"

#include "djstring.h"

char *djStrDeepCopy( const char * src )
{
	if (!src)
		return NULL;
	char *newstring = new char[strlen(src) + 1];
	strcpy(newstring, src);
	return newstring;
}

char *djStrDup(const char *src)
{
	if (!src)
		return NULL;
	char *newstring = new char[strlen(src) + 1];
	strcpy(newstring, src);
	return newstring;
}

char *djStrDeepCopy( const char * src, int n )
{
	char * temp;

	if ( NULL == src )
		return NULL;

	// FIXME: If src is shorter than n then allocate less
	temp = new char[ n + 1 ];
	strncpy( temp, src, n );
	temp[n] = '\0';

	return temp;
}

// i starts counting at 1
char* djStrPart( const char *str, int i, const char *delim )
{
	int pos, count, length;

	if ( i < 1 )
		return NULL;

	pos = 0;
	count = 0;
	while ( ( count < i - 1 ) && ( pos < (int)strlen( str ) ) )
	{
		while ( (NULL == strchr( delim, str[pos] )) && (pos < (int)strlen( str )) )
		{
			//	 printf( "[%c]", str[pos] );
			pos++;
		}
		if ( pos < (int)strlen(str) )
		{
			pos++;
			count++;
		}
		//      printf( "\n" );
	}

	// if end of string reached before part i was reached
	if ( count != i - 1 )
		return NULL;

	// Determine length of substring required
	length = 0;
	while ( ((pos+length) < (int)strlen(str)) && (NULL == strchr( delim, str[pos+length] )) )
	{
		length++;
	}

	char * temp = NULL;
	temp = djStrDeepCopy( &str[pos], length );
	//   printf("String found (length %d) is [%s]\n", length, temp );

	return temp;
}

void djStrToLower( char * str )
{
	for ( unsigned int i=0; i<strlen(str); i++ )
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z')) str[i] += 32;
	}
}
