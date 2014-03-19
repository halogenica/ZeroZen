
#include "Common.h"
#include "Stats.h"
#include "SimpleGUI.h"
#include "ObjectManager.h"

#include <sstream>

#define STRINGIFY( name ) # name

const char* gameStateNames [] =
{
    STRINGIFY(TITLE),
    STRINGIFY(READY),
    STRINGIFY(PLAYING),
    STRINGIFY(P1_WINS),
    STRINGIFY(P2_WINS),
    STRINGIFY(P3_WINS),
    STRINGIFY(P4_WINS),
    STRINGIFY(CREDITS),
};

using namespace std;
using namespace gen;

Stats::Stats()
: m_FPS(0.0f), m_UPS(0.0f), m_MilliSecPerFrame(0.0f), m_NumTris(0), m_NumVertices(0), m_NumRigidBodies(0), m_CursorX(0), m_CursorY(0)
{
    ASSERT(NUM_GAME_STATES == (sizeof(gameStateNames) / sizeof (gameStateNames[0])));
    m_TextFont = ci::gl::TextureFont::create(ci::Font(ci::app::loadResource(RES_STATS_FONT), 24)); // pf_tempesta_seven.ttf has problems with new line
    m_TextBoundsRect = ci::Rectf(40, m_TextFont->getAscent() + 20, getWindowWidth(), getWindowHeight());
}

Stats::~Stats()
{
}

void Stats::OnLostDevice()
{
}

void Stats::OnResetDevice()
{
}

void Stats::addVertices(unsigned int n)         {m_NumVertices += n;     }
void Stats::subVertices(unsigned int n)         {m_NumVertices -= n;     }
void Stats::addTriangles(unsigned int n)        {m_NumTris += n;         }
void Stats::subTriangles(unsigned int n)        {m_NumTris -= n;         }
void Stats::addRigidBodies(unsigned int n)      {m_NumRigidBodies += n;  }
void Stats::subRigidBodies(unsigned int n)      {m_NumRigidBodies -= n;  }
void Stats::setTriCount(unsigned int n)         {m_NumTris = n;          }
void Stats::setVertexCount(unsigned int n)      {m_NumVertices = n;      }
void Stats::setRigidBodyCount(unsigned int n)   {m_NumRigidBodies = n;   }

void Stats::setCursorPosition(float x, float y) {m_CursorX = x; m_CursorY = y;}

void Stats::updateFPS(float dt)
{
	// Make static so that their values persist accross function calls.
	static float numFrames   = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numFrames += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		m_FPS = numFrames;

		// Average time, in miliseconds, it took to render a single frame.
		m_MilliSecPerFrame = 1000.0f / m_FPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numFrames   = 0.0f;
	}
}

void Stats::updateUPS(float dt)
{
	// Make static so that their values persist accross function calls.
	static float numUpdates  = 0.0f;
	static float timeElapsed = 0.0f;

	// Increment the frame count.
	numUpdates += 1.0f;

	// Accumulate how much time has passed.
	timeElapsed += dt;

	// Has one second passed?--we compute the frame statistics once 
	// per second.  Note that the time between frames can vary so 
	// these stats are averages over a second.
	if( timeElapsed >= 1.0f )
	{
		// Frames Per Second = numFrames / timeElapsed,
		// but timeElapsed approx. equals 1.0, so 
		// frames per second = numFrames.

		m_UPS = numUpdates;

		// Average time, in miliseconds, it took to render a single frame.
		m_MilliSecPerUpdate = 1000.0f / m_UPS;

		// Reset time counter and frame count to prepare for computing
		// the average stats over the next second.
		timeElapsed = 0.0f;
		numUpdates   = 0.0f;
	}
}

void Stats::display()
{
    stringstream displayString;
    displayString << "Frames Per Second: "  << setiosflags(ios::fixed) << setprecision(2) << m_FPS << endl;
    displayString << "Updates Per Second: " << setiosflags(ios::fixed) << setprecision(2) << m_UPS << endl;
    displayString << "Ms Per Frame: "       << setiosflags(ios::fixed) << setprecision(4) << m_MilliSecPerFrame << endl;
    displayString << "Triangle Count: "     << m_NumTris << endl;
    displayString << "Vertex Count: "       << m_NumVertices << endl;
    displayString << "Rigid Bodies: "       << m_NumRigidBodies << endl;
    displayString << "Game State: "         << gameStateNames[g_pObjectManager->m_gameState] << endl;
    
    gl::pushMatrices();
    gl::setMatricesWindow(getWindowSize());
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::enableAlphaBlending();

    ci::gl::color(Color::white());
    m_TextFont->drawStringWrapped(displayString.str(), m_TextBoundsRect, ci::Vec2f(0,0));

    gl::disableAlphaBlending();
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::color(ColorA(1,1,1,1));
    gl::popMatrices();
}