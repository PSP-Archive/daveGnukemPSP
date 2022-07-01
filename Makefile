TARGET = daveGnukem
PSPSDK=$(shell psp-config --pspsdk-path)
PSPBIN = $(PSPSDK)/../bin
SDL_CONFIG = $(PSPBIN)/sdl-config

PSP_EBOOT_ICON = ICON0.PNG
#PSP_EBOOT_PIC1 = PIC1.PNG


# Object files
OBJS =	bullet.o	\
	ed.o		\
	ed_common.o	\
	ed_lvled.o	\
	ed_spred.o	\
	ed_macros.o	\
	ed_DrawBoxContents.o	\
	sys_log.o	\
	sys_error.o	\
	m_misc.o	\
	graph.o		\
	block.o		\
	djstring.o	\
	djtypes.o	\
	djimage.o	\
	djlog.o		\
	mission.o	\
	mixins.o	\
	level.o		\
	sdl/djgraph.o	\
	sdl/djinput.o	\
	sdl/djsound.o	\
	sdl/djtime.o	\
	main.o		\
	game.o		\
	menu.o		\
	credits.o	\
	instructions.o	\
	inventory.o	\
	hiscores.o	\
	thing.o		\
	hero.o		\
	settings.o	\
	keys.o		\
	soundmanager.o


INCDIR = 
CFLAGS = -O2 -G0 -g -Wall $(shell $(SDL_CONFIG) --cflags) -DUSESDL -DSYS_WIN32 -DDAVE_GAME 
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR = 
LDFLAGS =
LIBS = -lstdc++ -lSDL_mixer -lvorbisidec $(shell $(SDL_CONFIG) --libs)  -lpsppower

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Dave Gnukem

include $(PSPSDK)/lib/build.mak
