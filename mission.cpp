/*--------------------------------------------------------------------------*/
/* David Joffe '99/02 */
/* Level sets, campaigns, missions, whatever you want to call 'em */

/*
mission.cpp

Copyright (C) 1999-2002 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/
/*--------------------------------------------------------------------------*/
#include "datadir.h"
#include "mission.h"
#include "djlog.h"
#include "djstring.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mmgr/nommgr.h"
#include <string>
#include <fstream>
using namespace std;
#include "mmgr/mmgr.h"

vector<CMission * > g_apMissions;
// FIXME: Does this stuff really belong here? Probably not.
CMission * g_pCurMission = NULL;
/*--------------------------------------------------------------------------*/
int LoadMissions( const char * szfilename )
{
	if (szfilename==NULL) return -1; // NULL string
	if (szfilename[0]==0) return -2; // empty string
	FILE * fin;
	char buf[1024];

	// open file
	// FIXME: We need a consistent way to handle "DATA_DIR"
	if ( NULL == ( fin = fopen( szfilename, "r" ) ) )
	{
		djMSG("ERROR: LoadMissions(%s): Unable to open file.\n", szfilename);
		return -3;
	}

	// Read the list of available mission filenames from the "mission registry"
	fgets( buf, sizeof(buf), fin );
	djStripCRLF(buf); // strip CR/LF characters
	while (!feof( fin ))
	{
		CMission * pMission;
		if ( strlen(buf) > 1 )
		{
			pMission = new CMission;

			// Load this mission and add to list (FIXME: ERROR CHECK)
			pMission->Load( buf );
			g_apMissions.push_back(pMission);
		}

		fgets( buf, sizeof(buf), fin );
		djStripCRLF(buf); // strip CR/LF characters
	}

	fclose( fin );

	if (g_apMissions.size()<1)
	{
		djMSG("ERROR: LoadMissions(%s): No valid missions loaded from file.\n", szfilename);
		return -1;
	}

	// Set current mission by default to the first one in the list
	g_pCurMission = g_apMissions[0];
	return 0;
}
/*--------------------------------------------------------------------------*/
CMission::CMission()
{
	int i;
	for ( i=0; i<NUM_SPRITE_DATA; i++ )
	{
		m_apSpriteData[i] = NULL;
	}
}

CMission::~CMission()
{
	int i;

	for ( i=0; i<m_apLevels.size(); i++ )
	{
		djDEL(m_apLevels[i]);
	}
	m_apLevels.clear();

	for ( i=0; i<NUM_SPRITE_DATA; i++ )
	{
		djDEL(m_apSpriteData[i]);
	}
}

int CMission::Load( char * szfilename )
{
	if (szfilename==NULL) return -1; // NULL string
	if (szfilename[0]==0) return -2; // empty string

	ifstream	fin;
	string		line;
	int			state;
	char		filename[2048];

	SYS_Debug ( "CMission::Load( %s ): Loading ...\n", szfilename );

	sprintf( filename, "%s%s", DATA_DIR, szfilename );
	// open file
	fin.open ( filename );
	if ( !fin.is_open() )
	{
		SYS_Error ( "CMission::Load( %s ) Unable to open file.\n", szfilename );
		return -3;
	}

	state = 0;
	while ( getline ( fin, line ) )
	{
		switch (state)
		{
			///////////////////////////////////////////////////////////////
		case 0: // name
			SetName( line.c_str() );
			SYS_Debug ( "CMission::Load(): Name: %s\n", GetName() );
			state = 1;
			SYS_Debug ( "CMission::Load(): Description:\n", line.c_str() );
			break;
			///////////////////////////////////////////////////////////////
		case 1: // description
			if ( 0 == strcmp( line.c_str(), "~" ) )
			{
				state = 2;
				SYS_Debug ( "-------------\n" );
			}
			else
			{
				// add this part of string to description
				SYS_Debug ( "%s\n", line.c_str() );
			}
			break;
			///////////////////////////////////////////////////////////////
		case 2: // level filenames, names, authors
			if ( 0 == strcmp( line.c_str(), "~" ) )
			{
				state = 3;
			}
			else
			{
				CLevel * pLevel;
				pLevel = new CLevel;
				char *szFilename, *szName, *szAuthor, *szBackground;
				// Interpret line of level information

				szFilename   = djStrPart( line.c_str(), 1, "," );
				szBackground = djStrPart( line.c_str(), 2, "," );
				szName       = djStrPart( line.c_str(), 3, "," );
				szAuthor     = djStrPart( line.c_str(), 4, "," );

				SYS_Debug ( "CMission::Load(): Level: [%s][%s][%s]\n", szFilename, szName, szAuthor );

				pLevel->SetFilename(szFilename);
				pLevel->m_szName		= djStrDeepCopy(szName);
				pLevel->m_szBackground	= djStrDeepCopy(szBackground);
				pLevel->m_szAuthor		= djStrDeepCopy(szAuthor);
				AddLevel( pLevel );

				djDELV(szFilename);
				djDELV(szName);
				djDELV(szAuthor);
				djDELV(szBackground);
			}
			break;
			///////////////////////////////////////////////////////////////
		case 3: // sprites
			if ( 0 == strcmp( line.c_str(), "~" ) )
			{
				state = 4;
			}
			else
			{
				char * szID, * szDatafile, *szImgFilename;
				CSpriteData * pSpriteData;

				pSpriteData = new CSpriteData;

				szID          = djStrPart( line.c_str(), 1, "," );
				szImgFilename = djStrPart( line.c_str(), 2, "," );
				szDatafile    = djStrPart( line.c_str(), 3, "," );

				SYS_Debug ( "CMission::Load(): Sprites: [%s][%s][%s]\n", szID, szImgFilename, szDatafile );
				// Should this happen? NO! LATER! Then must remember names!
				//	       pSprites->LoadSpriteImage(
				pSpriteData->m_iID            = atoi( szID );
				pSpriteData->m_szImgFilename  = djStrDeepCopy( szImgFilename );
				pSpriteData->m_szFilenameData = djStrDeepCopy( szDatafile );
				AddSpriteData( pSpriteData->m_iID, pSpriteData );

				djDELV( szID );
				djDELV( szImgFilename );
				djDELV( szDatafile );
			}
			break;
		}
	} // while ( getline() )

	if ( 4 != state )
	{
		SYS_Error( "CMission::Load( %s ): Bad end state %d\n", szfilename, state );
	}

	fin.close ();

	return 0;
}

void CMission::AddLevel( CLevel * pLevel )
{
	m_apLevels.push_back( pLevel );
}

CLevel *CMission::GetLevel( int i )
{
	return m_apLevels[i];
}

int CMission::NumLevels()
{
	return m_apLevels.size();
}

void CMission::AddSpriteData( int iID, CSpriteData * pSprites )
{
	TRACE( "CMission::AddSpriteData( %d )\n", iID );
	if ( m_apSpriteData[ iID ] != NULL )
	{
		delete m_apSpriteData[ iID ];
	}
	m_apSpriteData[ iID ] = pSprites;
}

CSpriteData * CMission::GetSpriteData( int iID )
{
	// FIXME: Error check here
	return m_apSpriteData[ iID ];
}

int CMission::LoadSprites()
{
	int i;

	// i iterates through the 256 possible ID's for spritesets
	for ( i=0; i<256; i++ )
	{
		CSpriteData * pSpriteData;

		pSpriteData = g_pCurMission->GetSpriteData( i );
		if ( pSpriteData != NULL )
		{
			// FIXME: WHERE IS OUR ERROR CHECKING!?!?!?
			// Load spriteset (the sprite image)
			pSpriteData->LoadSpriteImage();

			// Load data (flags etc associated with sprites)
			pSpriteData->LoadData( pSpriteData->m_szFilenameData );

		} // if
	} // i

	return 0;
}

int CMission::SaveSprites()
{
	int i;
	int nRet = 0;

	// i iterates through the 256 possible ID's for spritesets
	for ( i=0; i<256; i++ )
	{
		CSpriteData * pSpriteData;

		pSpriteData = g_pCurMission->GetSpriteData( i );
		if ( pSpriteData != NULL ) // It *can* be NULL
		{
			char szFilename[1024];
			// Save sprite data file
#ifdef DATA_DIR
			sprintf( szFilename, "%s%s", DATA_DIR, pSpriteData->m_szFilenameData );
#else
			sprintf( szFilename, "%s", pSpriteData->m_szFilenameData );
#endif
			if (nRet>=0)
				nRet = pSpriteData->SaveData( szFilename );

		} // if
	}

	return nRet;
}


void InitMissionSystem()
{
}

void KillMissionSystem()
{
	vector<CMission*>::iterator	i;

	for ( i=g_apMissions.begin(); i!=g_apMissions.end(); ++i )
	{
		if ( NULL != *i )
			delete *i;
	}
	g_apMissions.clear();
}

/*--------------------------------------------------------------------------*/
CLevel::CLevel()
{
	m_szFilename = NULL;
	m_szName = NULL;
	m_szAuthor = NULL;
	m_szBackground = NULL;
}

CLevel::~CLevel()
{
	djDELV(m_szFilename);
	djDELV(m_szName);
	djDELV(m_szAuthor);
	djDELV(m_szBackground);
}

void CLevel::SetFilename( const char *szFilename )
{
	djDELV(m_szFilename);
	m_szFilename = djStrDeepCopy( szFilename );
}

/*--------------------------------------------------------------------------*/
CSpriteData::CSpriteData()
{
	m_iID = -1;
	m_pImage = NULL;
	m_szImgFilename = NULL;
	m_szFilenameData = NULL;
	memset(m_extras, 0, sizeof(m_extras));
	memset(m_type, 0, sizeof(m_type));
	memset(m_color, 0, sizeof(m_color));
}

CSpriteData::~CSpriteData()
{
	djDEL(m_pImage);
	djDELV(m_szImgFilename);
	djDELV(m_szFilenameData);
}

int CSpriteData::LoadData( const char *szFilename )
{
	FILE	*fin;
	int		i, j;
	int		temp;

	SYS_Debug ( "CSpriteData::LoadData( %s ): Loading ...\n", szFilename );

	// open the file
	if (NULL == (fin = fopen( szFilename, "r" )))
	{
#ifdef DATA_DIR
		char buf[1024];
		sprintf( buf, "%s%s", DATA_DIR, szFilename );
		if (NULL == (fin = fopen( buf, "r" )))
#endif
		{
			SYS_Error ( "CSpriteData::LoadData( %s ): Error opening file\n", szFilename );
			return -1;
		}
	}

	// Read "128"
	fscanf ( fin, "%i", &temp );

	for ( i=0; i<128; i++ )
	{
		// Read type
		fscanf ( fin, "%i", &temp );
		if (feof(fin))
			goto error;

		m_type[i] = temp;

		// Read extras
		for ( j=0; j<11; j++ )
		{
			fscanf ( fin, "%i,", &temp );
			m_extras[i][j] = temp;
		}
		fscanf ( fin, "%i", &temp );
		m_extras[i][11] = temp;

		// FIXME: FOLLOWING OLD COLOR OBSOLETE
		// Read block color
		fscanf ( fin, "%i", &temp );
		if (feof(fin))
			goto error;

		m_color[i] = temp;


		// Calculate color from sprite by averaging the 16x16 array of pixels
		if (m_pImage)
		{
			int iIndexX = (i%16)*16; // x offset into image
			int iIndexY = (i/16)*16; // y offset into image
			int r=0,g=0,b=0;
			for ( j=0; j<16; j++ )
			{
				for ( int k=0; k<16; k++ )
				{
					djColor clr = m_pImage->GetPixelColor(k+iIndexX, j+iIndexY);
					r += (int)clr.r;
					g += (int)clr.g;
					b += (int)clr.b;
				}
			}
			r /= 256;
			g /= 256;
			b /= 256;
			m_Color[i] = djColor((unsigned char)r,(unsigned char)g,(unsigned char)b);
		}
	}

	fclose( fin );
	return 0;

error:
	fclose( fin );
	SYS_Error ( "CSpriteData::LoadData( %s ): Error reading data.\n", szFilename );
	return -2;
}

int CSpriteData::SaveData( const char *szFilename )
{
	FILE * fout;
	int    i, j;

	TRACE( "CSpriteData::SaveData( %s ): Saving ...\n", szFilename );

	if (NULL == (fout = fopen( szFilename, "w" )))
	{
		djMSG( "CSpriteData::SaveData( %s ): unable to open file.\n", szFilename );
		return -1;
	}

	// Print the number of images in the file. Not used yet, but maybe in
	// the future.
	fprintf( fout, "%d\n", 128 );
	for ( i=0; i<128; i++ )
	{
		// output block type
		fprintf( fout, "%d\n", m_type[i] );

		// output "extras" values
		for ( j=0; j<12; j++ )
		{
			if ( j<11 )
				fprintf( fout, "%d,", m_extras[i][j] );
			else
				fprintf( fout, "%d\n", m_extras[i][j] );
		} // j

		// output block color (for level editor)
		fprintf( fout, "%d\n", m_color[i] );
	} // i

	fclose( fout );

	return 0;
}

int CSpriteData::LoadSpriteImage()
{
	SYS_Debug ( "CSpriteData::LoadSpriteImage( %s ): Loading ...\n", m_szImgFilename );

	// If image already exists, delete it
	djDEL(m_pImage);

	int iRet = 0;

	// Attempt to load image file
	m_pImage = new djImage;
	if (m_pImage==NULL)
	{
	}
	else
	{
		char buf[1024];
#ifdef DATA_DIR
		sprintf( buf, "%s%s", DATA_DIR, m_szImgFilename );
#else
		sprintf( buf, "%s", m_szImgFilename );
#endif
		iRet = m_pImage->Load( buf );
	}

	return iRet;
}
/*--------------------------------------------------------------------------*/
