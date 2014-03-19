#include "Input.h"
#include "ObjectManager.h"
#include "App.h"
#include "ObjectData.h"
#include "PhysicsData.h"
#include "RenderData.h"
#include "Player.h"

using namespace ci;

std::map<unsigned int, gen::Input*> gen::Input::gamepadDeviceMap;

/**************************************
Gamepad code
**************************************/
#ifdef _DEBUG
bool verbose = true;
#else
bool verbose = false;
#endif

bool gen::Input::onButtonDown(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_buttonEvent * event;
	
	event = (Gamepad_buttonEvent*)eventData;
	if (verbose) {
//        printf("Button %u down (%d) on device %u at %f\n", event->buttonID, (int) event->down, event->device->deviceID, event->timestamp);
//        ci::app::console() << "Button " << event->buttonID << " down " << event->down << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
#ifdef __APPLE__
        if (event->device->vendorID == 1356)    // PS3 Controller
        {
            if (event->buttonID == 14)
            {
                pPlayerInput->Fly();
                pPlayerInput->m_keyState.MOVE_UP_STATE = true;
            }
            if (event->buttonID == 15)
            {
                pPlayerInput->Shoot();
                pPlayerInput->m_keyState.SHOOT_STATE = true;
            }
        }
        else                                    // 360 Controller
        {
            if (event->buttonID == 16)
            {
                pPlayerInput->Fly();
                pPlayerInput->m_keyState.MOVE_UP_STATE = true;
            }
            if (event->buttonID == 18)
            {
                pPlayerInput->Shoot();
                pPlayerInput->m_keyState.SHOOT_STATE = true;
            }
        }
#else
        if (event->buttonID == 0)
        {
            pPlayerInput->Fly();
            pPlayerInput->m_keyState.MOVE_UP_STATE = true;
        }
        if (event->buttonID == 2)
        {
            pPlayerInput->Shoot();
            pPlayerInput->m_keyState.SHOOT_STATE = true;
        }
#endif
    }

    return true;
}

bool gen::Input::onButtonUp(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_buttonEvent * event;
	
	event = (Gamepad_buttonEvent*)eventData;
	if (verbose) {
//        printf("Button %u up (%d) on device %u at %f\n", event->buttonID, (int) event->down, event->device->deviceID, event->timestamp);
//        ci::app::console() << "Button " << event->buttonID << " up " << event->down << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
#ifdef __APPLE__
        if (event->device->vendorID == 1356)    // PS3 Controller
        {
            if (event->buttonID == 14)
            {
                pPlayerInput->FlyOff();
                pPlayerInput->m_keyState.MOVE_UP_STATE = false;
            }
            if (event->buttonID == 15)
            {
                pPlayerInput->m_keyState.SHOOT_STATE = false;
            }
        }
        else                                    // 360 Controller
        {
            if (event->buttonID == 16)
            {
                pPlayerInput->FlyOff();
                pPlayerInput->m_keyState.MOVE_UP_STATE = false;
            }
            if (event->buttonID == 18)
            {
                pPlayerInput->m_keyState.SHOOT_STATE = false;
            }
        }
#else
        if (event->buttonID == 0)
        {
            pPlayerInput->FlyOff();
            pPlayerInput->m_keyState.MOVE_UP_STATE = false;
        }
        if (event->buttonID == 2)
        {
            pPlayerInput->m_keyState.SHOOT_STATE = false;
        }
#endif
    }

	return true;
}

bool gen::Input::onAxisMoved(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_axisEvent * event;
	
	event = (Gamepad_axisEvent*)eventData;
	if (verbose) {
//        printf("Axis %u moved to %f on device %u at %f\n", event->axisID, event->value, event->device->deviceID, event->timestamp);
//        ci::app::console() << "Axis " << event->axisID << " moved to " << event->value << " on device " << event->device->deviceID << " at " << event->timestamp << std::endl;
	}

    gen::Input* pPlayerInput = NULL;
    pPlayerInput = gamepadDeviceMap[event->device->deviceID];

    if (pPlayerInput)
    {
        if (event->axisID == 0)
        {
            if (event->value <= -0.2f)
            {
                pPlayerInput->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,event->value * -5));
            }
            else if (event->value >= 0.2)
            {
                pPlayerInput->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,event->value * -5));
            }
            else
            {
                pPlayerInput->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,0));
                pPlayerInput->m_keyState.MOVE_LEFT_STATE = false;
                pPlayerInput->m_keyState.MOVE_RIGHT_STATE = false;
            }
        }
    }
    
    return true;
}

bool gen::Input::onDeviceAttached(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_device * device;
	
    device = (Gamepad_device*)eventData;
    if (verbose) {
//        printf("Device ID %u attached (vendor = 0x%X; product = 0x%X)\n", device->deviceID, device->vendorID, device->productID);
//        ci::app::console() << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID << "; product = " << device->productID <<")" << std::endl;
    }
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_DOWN, onButtonDown, device);
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_BUTTON_UP, onButtonUp, device);
	device->eventDispatcher->registerForEvent(device->eventDispatcher, GAMEPAD_EVENT_AXIS_MOVED, onAxisMoved, device);

    if (gamepadDeviceMap.size() == 0)
    {
        // g_pInputP1 is initialized by default
        gamepadDeviceMap[device->deviceID] = g_pInputP1;
    }
    else if (gamepadDeviceMap.size() == 1)
    {
        // g_pInputP2 is initialized by default
        gamepadDeviceMap[device->deviceID] = g_pInputP2;
    }
    else if (gamepadDeviceMap.size() == 2)
    {
        g_pInputP3 = new Input(g_pObjectManager->AddPlayer());
        gamepadDeviceMap[device->deviceID] = g_pInputP3;
    }
    else if (gamepadDeviceMap.size() == 3)
    {
        g_pInputP4 = new Input(g_pObjectManager->AddPlayer());
        gamepadDeviceMap[device->deviceID] = g_pInputP4;
    }
    else
    {
        ci::app::console() << "Only up to 4 players are supported!" << std::endl;
    }


	return true;
}

bool gen::Input::onDeviceRemoved(void * sender, const char * eventID, void * eventData, void * context) {
	struct Gamepad_device * device;
	
	device = (Gamepad_device*)eventData;
	if (verbose) {
		printf("Device ID %u removed\n", device->deviceID);
	}
    // TODO : put gamepad/input delete here?
	return true;
}


gen::Input::Input(Player* pPlayer) : m_pPlayer(pPlayer), m_keyState(), m_selectState(false), m_selectStart(0.0f), m_selectPos(0,0), m_timeTap(0)
{
}

#define POLL_ITERATION_INTERVAL 10

void gen::Input::Update(float dt)
{
	static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	
	iterationsToNextPoll--;
	if (iterationsToNextPoll == 0) {
		Gamepad_detectDevices();
		iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	}
    
    Gamepad_processEvents();

    // handle constant "button down" events, like flying

    if ((g_pObjectManager->m_gameState == PLAYING || g_pObjectManager->m_gameState == READY) && m_pPlayer->m_alive)
    {
        if (m_keyState.MOVE_LEFT_STATE)
        {
            m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,5));
        }
        else if (m_keyState.MOVE_RIGHT_STATE)
        {
            m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,-5));
        }
    }
    
    if (g_pObjectManager->m_gameState == PLAYING  && m_pPlayer->m_alive)
    {
        if (m_keyState.MOVE_UP_STATE)
        {
            btVector3 force = btVector3(0,0,0);
            Vec3f orient = Vec3f(0,10,0);
            if (m_pPlayer->GetRot().getAngle() != 0)
            {
                orient.rotate(m_pPlayer->GetRot().getAxis(), m_pPlayer->GetRot().getAngle());
            }
            force.setValue(orient.x, orient.y, orient.z);
//            pPlayer->m_pObjectData->m_pPhysicsData->m_pBtRigidBody->setLinearVelocity(vel);
            m_pPlayer->m_pPhysicsData->m_pBtRigidBody->applyForce(force, btVector3(0,0,0));
        }
    }
}

bool gen::Input::KeyDown(ci::app::KeyEvent event)
{
    switch (event.getCode())
    {
#ifdef _DEBUG
        case TOGGLE_STATS:
            g_pObjectManager->m_displayStats = !g_pObjectManager->m_displayStats;
            break;

        case TOGGLE_WIREFRAME:
            g_pObjectManager->m_wireframe = !g_pObjectManager->m_wireframe;
            break;

        case ADVANCE_GAME_STATE:
            switch (g_pObjectManager->m_gameState)
            {
                case TITLE:
                    g_pObjectManager->m_gameState = READY;
                    break;
                case READY:
                    g_pObjectManager->m_gameState = PLAYING;
                    break;
                case PLAYING:
                    g_pObjectManager->m_gameState = P1_WINS;
                    break;
                case P1_WINS:
                    g_pObjectManager->m_gameState = P2_WINS;
                    break;
                case P2_WINS:
                    g_pObjectManager->m_gameState = P3_WINS;
                    break;
                case P3_WINS:
                    g_pObjectManager->m_gameState = P4_WINS;
                    break;
                case P4_WINS:
                    g_pObjectManager->m_gameState = READY;
                    break;
                default:
                    break;
            }
            break;
#endif
        case TOGGLE_FULLSCREEN:
            g_pObjectManager->m_fullscreen = !g_pObjectManager->m_fullscreen;
            g_pApp->setFullScreen(g_pObjectManager->m_fullscreen);
            g_pObjectManager->m_fbo = gl::Fbo(g_pApp->getWindowWidth()*ci::app::getWindow()->getContentScale(),
                                              g_pApp->getWindowHeight()*ci::app::getWindow()->getContentScale(),
                                              g_pObjectManager->m_fboFormat);
            break;

        case TOGGLE_GUI:
            g_pObjectManager->m_displayGui = !g_pObjectManager->m_displayGui;
            break;

        case SHOOT_P1:
            if (!g_pInputP1->m_keyState.SHOOT_STATE)
            {
                g_pInputP1->Shoot();
                g_pInputP1->m_keyState.SHOOT_STATE = true;
            }
            break;

        case MOVE_UP_P1:
            if (g_pInputP1->m_keyState.MOVE_UP_STATE == false)
            {
                g_pInputP1->Fly();
            }
            g_pInputP1->m_keyState.MOVE_UP_STATE = true;
            break;

        case MOVE_LEFT_P1:
            g_pInputP1->m_keyState.MOVE_LEFT_STATE = true;
            g_pInputP1->m_keyState.MOVE_RIGHT_STATE = false;
            break;

        case MOVE_RIGHT_P1:
            g_pInputP1->m_keyState.MOVE_RIGHT_STATE = true;
            g_pInputP1->m_keyState.MOVE_LEFT_STATE = false;
            break;

        case SHOOT_P2:
            if (!g_pInputP2->m_keyState.SHOOT_STATE)
            {
                g_pInputP2->Shoot();
                g_pInputP2->m_keyState.SHOOT_STATE = true;
            }
            break;

        case MOVE_UP_P2:
            if (g_pInputP2->m_keyState.MOVE_UP_STATE == false)
            {
                g_pInputP2->Fly();
            }
            g_pInputP2->m_keyState.MOVE_UP_STATE = true;
            break;

        case MOVE_LEFT_P2:
            g_pInputP2->m_keyState.MOVE_LEFT_STATE = true;
            g_pInputP2->m_keyState.MOVE_RIGHT_STATE = false;
            break;

        case MOVE_RIGHT_P2:
            g_pInputP2->m_keyState.MOVE_RIGHT_STATE = true;
            g_pInputP2->m_keyState.MOVE_LEFT_STATE = false;
            break;

        case RESTART:
            if (g_pObjectManager->m_gameState == READY)
            {
                g_pObjectManager->Restart();
            }
            break;

        case ci::app::KeyEvent::KEY_ESCAPE:
            g_pObjectManager->m_gameState = CREDITS;
            break;

        default:
            return false;   // the input was not handled
            break;
    }

    return true;    // the input was handled
}

bool gen::Input::KeyUp(ci::app::KeyEvent event)
{
    switch (event.getCode())
    {
        case MOVE_UP_P1:
            g_pInputP1->m_keyState.MOVE_UP_STATE = false;
            g_pInputP1->FlyOff();
            break;

        case MOVE_LEFT_P1:
            g_pInputP1->m_keyState.MOVE_LEFT_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_RIGHT_STATE == false)
            {
                g_pInputP1->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,0));
            }
            break;

        case MOVE_RIGHT_P1:
            g_pInputP1->m_keyState.MOVE_RIGHT_STATE = false;
            if (g_pInputP1->m_keyState.MOVE_LEFT_STATE == false)
            {
                g_pInputP1->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,0));
            }
            break;

        case SHOOT_P1:
            g_pInputP1->m_keyState.SHOOT_STATE = false;
            break;

        case MOVE_UP_P2:
            g_pInputP2->m_keyState.MOVE_UP_STATE = false;
            g_pInputP2->FlyOff();
            break;

        case MOVE_LEFT_P2:
            g_pInputP2->m_keyState.MOVE_LEFT_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_RIGHT_STATE == false)
            {
                g_pInputP2->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,0));
            }
            break;

        case MOVE_RIGHT_P2:
            g_pInputP2->m_keyState.MOVE_RIGHT_STATE = false;
            if (g_pInputP2->m_keyState.MOVE_LEFT_STATE == false)
            {
                g_pInputP2->m_pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularVelocity(btVector3(0,0,0));
            }
            break;

        case SHOOT_P2:
            g_pInputP2->m_keyState.SHOOT_STATE = false;
            break;

        default:
            return false;   // the input was not handled
            break;
    }

    return true;
}

bool gen::Input::MouseDown(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::MouseDrag(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::MouseUp(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::MouseWheel(ci::app::MouseEvent event)
{
    return false;
}

bool gen::Input::TouchesBegan(ci::app::TouchEvent event)
{
    return false;
}

bool gen::Input::TouchesMoved(ci::app::TouchEvent event)
{
    return false;
}

bool gen::Input::TouchesEnded(ci::app::TouchEvent event)
{
    return false;
}

void gen::Input::Fly()
{
    if (g_pObjectManager->m_gameState == TITLE)
    {
        if (g_pObjectManager->m_titleScreenTime >= 5.0f)
        {
            g_pObjectManager->ResetGame();
        }
    }
    else if (g_pObjectManager->m_gameState == READY)
    {
        if (g_pObjectManager->m_readyToPlay)
        {
            g_pObjectManager->Start();
            m_pPlayer->QueueFly();
        }
    }
    else if (g_pObjectManager->m_gameState == PLAYING && m_pPlayer->m_alive)
    {
        m_pPlayer->Fly();
    }
}

void gen::Input::FlyOff()
{
    m_pPlayer->FlyOff();
}   

void gen::Input::Shoot()
{
    if (g_pObjectManager->m_gameState == TITLE)
    {
        if (g_pObjectManager->m_titleScreenTime >= 5.0f)
        {
            g_pObjectManager->ResetGame();
        }
    }
    else if (g_pObjectManager->m_gameState == READY)
    {
        if (g_pObjectManager->m_readyToPlay)
        {
            g_pObjectManager->Start();
        }
    }
    else if (g_pObjectManager->m_gameState == PLAYING && m_pPlayer->m_alive)
    {
        m_pPlayer->Shoot();
    }
}