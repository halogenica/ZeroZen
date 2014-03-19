
#include "App.h"
#include "Common.h"
#include "ObjectManager.h"
#include "PhysicsData.h"
#include "SimpleGUI.h"

using namespace gen;

// externs
extern ContactProcessedCallback gContactProcessedCallback;

// global declarations
gen::Stats*                 g_pStats;               // the graphics and physics statistics
gen::App*                   g_pApp;                 // the application
gen::Input*                 g_pInputP1 = NULL;      // the input control system
gen::Input*                 g_pInputP2 = NULL;      // the input control system
gen::Input*                 g_pInputP3 = NULL;      // the input control system
gen::Input*                 g_pInputP4 = NULL;      // the input control system
gen::ObjectManager*         g_pObjectManager;       // the object manager
btDiscreteDynamicsWorld*    g_pBtDynamicsWorld;     // the physics world
mowa::sgui::SimpleGUI*      g_pGui;                 // the GUI

// timestep variables
unsigned long       timeNow;        // the timestamp of the current frame
unsigned long       timePrev;       // the timestamp of the previous frame
unsigned long       timeFrame;      // the time delta for the current frame
unsigned long       timeSum;        // accumulation of time
const unsigned long timeDelta = 1;  // the constant update time delta
const unsigned long timeMax = 16;   // the constant maximum time delta

// bullet variables
btBroadphaseInterface*                  pBtBroadphase;              // the broadphase
btDefaultCollisionConfiguration*        pBtCollisionConfiguration;  // the collision config
btCollisionDispatcher*                  pBtDispatcher;              // the dispatch config
btSequentialImpulseConstraintSolver*    pBtSolver;                  // the physics solver
btCollisionShape*                       pBtGroundShape;             // the ground
btDefaultMotionState*                   pBtGroundMotionState;
btRigidBody*                            pBtGroundRigidBody;

// functions
void gen::App::prepareSettings(Settings *settings)
{
    settings->setWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	settings->enableHighDensityDisplay();
    settings->setFrameRate(60.0f);
}

void gen::App::setup()
{
    g_pApp = this;

    timePrev = ci::app::getElapsedSeconds();
    timeNow  = 0;
    timeSum = 0;

    InitBullet();   // Must init bullet before statics
    g_pStats = new Stats();
    g_pGui = new mowa::sgui::SimpleGUI(g_pApp);
    g_pObjectManager = new ObjectManager();
    g_pInputP1 = new gen::Input(g_pObjectManager->AddPlayer()); // always have 2 players for keyboard input
    g_pInputP2 = new gen::Input(g_pObjectManager->AddPlayer()); // always have 2 players for keyboard input
    InitGamepad();

    this->getWindow()->connectKeyDown(&gen::Input::KeyDown, g_pInputP1);
    this->getWindow()->connectKeyUp(&gen::Input::KeyUp, g_pInputP1);

    // With all subsystems initialized, setup the game
//    g_pObjectManager->ResetGame();
}

void gen::App::shutdown()
{
    // Destroy the game before the subsystems disappear
//    g_pObjectManager->DestroyGame();

    // Cinder destroys "this" App object
    if (g_pInputP1)
    {
        delete g_pInputP1;
    }
    if (g_pInputP2)
    {
        delete g_pInputP2;
    }
    if (g_pInputP3)
    {
        delete g_pInputP3;
    }
    if (g_pInputP4)
    {
        delete g_pInputP4;
    }
    delete g_pObjectManager;
    delete g_pGui;
    delete g_pStats;
    DestroyBullet();
//    Gamepad_shutdown();
//    _CrtDumpMemoryLeaks();  // TODO: remove code checking for memory leaks
}

void gen::App::update()
{
    timeNow = ci::app::getElapsedSeconds() * 1000.0f ;
    timeFrame = timeNow - timePrev;
    timePrev = timeNow;

    g_pStats->updateFPS(timeFrame * .001f);    // g_pStats uses time in seconds
    g_pObjectManager->UpdateSoundTimer(timeFrame * .001f);

    // practical limitation, don't let updates overcome framerate
    if (timeFrame > timeMax)
    {
        timeFrame = timeMax;
    }

    timeSum += timeFrame;

    // update at a rate of timeDiff, regardless of framerate
    while (timeSum >= timeDelta)
    {
        UpdateTimestep(timeDelta * .001f);
        g_pBtDynamicsWorld->stepSimulation(timeDelta*.001f, 1, timeMax*.001f);
        g_pStats->updateUPS(timeDelta * .001f);    // g_pStats uses time in seconds
        timeSum -= timeDelta;
    }

    if (g_pObjectManager->m_creditScreenTime >= 5.0f)
    {
        if (g_pObjectManager->m_playMusic)
        {
            if (g_pObjectManager->m_bgTrackLength <= g_pObjectManager->m_loopPlaytime)
            {
                g_pApp->quit();
            }
        }
        else
        {
            g_pApp->quit();
        }
    }
}

void gen::App::draw()
{
    // set state

    // begin
    
    // draw
    g_pObjectManager->Draw();

    // end
    
    // present
}

void gen::App::resize()
{
    if (g_pInputP1)
    {
        g_pInputP1->m_arcball.setWindowSize(getWindowSize());
        g_pInputP1->m_arcball.setCenter(getWindowCenter());
        g_pInputP1->m_arcball.setRadius(getWindowSize().y/2);
    }
}

void gen::App::InitGamepad()
{
    Gamepad_eventDispatcher()->registerForEvent(Gamepad_eventDispatcher(), GAMEPAD_EVENT_DEVICE_ATTACHED, gen::Input::onDeviceAttached, NULL);
    Gamepad_eventDispatcher()->registerForEvent(Gamepad_eventDispatcher(), GAMEPAD_EVENT_DEVICE_REMOVED, gen::Input::onDeviceRemoved, NULL);
    Gamepad_init();
}

void gen::App::InitBullet()
{
    // Build the broadphase
    pBtBroadphase = new btDbvtBroadphase();
 
    // Set up the collision configuration and dispatcher
    pBtCollisionConfiguration = new btDefaultCollisionConfiguration();
    pBtDispatcher = new btCollisionDispatcher(pBtCollisionConfiguration);
 
    // The actual physics solver
    pBtSolver = new btSequentialImpulseConstraintSolver;
 
    // The physics world
    g_pBtDynamicsWorld = new btDiscreteDynamicsWorld(pBtDispatcher,pBtBroadphase,pBtSolver,pBtCollisionConfiguration);
    g_pBtDynamicsWorld->setGravity(btVector3(0, GRAVITY_CONST, 0));

    gContactProcessedCallback = (ContactProcessedCallback)PhysicsData::CollisionCallback;
}

void gen::App::UpdateTimestep(float dt)
{
    g_pObjectManager->Update(dt);

    if (g_pInputP1)
    {
        g_pInputP1->Update(dt);
    }
    if (g_pInputP2)
    {
        g_pInputP2->Update(dt);
    }
    if (g_pInputP3)
    {
        g_pInputP3->Update(dt);
    }
    if (g_pInputP4)
    {
        g_pInputP4->Update(dt);
    }

}

void gen::App::DestroyBullet()
{
    delete g_pBtDynamicsWorld;
    delete pBtSolver;
    delete pBtDispatcher;
    delete pBtCollisionConfiguration;
    delete pBtBroadphase;
    delete pBtGroundShape;
}
