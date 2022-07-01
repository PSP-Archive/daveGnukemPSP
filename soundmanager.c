/*
 * $Id: soundmanager.c,v 1.4 2003/02/09 07:34:16 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * BGM/SE manager(using SDL_mixer).
 *
 * @version $Revision: 1.4 $
 */
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "SDL_mixer.h"
#include "soundmanager.h"
static int useAudio = 0;



static Mix_Music *music;


void closeSound() {
  if ( !useAudio ) return;
  if ( Mix_PlayingMusic() ) {
    Mix_HaltMusic();
  }
  Mix_CloseAudio();
}


// Initialize the sound.



// Play/Stop the music/chunk.

void playMusic(char *file) {
  char name[72];
	printf("Attempting to play music\n");
    if ( music==NULL ){
    strcpy(name, "data/sounds/");
    strcat(name, file);
    strcat(name, ".s3m");
    if ( NULL == (music = Mix_LoadMUS(name)) ) {

		//try mod
    	strcpy(name, "data/sounds/");
    	strcat(name, file);
    	strcat(name, ".mod");
    	if ( NULL == (music = Mix_LoadMUS(name)) ) {
    		strcpy(name, "data/sounds/");
    		strcat(name, file);
    		strcat(name, ".xm");
    		if ( NULL == (music = Mix_LoadMUS(name)) ) {
    			strcpy(name, "data/sounds/");
    			strcat(name, file);
    			strcat(name, ".ogg");
    			if ( NULL == (music = Mix_LoadMUS(name)) ) {
      				//useAudio = 0;
      				//return;
			}
		}
	}
    }
  }
  Mix_PlayMusic(music, 1000);
}




void stopMusic() {
  if ( Mix_PlayingMusic() ) {
    Mix_HaltMusic();
  }
}

