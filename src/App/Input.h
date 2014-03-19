
#pragma once

#include "Common.h"
#include "Gamepad.h"
#include "Shell.h"
#include "ShellKeyCodes.h"
#include <map>

namespace gen
{
// prototypes
class ObjectManager;
class Player;

enum InputKey
{
    TOGGLE_FULLSCREEN   = ci::app::KeyEvent::KEY_f,
    TOGGLE_STATS        = ci::app::KeyEvent::KEY_s,
    TOGGLE_GUI          = ci::app::KeyEvent::KEY_g,
    TOGGLE_WIREFRAME    = ci::app::KeyEvent::KEY_h,
    ADVANCE_GAME_STATE  = ci::app::KeyEvent::KEY_TAB, 
    SHOOT_P1            = ci::app::KeyEvent::KEY_RETURN,
    MOVE_UP_P1          = ci::app::KeyEvent::KEY_UP,
    MOVE_LEFT_P1        = ci::app::KeyEvent::KEY_LEFT,
    MOVE_RIGHT_P1       = ci::app::KeyEvent::KEY_RIGHT,
    SHOOT_P2            = ci::app::KeyEvent::KEY_1,
    MOVE_UP_P2          = ci::app::KeyEvent::KEY_w,
    MOVE_LEFT_P2        = ci::app::KeyEvent::KEY_a,
    MOVE_RIGHT_P2       = ci::app::KeyEvent::KEY_d,
    RESTART             = ci::app::KeyEvent::KEY_r,
};

struct InputKeyState        // For continuous input
{
    // TODO: These expand to bytes. big deal?
    bool SHOOT_STATE;
    bool MOVE_UP_STATE;
    bool MOVE_LEFT_STATE;
    bool MOVE_RIGHT_STATE;
};

class Input
{
public:
    Input(Player* pPlayer);
    virtual ~Input(){}
    void Update(float dt);  // For continuous input

    bool KeyDown(ci::app::KeyEvent event);
    bool KeyUp(ci::app::KeyEvent event);
    bool MouseDown(ci::app::MouseEvent event);
    bool MouseDrag(ci::app::MouseEvent event);
    bool MouseUp(ci::app::MouseEvent event);
    bool MouseWheel(ci::app::MouseEvent event);
    bool TouchesBegan(ci::app::TouchEvent event);
    bool TouchesMoved(ci::app::TouchEvent event);
    bool TouchesEnded(ci::app::TouchEvent event);

    // joystick events
    static bool onButtonDown(void * sender, const char * eventID, void * eventData, void * context);
    static bool onButtonUp(void * sender, const char * eventID, void * eventData, void * context);
    static bool onAxisMoved(void * sender, const char * eventID, void * eventData, void * context);
    static bool onDeviceAttached(void * sender, const char * eventID, void * eventData, void * context);
    static bool onDeviceRemoved(void * sender, const char * eventID, void * eventData, void * context);

    static std::map<unsigned int, Input*> gamepadDeviceMap;

    InputKeyState m_keyState;
    ci::Arcball m_arcball;  // TODO: Should be private? Public for ease of App::Resize function

    // TODO: should be private
    Player*     m_pPlayer;
    void Fly();
    void FlyOff();
    void Shoot();
    
private:
    bool        m_selectState;
    float       m_selectStart;
    ci::Vec2i   m_selectPos;
    float       m_timeTap;
};

}
