/*
keys.cpp

Copyright (C) 2001 David Joffe

License: GNU GPL Version 2 (*not* "later versions")

Created: 09/2001
*/

#include "keys.h"
#include "settings.h"
#include "SDL.h"
#include <vector>
using namespace std;

// Default keys
int g_anKeys[KEY_NUMKEYS] =
{
	SDLK_UP,
	SDLK_LEFT,
	SDLK_RIGHT,
	SDLK_RCTRL,
	SDLK_RALT
};

// Key descriptions
char *g_aszKeys[KEY_NUMKEYS] =
{
	"Action",
	"Left",
	"Right",
	"Jump",
	"Shoot"
};

vector<int> g_anValidGameKeys;

void StoreGameKeys()
{
	int i;
	for ( i=0; i<KEY_NUMKEYS; i++ )
	{
		char szKey[64];
		sprintf(szKey, "Key%s", g_aszKeys[i]);
		g_Settings.SetSettingInt(szKey, g_anKeys[i]);
	}
}

void InitialiseGameKeySystem()
{
	// Set default keys (if not defined in settings already, e.g. if there was no config file)
	int i;
	char szKey[64];
	for ( i=0; i<KEY_NUMKEYS; i++ )
	{
		sprintf(szKey, "Key%s", g_aszKeys[i]);
		g_Settings.SetDefaultSettingInt(szKey, g_anKeys[i]);
	}

	// Read key settings from config
	for ( i=0; i<KEY_NUMKEYS; i++ )
	{
		sprintf(szKey, "Key%s", g_aszKeys[i]);
		g_anKeys[i] = g_Settings.FindSettingInt(szKey);
	}

	// FIXME: TODO: Trim this down, remove non-allowed keys
	g_anValidGameKeys.push_back(SDLK_BACKSPACE);
	g_anValidGameKeys.push_back(SDLK_TAB);
	g_anValidGameKeys.push_back(SDLK_CLEAR);
	g_anValidGameKeys.push_back(SDLK_RETURN);
	g_anValidGameKeys.push_back(SDLK_PAUSE);
	//g_anValidGameKeys.push_back(SDLK_ESCAPE);
	g_anValidGameKeys.push_back(SDLK_SPACE);
	g_anValidGameKeys.push_back(SDLK_EXCLAIM);
	g_anValidGameKeys.push_back(SDLK_QUOTEDBL);
	g_anValidGameKeys.push_back(SDLK_HASH);
	g_anValidGameKeys.push_back(SDLK_DOLLAR);
	g_anValidGameKeys.push_back(SDLK_AMPERSAND);
	g_anValidGameKeys.push_back(SDLK_QUOTE);
	g_anValidGameKeys.push_back(SDLK_LEFTPAREN);
	g_anValidGameKeys.push_back(SDLK_RIGHTPAREN);
	g_anValidGameKeys.push_back(SDLK_ASTERISK);
	g_anValidGameKeys.push_back(SDLK_PLUS);
	g_anValidGameKeys.push_back(SDLK_COMMA);
	g_anValidGameKeys.push_back(SDLK_MINUS);
	g_anValidGameKeys.push_back(SDLK_PERIOD);
	g_anValidGameKeys.push_back(SDLK_SLASH);
	g_anValidGameKeys.push_back(SDLK_0);
	g_anValidGameKeys.push_back(SDLK_1);
	g_anValidGameKeys.push_back(SDLK_2);
	g_anValidGameKeys.push_back(SDLK_3);
	g_anValidGameKeys.push_back(SDLK_4);
	g_anValidGameKeys.push_back(SDLK_5);
	g_anValidGameKeys.push_back(SDLK_6);
	g_anValidGameKeys.push_back(SDLK_7);
	g_anValidGameKeys.push_back(SDLK_8);
	g_anValidGameKeys.push_back(SDLK_9);
	g_anValidGameKeys.push_back(SDLK_COLON);
	g_anValidGameKeys.push_back(SDLK_SEMICOLON);
	g_anValidGameKeys.push_back(SDLK_LESS);
	g_anValidGameKeys.push_back(SDLK_EQUALS);
	g_anValidGameKeys.push_back(SDLK_GREATER);
	g_anValidGameKeys.push_back(SDLK_QUESTION);
	g_anValidGameKeys.push_back(SDLK_AT);
	g_anValidGameKeys.push_back(SDLK_LEFTBRACKET);
	g_anValidGameKeys.push_back(SDLK_BACKSLASH);
	g_anValidGameKeys.push_back(SDLK_RIGHTBRACKET);
	g_anValidGameKeys.push_back(SDLK_CARET);
	g_anValidGameKeys.push_back(SDLK_UNDERSCORE);
	g_anValidGameKeys.push_back(SDLK_BACKQUOTE);
	g_anValidGameKeys.push_back(SDLK_a);
	g_anValidGameKeys.push_back(SDLK_b);
	g_anValidGameKeys.push_back(SDLK_c);
	g_anValidGameKeys.push_back(SDLK_d);
	g_anValidGameKeys.push_back(SDLK_e);
	g_anValidGameKeys.push_back(SDLK_f);
	g_anValidGameKeys.push_back(SDLK_g);
	g_anValidGameKeys.push_back(SDLK_h);
	g_anValidGameKeys.push_back(SDLK_i);
	g_anValidGameKeys.push_back(SDLK_j);
	g_anValidGameKeys.push_back(SDLK_k);
	g_anValidGameKeys.push_back(SDLK_l);
	g_anValidGameKeys.push_back(SDLK_m);
	g_anValidGameKeys.push_back(SDLK_n);
	g_anValidGameKeys.push_back(SDLK_o);
	g_anValidGameKeys.push_back(SDLK_p);
	g_anValidGameKeys.push_back(SDLK_q);
	g_anValidGameKeys.push_back(SDLK_r);
	g_anValidGameKeys.push_back(SDLK_s);
	g_anValidGameKeys.push_back(SDLK_t);
	g_anValidGameKeys.push_back(SDLK_u);
	g_anValidGameKeys.push_back(SDLK_v);
	g_anValidGameKeys.push_back(SDLK_w);
	g_anValidGameKeys.push_back(SDLK_x);
	g_anValidGameKeys.push_back(SDLK_y);
	g_anValidGameKeys.push_back(SDLK_z);
	g_anValidGameKeys.push_back(SDLK_DELETE);
	g_anValidGameKeys.push_back(SDLK_WORLD_0);
	g_anValidGameKeys.push_back(SDLK_WORLD_1);
	g_anValidGameKeys.push_back(SDLK_WORLD_2);
	g_anValidGameKeys.push_back(SDLK_WORLD_3);
	g_anValidGameKeys.push_back(SDLK_WORLD_4);
	g_anValidGameKeys.push_back(SDLK_WORLD_5);
	g_anValidGameKeys.push_back(SDLK_WORLD_6);
	g_anValidGameKeys.push_back(SDLK_WORLD_7);
	g_anValidGameKeys.push_back(SDLK_WORLD_8);
	g_anValidGameKeys.push_back(SDLK_WORLD_9);
	g_anValidGameKeys.push_back(SDLK_WORLD_10);
	g_anValidGameKeys.push_back(SDLK_WORLD_11);
	g_anValidGameKeys.push_back(SDLK_WORLD_12);
	g_anValidGameKeys.push_back(SDLK_WORLD_13);
	g_anValidGameKeys.push_back(SDLK_WORLD_14);
	g_anValidGameKeys.push_back(SDLK_WORLD_15);
	g_anValidGameKeys.push_back(SDLK_WORLD_16);
	g_anValidGameKeys.push_back(SDLK_WORLD_17);
	g_anValidGameKeys.push_back(SDLK_WORLD_18);
	g_anValidGameKeys.push_back(SDLK_WORLD_19);
	g_anValidGameKeys.push_back(SDLK_WORLD_20);
	g_anValidGameKeys.push_back(SDLK_WORLD_21);
	g_anValidGameKeys.push_back(SDLK_WORLD_22);
	g_anValidGameKeys.push_back(SDLK_WORLD_23);
	g_anValidGameKeys.push_back(SDLK_WORLD_24);
	g_anValidGameKeys.push_back(SDLK_WORLD_25);
	g_anValidGameKeys.push_back(SDLK_WORLD_26);
	g_anValidGameKeys.push_back(SDLK_WORLD_27);
	g_anValidGameKeys.push_back(SDLK_WORLD_28);
	g_anValidGameKeys.push_back(SDLK_WORLD_29);
	g_anValidGameKeys.push_back(SDLK_WORLD_30);
	g_anValidGameKeys.push_back(SDLK_WORLD_31);
	g_anValidGameKeys.push_back(SDLK_WORLD_32);
	g_anValidGameKeys.push_back(SDLK_WORLD_33);
	g_anValidGameKeys.push_back(SDLK_WORLD_34);
	g_anValidGameKeys.push_back(SDLK_WORLD_35);
	g_anValidGameKeys.push_back(SDLK_WORLD_36);
	g_anValidGameKeys.push_back(SDLK_WORLD_37);
	g_anValidGameKeys.push_back(SDLK_WORLD_38);
	g_anValidGameKeys.push_back(SDLK_WORLD_39);
	g_anValidGameKeys.push_back(SDLK_WORLD_40);
	g_anValidGameKeys.push_back(SDLK_WORLD_41);
	g_anValidGameKeys.push_back(SDLK_WORLD_42);
	g_anValidGameKeys.push_back(SDLK_WORLD_43);
	g_anValidGameKeys.push_back(SDLK_WORLD_44);
	g_anValidGameKeys.push_back(SDLK_WORLD_45);
	g_anValidGameKeys.push_back(SDLK_WORLD_46);
	g_anValidGameKeys.push_back(SDLK_WORLD_47);
	g_anValidGameKeys.push_back(SDLK_WORLD_48);
	g_anValidGameKeys.push_back(SDLK_WORLD_49);
	g_anValidGameKeys.push_back(SDLK_WORLD_50);
	g_anValidGameKeys.push_back(SDLK_WORLD_51);
	g_anValidGameKeys.push_back(SDLK_WORLD_52);
	g_anValidGameKeys.push_back(SDLK_WORLD_53);
	g_anValidGameKeys.push_back(SDLK_WORLD_54);
	g_anValidGameKeys.push_back(SDLK_WORLD_55);
	g_anValidGameKeys.push_back(SDLK_WORLD_56);
	g_anValidGameKeys.push_back(SDLK_WORLD_57);
	g_anValidGameKeys.push_back(SDLK_WORLD_58);
	g_anValidGameKeys.push_back(SDLK_WORLD_59);
	g_anValidGameKeys.push_back(SDLK_WORLD_60);
	g_anValidGameKeys.push_back(SDLK_WORLD_61);
	g_anValidGameKeys.push_back(SDLK_WORLD_62);
	g_anValidGameKeys.push_back(SDLK_WORLD_63);
	g_anValidGameKeys.push_back(SDLK_WORLD_64);
	g_anValidGameKeys.push_back(SDLK_WORLD_65);
	g_anValidGameKeys.push_back(SDLK_WORLD_66);
	g_anValidGameKeys.push_back(SDLK_WORLD_67);
	g_anValidGameKeys.push_back(SDLK_WORLD_68);
	g_anValidGameKeys.push_back(SDLK_WORLD_69);
	g_anValidGameKeys.push_back(SDLK_WORLD_70);
	g_anValidGameKeys.push_back(SDLK_WORLD_71);
	g_anValidGameKeys.push_back(SDLK_WORLD_72);
	g_anValidGameKeys.push_back(SDLK_WORLD_73);
	g_anValidGameKeys.push_back(SDLK_WORLD_74);
	g_anValidGameKeys.push_back(SDLK_WORLD_75);
	g_anValidGameKeys.push_back(SDLK_WORLD_76);
	g_anValidGameKeys.push_back(SDLK_WORLD_77);
	g_anValidGameKeys.push_back(SDLK_WORLD_78);
	g_anValidGameKeys.push_back(SDLK_WORLD_79);
	g_anValidGameKeys.push_back(SDLK_WORLD_80);
	g_anValidGameKeys.push_back(SDLK_WORLD_81);
	g_anValidGameKeys.push_back(SDLK_WORLD_82);
	g_anValidGameKeys.push_back(SDLK_WORLD_83);
	g_anValidGameKeys.push_back(SDLK_WORLD_84);
	g_anValidGameKeys.push_back(SDLK_WORLD_85);
	g_anValidGameKeys.push_back(SDLK_WORLD_86);
	g_anValidGameKeys.push_back(SDLK_WORLD_87);
	g_anValidGameKeys.push_back(SDLK_WORLD_88);
	g_anValidGameKeys.push_back(SDLK_WORLD_89);
	g_anValidGameKeys.push_back(SDLK_WORLD_90);
	g_anValidGameKeys.push_back(SDLK_WORLD_91);
	g_anValidGameKeys.push_back(SDLK_WORLD_92);
	g_anValidGameKeys.push_back(SDLK_WORLD_93);
	g_anValidGameKeys.push_back(SDLK_WORLD_94);
	g_anValidGameKeys.push_back(SDLK_WORLD_95);
	g_anValidGameKeys.push_back(SDLK_KP0);
	g_anValidGameKeys.push_back(SDLK_KP1);
	g_anValidGameKeys.push_back(SDLK_KP2);
	g_anValidGameKeys.push_back(SDLK_KP3);
	g_anValidGameKeys.push_back(SDLK_KP4);
	g_anValidGameKeys.push_back(SDLK_KP5);
	g_anValidGameKeys.push_back(SDLK_KP6);
	g_anValidGameKeys.push_back(SDLK_KP7);
	g_anValidGameKeys.push_back(SDLK_KP8);
	g_anValidGameKeys.push_back(SDLK_KP9);
	g_anValidGameKeys.push_back(SDLK_KP_PERIOD);
	g_anValidGameKeys.push_back(SDLK_KP_DIVIDE);
	g_anValidGameKeys.push_back(SDLK_KP_MULTIPLY);
	g_anValidGameKeys.push_back(SDLK_KP_MINUS);
	g_anValidGameKeys.push_back(SDLK_KP_PLUS);
	g_anValidGameKeys.push_back(SDLK_KP_ENTER);
	g_anValidGameKeys.push_back(SDLK_KP_EQUALS);
	g_anValidGameKeys.push_back(SDLK_UP);
	g_anValidGameKeys.push_back(SDLK_DOWN);
	g_anValidGameKeys.push_back(SDLK_RIGHT);
	g_anValidGameKeys.push_back(SDLK_LEFT);
	g_anValidGameKeys.push_back(SDLK_INSERT);
	g_anValidGameKeys.push_back(SDLK_HOME);
	g_anValidGameKeys.push_back(SDLK_END);
	g_anValidGameKeys.push_back(SDLK_PAGEUP);
	g_anValidGameKeys.push_back(SDLK_PAGEDOWN);
	g_anValidGameKeys.push_back(SDLK_F1);
	g_anValidGameKeys.push_back(SDLK_F2);
	g_anValidGameKeys.push_back(SDLK_F3);
	g_anValidGameKeys.push_back(SDLK_F4);
	g_anValidGameKeys.push_back(SDLK_F5);
	g_anValidGameKeys.push_back(SDLK_F6);
	g_anValidGameKeys.push_back(SDLK_F7);
	g_anValidGameKeys.push_back(SDLK_F8);
	g_anValidGameKeys.push_back(SDLK_F9);
	g_anValidGameKeys.push_back(SDLK_F10);
	g_anValidGameKeys.push_back(SDLK_F11);
	g_anValidGameKeys.push_back(SDLK_F12);
	g_anValidGameKeys.push_back(SDLK_F13);
	g_anValidGameKeys.push_back(SDLK_F14);
	g_anValidGameKeys.push_back(SDLK_F15);
	//g_anValidGameKeys.push_back(SDLK_NUMLOCK);
	//g_anValidGameKeys.push_back(SDLK_CAPSLOCK);
	//g_anValidGameKeys.push_back(SDLK_SCROLLOCK);
	g_anValidGameKeys.push_back(SDLK_RSHIFT);
	g_anValidGameKeys.push_back(SDLK_LSHIFT);
	g_anValidGameKeys.push_back(SDLK_RCTRL);
	g_anValidGameKeys.push_back(SDLK_LCTRL);
	g_anValidGameKeys.push_back(SDLK_RALT);
	g_anValidGameKeys.push_back(SDLK_LALT);
	g_anValidGameKeys.push_back(SDLK_RMETA);
	g_anValidGameKeys.push_back(SDLK_LMETA);
	g_anValidGameKeys.push_back(SDLK_LSUPER);
	g_anValidGameKeys.push_back(SDLK_RSUPER);
	g_anValidGameKeys.push_back(SDLK_MODE);
	g_anValidGameKeys.push_back(SDLK_COMPOSE);
	g_anValidGameKeys.push_back(SDLK_HELP);
	g_anValidGameKeys.push_back(SDLK_PRINT);
	//g_anValidGameKeys.push_back(SDLK_SYSREQ);
	//g_anValidGameKeys.push_back(SDLK_BREAK);
	g_anValidGameKeys.push_back(SDLK_MENU);
	//g_anValidGameKeys.push_back(SDLK_POWER);
	g_anValidGameKeys.push_back(SDLK_EURO);
}

bool IsGameKey(int nKeyCode)
{
	unsigned int i;
	for ( i=0; i<g_anValidGameKeys.size(); i++ )
	{
		if (nKeyCode==g_anValidGameKeys[i])
			return true;
	}
	return false;
}

