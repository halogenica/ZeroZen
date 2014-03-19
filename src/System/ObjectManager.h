
#pragma once

#include "Common.h"
#include "Stats.h"
#include "Input.h"
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

namespace gen
{
// protos
class ObjectData;
class SpriteSheet;
class Player;
class Particle;

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    void Update(float dt);
    void Draw();
    void UpdateSoundTimer(float dt);    // this is its own function because the timing is very sensitive
    Player* AddPlayer();
    void Start();                       // transition from Ready to Playing state.
    void ResetGame();
    void Restart();
    void CheckWinners();

    ci::CameraPersp m_camera;

    GameState       m_gameState;
    bool            m_gravity;          // toggle to enable or disable gravity TODO: physics state?
    bool            m_displayStats;
    bool            m_displayGui;
    bool            m_fullscreen;
    bool            m_wireframe;

    bool            m_grid;
    bool            m_showStars;
    bool            m_playMusic;

    std::vector<ObjectData*> m_objects; // collection of assets
    std::vector<ObjectData*> m_gridObjects;
    std::vector<Player*> m_players;

    ci::gl::Fbo     m_fbo;
    ci::gl::Fbo::Format m_fboFormat;

    ci::gl::Texture m_texTitle;    
    ci::gl::Texture m_texCredits;    
    ci::gl::Texture m_texReady;
    ci::gl::Texture m_texGo;
    ci::gl::Texture m_texP1Wins;
    ci::gl::Texture m_texP2Wins;
    ci::gl::Texture m_texP3Wins;
    ci::gl::Texture m_texP4Wins;

    float           m_minZoom;
    float           m_maxZoom;
    float           m_curZoom;

    float           m_winScreenTime;
    float           m_readyScreenTime;
    float           m_titleScreenTime;
    float           m_creditScreenTime;
    float           m_goScreenTime;
    bool            m_readyToPlay;
    bool            m_waitingToStart;   // start has been requested, waiting for the right time to switch state

    double          m_loopPlaytime;
    double          m_bgTrackLength;

    ci::audio::SourceRef m_bgSoundIntro;
    std::vector<ci::audio::SourceRef> m_bgSounds;

    ci::gl::GlslProg    m_shader;

    boost::asio::io_service     m_io;
    boost::asio::serial_port    m_port;

private:
    void AutoZoom();
    void DrawScore();
    void DrawScoreDigit(UINT digit, ci::Area bounds);
    void UpdateStarfield(float dt);
    void DrawStarfield();

    ci::gl::Texture m_sDash;
    ci::gl::Texture m_s0;
    ci::gl::Texture m_s1;
    ci::gl::Texture m_s2;
    ci::gl::Texture m_s3;
    ci::gl::Texture m_s4;
    ci::gl::Texture m_s5;
    ci::gl::Texture m_s6;
    ci::gl::Texture m_s7;
    ci::gl::Texture m_s8;
    ci::gl::Texture m_s9;

    std::vector<Particle*> m_stars;
};
}
