/*!
\file    graph.h
\brief   Graphics helpers
\author  David Joffe

Copyright (C) 1998-2001 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/
/*--------------------------------------------------------------------------*/
// graph.h
// David Joffe 1998/12
// replacing the old djgega.cpp graphics interface
/*--------------------------------------------------------------------------*/
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "djgraph.h"
#include "djimage.h"

//! Image file to use for font
#define FILE_IMG_FONT "data/font.tga"

//! Main visual
extern djVisual *pVisMain;
//! Back buffer
extern djVisual *pVisBack;
//! View visual - 3rd buffer for game play display
extern djVisual *pVisView;

extern djImage *g_pFont8x8;

//! Convenience macro to call the sprite draw function for 16x16 sprite b in sprite set a
#define DRAW_SPRITE16(vis,a,b,x,y) djgDrawImage( vis, g_pCurMission->GetSpriteData(a)->m_pImage, ((b)%16)*16,((b)/16)*16, (x),(y), 16,16 )
//! Same as \ref DRAW_SPRITE16 but also uses alpha map for transparency masking
#define DRAW_SPRITE16A(vis,a,b,x,y) djgDrawImageAlpha( vis, g_pCurMission->GetSpriteData(a)->m_pImage, ((b)%16)*16,((b)/16)*16, (x),(y), 16,16 )


//! Initialize the graphics system for the game
extern bool GraphInit( bool bFullScreen, int iWidth, int iHeight );

//! Shut down the graphics system
extern void GraphDone();

//! Flip the back buffer to the front buffer
extern void GraphFlip();

//! Flip the view buffer into the back buffer
extern void GraphFlipView( int iViewWidth );

//! Draw a string of characters from a font bitmap
extern void GraphDrawString( djVisual *pVis, djImage *pImg, int x, int y, const unsigned char *szStr );

void screenshot();
#endif
