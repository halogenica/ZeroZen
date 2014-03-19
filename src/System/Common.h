
#pragma once

#include <cassert>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "btBulletDynamicsCommon.h"

#include "cinder/app/AppNative.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/ObjLoader.h"
#include "cinder/Camera.h"
#include "cinder/Cinder.h"
#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Text.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Io.h"
#include "cinder/Arcball.h"

#if ! defined(CINDER_GLES)
#include "cinder/gl/GlslProg.h"
#endif

#include "Resources.h"

#ifdef __APPLE__

#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
// iOS device
#elif TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#endif

#else
// Windows
#endif

/**********************************************************\
*    Macros
\**********************************************************/
// Asserts
#if _DEBUG
//    #define ASSERT(x) { if (!(x)) { abort(); } }
#define ASSERT(x) {if (!(x)) {__debugbreak();}}
#else
    #define ASSERT(x)
#endif

#define CASSERT(predicate) _impl_CASSERT_LINE(predicate,__LINE__,__FILE__)
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];


/**********************************************************\
*    Defines
\**********************************************************/
//#define NULL    ((void *)0)

// define the screen resolution
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720
#define FOV             D3DXToRadian(45)
#define ASPECT          ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT)
#define NEAR_CLIP       1.0f
#define FAR_CLIP        5000.0f

#define GRAVITY_CONST   -20.0f
#define MAX_VELOCITY    500.0f

#define PLAY_SOUND      1


/**********************************************************\
*    Prototypes
\**********************************************************/
namespace gen
{
    class App;
    class Input;
    class Stats;
    class ObjectManager;
}

namespace mowa
{
    namespace sgui
    {
        class SimpleGUI;
    }
}


/**********************************************************\
*    Globals
\**********************************************************/
extern gen::App*                g_pApp;               // application
extern gen::Input*              g_pInputP1;           // P1 input
extern gen::Input*              g_pInputP2;           // P2 input
extern gen::Input*              g_pInputP3;           // P3 input
extern gen::Input*              g_pInputP4;           // P4 input
extern gen::Stats*              g_pStats;             // statistics
extern gen::ObjectManager*      g_pObjectManager;     // the object manager
extern btDiscreteDynamicsWorld* g_pBtDynamicsWorld;   // the physics world
extern mowa::sgui::SimpleGUI*   g_pGui;               // the GUI


/**********************************************************\
*    Enums
\**********************************************************/
namespace gen
{
enum CollisionTypes 
{
    COL_NOTHING     = 0x0,
    COL_GROUND      = 0x1 << 0,
    COL_PARTICLE    = 0x1 << 1,
    COL_ENEMY       = 0x1 << 2,
    COL_PLAYER      = 0x1 << 3,
    COL_BULLET      = 0x1 << 4,

    COL_EVERYTHING  = (0x1 << 5) - 1
};

enum GameState
{
    TITLE               = 0x0,
    READY               = 0x1,
    PLAYING             = 0x2,
    P1_WINS             = 0x3,
    P2_WINS             = 0x4,
    P3_WINS             = 0X5,
    P4_WINS             = 0X6,
    CREDITS             = 0x7,

    NUM_GAME_STATES     = 0x8,
};
}


/**********************************************************\
*    Typedefs
\**********************************************************/
/*
namespace gen
{
typedef unsigned long   ULONG;
typedef unsigned int    UINT;
typedef unsigned short  USHORT;
typedef unsigned char   UCHAR;

typedef boost::uint64_t QWORD;
typedef boost::uint32_t DWORD;
typedef boost::uint16_t WORD;
typedef boost::uint8_t  BYTE;
}
*/

#ifdef __APPLE__
typedef unsigned long   ULONG;
typedef unsigned int    UINT;
typedef unsigned short  USHORT;
typedef unsigned char   UCHAR;

typedef unsigned long long  QWORD;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif


/**********************************************************\
*    Constants
\**********************************************************/
namespace gen
{
const float PI       = 3.14159265358979323f;
const float MATH_EPS = 0.0001f;
}


/**********************************************************\
*    Helpers
\**********************************************************/
namespace gen
{
float GetRandomMinMax( float fMin, float fMax );
ci::Vec3f GetRandomVector( void );
}


/**********************************************************\
*    Inline
\**********************************************************/

inline float gen::GetRandomMinMax(float fMin, float fMax)
{
    float fRandNum = (float)rand () / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
}

inline ci::Vec3f gen::GetRandomVector(void)
{
    ci::Vec3f vVector;

    // Pick a random Z between -1.0f and 1.0f.
    vVector.z = GetRandomMinMax( -1.0f, 1.0f );
    
    // Get radius of this circle
    float radius = (float)sqrt(1 - vVector.z * vVector.z);
    
    // Pick a random point on a circle.
    float t = GetRandomMinMax( -PI, PI );

    // Compute matching X and Y for our Z.
    vVector.x = (float)cosf(t) * radius;
    vVector.y = (float)sinf(t) * radius;

	return vVector;
}
