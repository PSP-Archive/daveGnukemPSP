/*!
\file    thing.h
\brief   "Thing"
\author  David Joffe

Copyright (C) 1999-2001 David Joffe

License: GNU GPL Version 2 (*not* "later versions")
*/
/*--------------------------------------------------------------------------*/
/* David Joffe 1999/03/25 */
/*--------------------------------------------------------------------------*/
#ifndef _THING_H_
#define _THING_H_

#include "game.h"
#include "hero.h"

//! Convert world X coordinate (level block coordinate) to view (world display buffer) coordinates.
#define CALC_XOFFSET(x,xsmall) ( 8 * ( (xsmall) - xo_small + 2 + ((( (x) - xo ) << 1))) )
//! Convert world Y coordinate (level block coordinate) to view (world display buffer) coordinates.
#define CALC_YOFFSET(y) ( 16 + ((y) - yo) * 16 )

//! Convert world X coordinate (pixels) to view (world display buffer) coordinates.
//! Theoretically this should probably eventually replace CALC_XOFFSET
#define WORLDX2VIEW(x) ( -(8*xo_small) + ((x) - (16*xo)) + 16 )
//! Convert world Y coordinate (pixels) to view (world display buffer) coordinates.
//! Theoretically this should probably eventually replace CALC_YOFFSET
#define WORLDY2VIEW(y) (                 ((y) - (16*yo)) + 16)


#include "djtypes.h"
#include "djsound.h"
#include <vector>
using namespace std;
/*-----------------------------------------------------------*/
//! Used to indicate the order in which "things" get drawn. Things at higher
//! layers get drawn over things at lower layers.
enum EdjLayer
{
	//! Layer 1; Absolute bottommost. Drawn first.
	LAYER_BOTTOM,
	//! Layer 2
	LAYER_2,
	//! Layer 3; Hero gets drawn at this level
	LAYER_MIDDLE,
	//! Layer 4
	LAYER_4,
	//! Layer 5; Absolute top-most, e.g. score floating letters
	LAYER_TOP
};
/*-----------------------------------------------------------*/

//! This crap should go. Fixme.
#define THING_DIE -1
//! Remove, but not delete
#define THING_REMOVE -2


class CThing;

//! The \ref CThingFactory uses functions that look like this to allocate CThing's.
typedef CThing* (*THING_ALLOCATER)();
typedef void (*THING_PERLEVELINIT)();


/*-----------------------------------------------------------*/
/*!
\class CThingFactory
\nosubgrouping

Class where \ref CThing types can register create handlers for themselves. These
handlers provide information required to allocate an instance of a thing, e.g.
when loading a level.
*/
class CThingFactory
{
public:
	CThingFactory();
	~CThingFactory();

	//! Register a CThing given its type ID and a pointer to an allocate procedure.
	int Register(int nTypeID, THING_ALLOCATER pAllocateProc, THING_PERLEVELINIT pPerLevelInitProc=NULL);

	//! Allocate a CThing based on its type ID. Returns NULL if no such type ID.
	CThing* Allocate(int nTypeID);

	//! Called per level. Allows thing-specific per-level initialization. This
	//! function calls the per-level-init proc for all registered things.
	void PerLevelInitialize();


protected:
	struct SDescriptor
	{
		SDescriptor () : nTypeID(0), pAllocateProc(0), pPerLevelInitProc(0) {}

		int 				nTypeID;			// Sprite type ID
		THING_ALLOCATER 	pAllocateProc;		// Pointer to procedure that allocates instance
		THING_PERLEVELINIT 	pPerLevelInitProc;	// Optional pointer to per-level initialization
	};

	vector<SDescriptor> m_aDescriptors;
};

//! Single global "thing" factory
extern CThingFactory g_ThingFactory;

//! Macro that "thing"s can use (in the .cpp file) to register handlers in the thing factory
#define REGISTER_THING(ClassName, nTypeID, pPerLevelInitProc)	\
	CThing *ALLOCATE##ClassName() \
	{ \
		return new ClassName; \
	} \
	static int UNIQUEID##ClassName = g_ThingFactory.Register(nTypeID, ALLOCATE##ClassName, pPerLevelInitProc);


/*-----------------------------------------------------------*/
/*!
\class CThing
\nosubgrouping

A CThing is the base class for any functional entity in the Dave Gnukem world,
such as a monster, or certain collectable types, shootable boxes, lifts, teleporters,
exits, explosions, and also the little floating scores that appear when our
hero scores.

Some collectables and other stuff are not derived from this, but are hardcoded
in the game. This is simply because its old C code that still needs to be converted
to use this stuff.

A CThing has bounds, which should indicate the "action area", which is the area in
the level in which the thing "has effect". Note that this need not be the same as the area which a thing
draws itself, for example the action area for the lift is just *above* the lift, as the action is
performed when the hero stands on top of the lift. If the hero is within the bounds of
the thing and the player presses the action key, the game calls the virtual function
Action().

Pressing 'D' during the game enables the showing of debug information. This will
display the action areas of things as yellow boxes. This appears as a single pixel
if the action area is not defined (that is, not applicable), e.g. for floating scores.
When the hero enters the bounding box, the box turns white.
*/
class CThing
{
public:
	///\name Constructors/destructors
	//@{

	//! Constructor
	CThing();
	//! Constructor
	CThing( int x, int y, int xsmall, bool bxsmall );
	//! Destructor
	virtual ~CThing() {}

	//@}
	///\name Overridables
	//@{

	//! Called per frame to update this thing. Returning THING_DIE indicates that this "thing"
	//! should be deleted from the world.
	virtual int  Tick() { return 0; }
	//! Called to draw this thing. Note that this does not get called if thing
	//! does not get drawn (i.e. if clipped out of visible viewport). Use Tick()
	//! for updating things that must be updated each frame.
	virtual void Draw() {}
	//! Called when the user presses the action key while within the "action area" (bounds) of the thing.
	//! This function should return to the game information on what action to perform.
	//! This function *may* be superceded by the one below it
	virtual int Action() { return 0; }
	//! Called when the hero enters the bounds of this thing. Derived implementations should call parent implementation.
	virtual void HeroEnter() { m_bHeroInside = true; }
	//! Called when the hero leaves the bounds of this thing. Derived implementations should call parent implementation.
	virtual void HeroLeave() { m_bHeroInside = false; }
	//! Called each frame that hero is touching the action box
	virtual int HeroOverlaps() { return 0; };

	//! Called just before added to world, to be used for custom initialization. May supercede OnAdded. (fixme)
	virtual void Initialize(int b0, int b1) {};
	//! Called when thing is first added to world. Can be used for initialisation.
	virtual void OnAdded() {};

	//! Called when thing is shot by hero. m_bShootable must be set.
	virtual int OnHeroShot() { return 0; }

	//! Called on an object in the inventory when inventory is cleared at end of level.
	//! Return false if object should remain in inventory (e.g. objects that persist
	//! throughout the game such as powerboots, as opposed to objects that disappear
	//! after each level, such as keys).
	virtual bool OnInventoryClear() { return true; }
	//! Called when we are loaded into the inventory during a load-game. This allows
	//! some special handling, e.g. powerboots can set the hero's jump mode.
	virtual void OnInventoryLoad() {}

	// Run once whenever a level starts on all types of registered things.
	// Some things require per-level information related to them, such as
	// the letters G,N,U,K,E and M, which must be collected in that order.
	// This allows initialization related to such information. Note that
	// internally, such things will most likely have to be implemented
	// using static members.
	//virtual int OnLevelStart() { return 0; }

	//! Draw action bounds. This is purely for debugging/development.
	virtual void DrawActionBounds(const djColor &Color);
protected:
	bool m_bHeroInside;
public:

	//@}

	//! Returns true if hero is currently inside the bounding area of this thing. This function does
	//! not physically check, but gets set/unset in HeroEnter() and HeroLeave().
	bool IsHeroInside() const { return m_bHeroInside; }
	//! Set position as well as xoffset, yoffset and width, height (????)
	void SetLocation( int ix, int iy, int xoffset, int yoffset, int width, int height );
	//! Set position of thing. This is in level block units, e.g. on a 128x100 block level
	void SetPosition(int x, int y) { m_x = x; m_y = y; }
	//! Set main spriteset and sprite index for graphics
	void SetSprite(int a, int b);
	//! Set action bounding area. This is in pixels, relative to object position in world (e.g. 0,0,15,15 is one level block)
	void SetActionBounds(int x1, int y1, int x2, int y2);
	//! Set area which is solid. This is in pixels, relative to object position in world (e.g. 0,0,15,15 is one level block)
	void SetSolidBounds(int x1, int y1, int x2, int y2);
	//! Set area which is shootable. This is in pixels, relative to object position in world (e.g. 0,0,15,15 is one level block)
	void SetShootBounds(int x1, int y1, int x2, int y2);
	//! Set area which is drawn on screen. This is in pixels, relative to object position in world.
	//! The default is (0,0,15,15). This is used to clip things to the view when redrawing the screen.
	void SetVisibleBounds(int x1, int y1, int x2, int y2);

	//! Test if thing is visible (i.e. if visiblebounds are overlapping the view)
	inline bool IsInView()
	{
		return (bool)(0!=OVERLAPS_VIEW(
			m_x*16 + m_iVisibleX1,
			m_y*16 + m_iVisibleY1,
			m_x*16 + m_iVisibleX2,
			m_y*16 + m_iVisibleY2));
	}

	///\name Attributes
	//@{

	//! Return the type ID of this thing
	int GetTypeID() const { return m_iType; }
	//! Return true if thing is shootable by hero (i.e. m_bShootable is set)
	bool IsShootable() const { return m_bShootable; }

	//@}

	// Attributes
	int  m_a;            // spriteset
	int  m_b;            // sprite index
	int  m_x;            // x position
	int  m_y;            // y position
	int  m_xsmall;       // x small offset (0 or 1)
	bool m_bxsmall;      // does this block use x small offset?

	//-- gravity
	bool m_bFalls;       // affected by gravity?
	bool Falls() { return m_bFalls; }

	//-- solidness
	bool m_bSolid;   // solid?
	int  m_iSolidX1; // if solid, this is a bounding box, in pixels, relative
	int  m_iSolidY1; // to (m_x*W,m_y*H), where W,H currently 16,16
	int  m_iSolidX2;
	int  m_iSolidY2;


	int  m_iShootX1;
	int  m_iShootY1;
	int  m_iShootX2;
	int  m_iShootY2;

	int m_iID;           // An ID number. Can do with what want. Eg door number, teleporeter nubmer
	void SetID(int i) { m_iID = i; }
	int ID() { return m_iID; }

	// Location:
	int  m_xoffset;      // x offset from (m_x,m_y) where thing starts
	int  m_yoffset;      // y offset from (m_x,m_y) where thing starts
	int  m_width;        // x size, in blocks, of thing
	int  m_height;       // y size, in blocks, of thing

	// "Action key" test box
	int m_iActionX1; // x offset (pixels) from (m_x*W,m_y*H)
	int m_iActionY1; // y offset (pixels) from (m_x*W,m_y*H)
	int m_iActionX2; // in pixels
	int m_iActionY2; // in pixels
	//! Test if hero is completely inside the box
	inline bool InBounds(int x, int y)
	{
		return (
			(x   >=m_x*16+m_iActionX1) &&
			(x+15<=m_x*16+m_iActionX2) &&
			(y   >=m_y*16+m_iActionY1) &&
			(y+31<=m_y*16+m_iActionY2));
	}
	//! Test if hero is at least partially inside the box. By default this
	//! just uses the action bounds; override this for things that have
	//! irregular shapes that cannot be described by a single rectangle.
	virtual bool OverlapsBounds(int x, int y);
	//! Test if given rectangle (in pixel coordinates) overlaps action rectangle (fixme, make virtual)
	inline bool OverlapsBounds(int x1, int y1, int x2, int y2)
	{
		return OVERLAPS(
			x1, y1,
			x2, y2,
			m_x*16+m_iActionX1, m_y*16+m_iActionY1,
			m_x*16+m_iActionX2, m_y*16+m_iActionY2);
	}
	//! Test if given rectangle (in pixel coordinates) overlaps shoot rectangle
	inline bool OverlapsShootArea(int x1, int y1, int x2, int y2)
	{
		return OVERLAPS(
			x1, y1,
			x2, y2,
			m_x*16+m_iShootX1, m_y*16+m_iShootY1,
			m_x*16+m_iShootX2, m_y*16+m_iShootY2);
	}

	EdjLayer m_eLayer;   // Z-depth for drawing in "layers"

	EdjLayer Layer()              { return m_eLayer; }
	int      Width()              { return m_width; }
	int      Height()             { return m_height; }
	int      OffsetX()            { return m_xoffset; }
	int      OffsetY()            { return m_yoffset; }
	void     SetType( int iType ) { m_iType = iType; }
	void     SetLayer(EdjLayer l) { m_eLayer = l; }

	// Visible on-screen size, pixels
	int m_iVisibleX1;
	int m_iVisibleY1;
	int m_iVisibleX2;
	int m_iVisibleY2;

protected:
	///\name Protected members
	//@{

	int      m_iType;    // Type of block, eg TYPE_LIFT
	bool     m_bShootable;

	//@}
};
/*-----------------------------------------------------------*/
/*!
\class CTest
\nosubgrouping

Debug class for testing out stuff
*/
class CTest : public CThing
{
public:
	CTest();

	virtual void HeroEnter();
	virtual void Draw();
	virtual int  Tick();
};
/*-----------------------------------------------------------*/
/*!
\class CSpikeBall
\nosubgrouping

Bouncing spiky ball that hurts you.

There is also a stationary type, which breaks up when you touch it.
*/
class CSpikeBall : public CThing
{
public:
	// Spike ball type
	enum EType
	{
		TYPE_BOUNCING,
		TYPE_STATIONARY
	};
	CSpikeBall();
	virtual void Draw();
	virtual int Tick();
	virtual int HeroOverlaps();
	virtual void OnAdded();
	virtual void Initialize(int b0, int b1);
protected:
	int m_nYOffset;
	int m_nBounceIndex;
	int m_nInitialOffset;
	EType m_eType;
};
/*-----------------------------------------------------------*/
/*!
\class CBox
\nosubgrouping

Shoot a box to reveal its contents
*/
class CBox : public CThing
{
public:
	CBox();

	void SetContents( int iContentsA, int iContentsB );

	virtual void Draw();
	virtual void Initialize(int b0, int b1);
	virtual int OnHeroShot();

	int  m_iContentsA;   // sprite object inside box (a==spriteset)
	int  m_iContentsB;   // sprite object inside box (b==sprite)
};
/*-----------------------------------------------------------*/
/*!
\class CLetter
\nosubgrouping

Letters G,N,U,K,E,M, collect them in this order for a bonus
*/
class CLetter : public CThing
{
public:
	CLetter();

	virtual void Draw();
	virtual void Initialize(int b0, int b1);
	virtual int HeroOverlaps();
protected:
	int m_iNumber;       // which letter is this? (ie from 0 to 5)
	friend void CLetterPerLevelInit();
	static int c_nBonusIndex;
};
/*-----------------------------------------------------------*/
/*!
\class CPickup
\nosubgrouping

Generic pickup (food, points etc)
*/
class CPickup : public CThing
{
public:
	CPickup();
	virtual int  HeroOverlaps();
	virtual void Draw();
	virtual int  Tick();
	virtual void Initialize(int b0, int b1);
	virtual bool OnInventoryClear() { return !m_bPersistent; }
protected:
	int m_nScoreDiff;
	int m_nHealthDiff;
	int m_nAnimationCount; // -1 if non-animated, otherwise [0, 3]
	bool m_bInventoryItem;
	bool m_bPersistent;
};
/*-----------------------------------------------------------*/
/*!
\class CLift
\nosubgrouping

Pressing "up" on a lift raises the lift, thereby raising hero, as long as
hero remains standing on lift.
*/
class CLift : public CThing
{
public:
	CLift();

	virtual int  Tick();
	virtual void Draw();
	virtual int  Action();
	virtual void HeroLeave();
protected:
	bool m_bBusy;
};
/*-----------------------------------------------------------*/
/*!
\class CExplosion
\nosubgrouping

This is an explosion type dust cloud type effect; a dynamically generated
animation effect when you shoot (for example) a monster.
*/
class CExplosion : public CThing
{
public:
	CExplosion();

	virtual int  Tick();
	virtual void Draw();

	int m_countdown;
	int m_xOffset;
	int m_yOffset;
};
/*-----------------------------------------------------------*/
/*!
\class CExit
\nosubgrouping

The exit! This is what the hero seeks in each level to reach the end.
*/
class CExit : public CThing
{
public:
	CExit();
	virtual void Draw();
	virtual int  Action();
	virtual int  Tick();
protected:
	int m_nActivated; // -1. Becomes 0 when activated, then increments from 1 - 5, then exit occurs
	int m_nSecondFrameTick; // To slow down animation to ever second frame ..
};
/*-----------------------------------------------------------*/
/*!
\class CTeleporter
\nosubgrouping

Pressing "up" on a teleporter takes hero to corresponding teleporter. Thus
teleporters are always placed in pairs. You can have up to four pairs of
teleporters in each level, these are chosen by selecting the corresponding
sprites in the level editor.
*/
class CTeleporter : public CThing
{
public:
	CTeleporter();
	virtual void Initialize(int b0, int b1);
	virtual int  Action();
	virtual void Draw();
	virtual int  Tick();
protected:
	int m_iAnimationCount;
	bool m_bActivated;
	int m_nTeleportX;
	int m_nTeleportY;
	static SOUND_HANDLE c_iSoundTeleport;
};
/*-----------------------------------------------------------*/
/*!
\class CFloatingScore
\nosubgrouping

Those floating score thingies that appear when hero scores points. These are
not placed in the level editor, they are dynamically generated by the
game when events occur that must result in these.
*/
class CFloatingScore : public CThing
{
public:
	CFloatingScore();

	virtual int  Tick();
	virtual void Draw();

	void SetScore( int score );

	int m_height;
	unsigned char m_buffer[16];
	int m_bufferlength;
};
/*-----------------------------------------------------------*/
/*!
\class CDoorRelatedType
\nosubgrouping

All "thing" types pertaining to doors (keys, doors and door activators) are
all linked by an ID number. This functionality is provided by this parent class.
*/
class CDoorRelatedType : public CThing
{
public:
	CDoorRelatedType();

	virtual void Initialize(int b0, int b1);
	int GetID() const { return m_nID; }
protected:
	int m_nID;
};
/*-----------------------------------------------------------*/
/*!
\class CDoor
\nosubgrouping

A door. Needs a key of same colour (index), as specified in extras[0].
*/
class CDoor : public CDoorRelatedType
{
public:
	CDoor();
	virtual int  Tick();
	virtual void Draw();
	void OpenDoor() { m_nOpenState = 1; }
protected:
	int m_nOpenState; // 0 when closed, 1,2,3 when opening (corresponds to animation), 4 deletes itself
};
/*-----------------------------------------------------------*/
/*!
\class CKey
\nosubgrouping

A key. Belongs to a door of same colour (index), as specified in extras[0].
*/
class CKey : public CDoorRelatedType
{
public:
	CKey();
	virtual int HeroOverlaps();
	virtual void Draw();
};
/*-----------------------------------------------------------*/
/*!
\class CDoorActivator
\nosubgrouping

This is what you put a key in to open a door. Belongs to a door and key of same
colour (index), as specified in extras[0].
*/
class CDoorActivator : public CDoorRelatedType
{
public:
	CDoorActivator();
	virtual int  Action();
	virtual void Draw();
};
/*-----------------------------------------------------------*/
/*!
\class CSoftBlock
\nosubgrouping

A solid block which disappears when it gets shot
*/
class CSoftBlock : public CThing
{
public:
	CSoftBlock();
	virtual void Draw();
	virtual int OnHeroShot();
};
/*-----------------------------------------------------------*/
/*!
\class CCamera
\nosubgrouping

Security camera. Turns to look at you. Can be shot.
If you shoot all of them in a level, you get a bonus (fixme, not implemented)
*/
class CCamera : public CThing
{
public:
	CCamera();
	virtual void Draw();
	virtual int  OnHeroShot();
protected:
	friend void CCameraPerLevelInit();
	static int c_nNumCameras; // Number of cameras in the level
};
/*-----------------------------------------------------------*/
/*!
\class CBanana
\nosubgrouping

Banana. If you shoot the banana, it becomes a bunch of bananas, and is
worth more. This is the equivalent of the turkey in the real Duke.
*/
class CBanana : public CThing
{
public:
	CBanana();
	virtual void Draw();
	virtual int  OnHeroShot();
	virtual int HeroOverlaps();
protected:
	int m_nState; // 0 = single banana, 1 = bunch
};
/*-----------------------------------------------------------*/
/*!
\class CCoke
\nosubgrouping

Coke can. If you shoot the coke can, it flies up into the air.
*/
class CCoke : public CPickup
{
public:
	CCoke();
	virtual void Draw();
	virtual int OnHeroShot();
	virtual int HeroOverlaps();
	virtual int Tick();
protected:
	bool IsShot() const { return m_nShotHeight!=-1; }
	int m_nShotHeight; // -1 until shot, otherwise positive=up height (pixels)
	int m_nAnim;
};
/*-----------------------------------------------------------*/
/*!
\class CCrawler
\nosubgrouping

Green crawler monster, clings to wall going up and down.
*/
class CCrawler : public CThing
{
public:
	CCrawler();
	virtual int Tick();
	virtual void Draw();
	virtual int  OnHeroShot();
	virtual int HeroOverlaps();
	virtual void Initialize(int b0, int b1);
protected:
	int m_nOffset;
	int m_nDir; // direction
	int m_nXDir; // -1=face left, 1=face right
};
/*-----------------------------------------------------------*/
/*!
\class CSpike
\nosubgrouping

Spike in ground, hurts hero.

Two types; static (extra[0]=0), and pop-up (extra[0]=1)
*/
class CSpike : public CThing
{
public:
	CSpike();
	virtual int  Tick();
	virtual void Draw();
	virtual int  HeroOverlaps();
	virtual void Initialize(int b0, int b1);
protected:
	int m_nType;
	int m_nSpikePopupCount;
};
/*-----------------------------------------------------------*/
/*!
\class CBalloon
\nosubgrouping

Floating balloon
*/
class CBalloon : public CThing
{
public:
	CBalloon();
	virtual int  HeroOverlaps();
	virtual void Draw();
	virtual int  Tick();
	virtual int  OnHeroShot();
protected:
	int m_nHeight; // Starts at 0, increases
};
/*-----------------------------------------------------------*/
/*!
\class CAcme
\nosubgrouping

Acme falling block
*/
class CAcme : public CThing
{
public:
	CAcme();
	virtual int  HeroOverlaps();
	virtual void Draw();
	virtual int  Tick();
	virtual int  OnHeroShot();
protected:
	int m_nHeight; // -1 when not moving. Starts at 0, increases as falls downwards
};
/*-----------------------------------------------------------*/
/*!
\class CBoots
\nosubgrouping

Power boots
*/
class CBoots : public CPickup
{
public:
	CBoots();
	virtual int HeroOverlaps();
	virtual void OnInventoryLoad();
};
/*-----------------------------------------------------------*/
/*!
\class CRobot
\nosubgrouping

Monster - dumb robot
*/
class CRobot : public CThing
{
public:
	CRobot();
	virtual int HeroOverlaps();
	virtual void Draw();
	virtual int Tick();
	virtual int OnHeroShot();
	virtual void Initialize(int b0, int b1);
protected:
	int m_nXOffset;
	int m_nDX; // Either -1 or 1.
	int m_nType; // Currently 0=normal robot, 1=fireball thingy
	int m_nHeightOffset; // 0 for robot, -15 for fireball
	int m_nNoShootCounter;
};
/*-----------------------------------------------------------*/
/*!
\class CConveyor
\nosubgrouping

Conveyor belt
*/
class CConveyor : public CThing
{
public:
	CConveyor();
	virtual int Tick();
	virtual void Draw();
	virtual void Initialize(int b0, int b1);
protected:
	int m_nDir; // Direction, either -1 or 1
};
/*-----------------------------------------------------------*/
/*!
\class CFlameThrow
\nosubgrouping

Flame throw thingy
*/
class CFlameThrow : public CThing
{
public:
	CFlameThrow();
	virtual int Tick();
	virtual void Draw();
	virtual void Initialize(int b0, int b1);
	virtual int HeroOverlaps();
protected:
	int m_nDir; // Direction, either -1 or 1
	int m_nCount; // This determines what phase of the flameblast we're in
};
/*-----------------------------------------------------------*/
/*!
\class CDynamite
\nosubgrouping

Dynamite (Flames, from in a box)
*/
class CDynamite : public CThing
{
public:
	CDynamite();
	virtual int Tick();
	virtual void Draw();
	virtual int HeroOverlaps();
	virtual bool OverlapsBounds(int x, int y);
	virtual void DrawActionBounds(const djColor &Color);
protected:
	int m_nCount; // This determines what phase of the flameblast we're in
	int m_nX1Right;
	int m_nX2Right;
	int m_nX1Left;
	int m_nX2Left;
	int m_nCenter;
};
/*-----------------------------------------------------------*/
/*!
\class CFan
\nosubgrouping

Fan. Blows. Stops when shot.
*/
class CFan : public CThing
{
public:
	CFan();
	virtual int Tick();
	virtual void Draw();
	virtual void Initialize(int b0, int b1);
	virtual int OnHeroShot();
protected:
	int m_nAnim;
	int m_nSpeed;
	int m_nAnimTimer;
	int m_nDir;
	bool m_bStopped;
};
/*-----------------------------------------------------------*/
/*!
\class CFirepower
\nosubgrouping

Firepower. When picked up, increments firepower.
*/
class CFirepower : public CThing
{
public:
	CFirepower();
	virtual void Draw();
	virtual int HeroOverlaps();
};
/*-----------------------------------------------------------*/
/*!
\class CDust
\nosubgrouping

Dust. Gets kicked up when e.g. hero falls onto ground.
*/
class CDust : public CThing
{
public:
	CDust();
	virtual int Tick();
	virtual void Draw();
protected:
	int m_anAnim[4];//!< Animation phase for up to four puffs
	int m_anX[4];	//!< X positions of puffs
	int m_anY[4];	//!< Y positions of puffs
	int m_nPuff;	//!< Current puff [0,3]
};
/*-----------------------------------------------------------*/
// Helper functions to create various things

//! Create a \ref CFloatingScore at (x,y) with score \b score
extern CThing *CreateFloatingScore( int x, int y, int score );

//! Create a \ref CExplosion at (nX,nY) (pixel coordinates)
extern CThing *CreateExplosion(int nX, int nY);

//! Create a \ref CDust effect at (nX, nY) (level coordinates)
extern CThing *CreateDust(int nX, int nY);

#endif
