/*********
  credits.cpp

  Copyright (C) 1999-2001 David Joffe

  License: GNU GPL Version 2 (*not* "later versions")
*********/

#include <stdlib.h> // for NULL
#include "djtypes.h"
#include "credits.h"
#include "menu.h"
#include "graph.h"

djImage *pImageCredits = NULL;

struct SMenuItem creditsMenuItems[] =
{
   { false, "        " },
   { true,  "   OK   " },
   { false, "        " },
   { false, NULL }
};
unsigned char creditsMenuCursor[] = { 128, 129, 130, 131, 0 };
CMenu creditsMenu ( "credits.cpp:creditsMenu" );

void InitCredits()
{
	// Load credits bitmap
	pImageCredits = new djImage;
	pImageCredits->Load( FILE_IMG_CREDITS );

	creditsMenu.setSize ( 0 );
	creditsMenu.setItems ( creditsMenuItems );
	creditsMenu.setMenuCursor (creditsMenuCursor);
	creditsMenu.setClrBack( djColor(0,0,0) );
	creditsMenu.setXOffset (200);
	creditsMenu.setYOffset (128);
}

void KillCredits()
{
	djDEL(pImageCredits);
}

void ShowCredits()
{
	// First time?
	if ( !pImageCredits )
		InitCredits();

	// Display credits bitmap
	djgDrawImage( pVisBack, pImageCredits, 0, 0, pImageCredits->Width(), pImageCredits->Height() );
	GraphFlip();

	// Pop up credits menu
	do_menu( &creditsMenu );
}
