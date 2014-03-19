
#include "ObjectManager.h"
#include "App.h"
#include "SimpleGUI.h"
#include "ObjectData.h"
#include "RenderData.h"
#include "PhysicsData.h"
#include "SpriteSheet.h"
#include "Player.h"
#include "Particle.h"
#include "SquareParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

#ifdef __APPLE__
std::string ARDUINO_COM_PORT = "/dev/cu.usbmodem1421";
#else
std::string ARDUINO_COM_PORT = "COM4";
#endif
int ARDUINO_BAUD_RATE = 19200;

ObjectManager::ObjectManager() : m_io(), m_port(m_io)
{
    m_minZoom = 10.0f;
    m_maxZoom = 40.0f;
    m_curZoom = m_maxZoom;

    // Setup camera
    m_camera.setPerspective(60, getWindowAspectRatio(), 1, 1000);
    m_camera.lookAt(Vec3f(0, 0, m_curZoom), Vec3f::zero());

    // Initialize controls
    m_gameState     = TITLE;
    m_gravity       = false;
    m_displayStats  = false;
    m_displayGui    = false;
    m_fullscreen    = false;
    m_wireframe     = false;

    m_grid          = true;
    m_showStars     = true;
    m_playMusic     = true;

    m_winScreenTime = 0;
    m_titleScreenTime = 0;
    m_creditScreenTime = 0;
    m_readyScreenTime = 0;
    m_goScreenTime = 0;
    m_readyToPlay = false;
    m_waitingToStart = false;

    g_pApp->setFullScreen(m_fullscreen);

    if (m_gravity)
    {
        g_pBtDynamicsWorld->setGravity(btVector3(0, GRAVITY_CONST, 0));
    }
    else
    {
        g_pBtDynamicsWorld->setGravity(btVector3(0, 0, 0));
    }

    // Open a serial connection if applicable
    try 
    {
        m_port.open(ARDUINO_COM_PORT);
    }
    catch(const std::exception& e)
    {
        ci::app::console() << e.what() << std::endl;
    }
    if (m_port.is_open())
    {
        m_port.set_option(boost::asio::serial_port_base::baud_rate(ARDUINO_BAUD_RATE));
        boost::asio::write(m_port, boost::asio::buffer("p"));
    }

    // Load assets
/*
    // random sphere
    ObjectData* pSphere = new ObjectData(new RenderData(ci::app::loadResource(RES_SPHERE_OBJ), ci::ColorAf(0.0f, 0.0f, 1.0f, 1.0f)), 
                                         new PhysicsData(new btSphereShape(0.5f), 10, COL_PLAYER, COL_EVERYTHING), 
                                         Vec3f(0,0,0), Quatf(0,0,0), Vec3f(1.f,1.f,1.f));
    m_objects.push_back(pSphere);
    pSphere->m_pRenderData->m_cellshadePercent = 0.2;
    pSphere->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));     // cannot move in z-axis
    pSphere->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));    // can only rotate along z-axis
*/
    // random cube
/*
    ObjectData* pCube = new ObjectData(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), ci::ColorAf(0.4f, 0.4f, 0.4f, 1.0f)), 
                                       new PhysicsData(new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)), 10, COL_PLAYER, COL_EVERYTHING), 
                                       Vec3f(0,0,0), Quatf(0,0,0), Vec3f(1.f,1.f,1.f));
    m_objects.push_back(pCube);
    pCube->m_pRenderData->m_cellshadePercent = 0.2;
    pCube->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));     // cannot move in z-axis
    pCube->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));    // can only rotate along z-axis
*/

    // superspace box
    float boxWidth = 40;
    float boxHeight = boxWidth / getWindowAspectRatio();

    ObjectData* pBottomWall =   new ObjectData(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), ci::ColorAf(1.0f,0.0f,0.0f,1.0f)), 
                                               new PhysicsData(new btBoxShape(btVector3(boxWidth/2,0.5f,0.5f)), 0, COL_ENEMY, COL_EVERYTHING), 
                                               Vec3f(0,-boxHeight/2,0), Quatf(0,0,0), Vec3f(boxWidth*1.2,1.f,1.f));
    pBottomWall->m_pPhysicsData->m_pBtRigidBody->setMassProps(0, btVector3(0,0,0));   // disable all rotational movement by setting inertia to zero
    pBottomWall->m_pRenderData->m_cellshadePercent = 0.2;
    m_objects.push_back(pBottomWall);

    ObjectData* pTopWall =      new ObjectData(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), ci::ColorAf(1.0f,0.0f,0.0f,1.0f)), 
                                               new PhysicsData(new btBoxShape(btVector3(boxWidth/2,0.5f,0.5f)), 0, COL_ENEMY, COL_EVERYTHING), 
                                               Vec3f(0,boxHeight/2,0), Quatf(0,0,0), Vec3f(boxWidth*1.2,1.f,1.f));
    pTopWall->m_pPhysicsData->m_pBtRigidBody->setMassProps(0, btVector3(0,0,0));   // disable all rotational movement by setting inertia to zero
    pTopWall->m_pRenderData->m_cellshadePercent = 0.2;
    m_objects.push_back(pTopWall);

    ObjectData* pLeftWall =     new ObjectData(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), ci::ColorAf(1.0f,0.0f,0.0f,1.0f)), 
                                               new PhysicsData(new btBoxShape(btVector3(.5f,boxHeight/2,0.5f)), 0, COL_ENEMY, COL_EVERYTHING), 
                                               Vec3f(-boxWidth/2,0,0), Quatf(0,0,0), Vec3f(1.f,boxHeight*1.2,1.f));
    pLeftWall->m_pPhysicsData->m_pBtRigidBody->setMassProps(0, btVector3(0,0,0));   // disable all rotational movement by setting inertia to zero
    pLeftWall->m_pRenderData->m_cellshadePercent = 0.2;
    m_objects.push_back(pLeftWall);

    ObjectData* pRightWall =   new ObjectData(new RenderData(ci::app::loadResource(RES_CUBE_OBJ), ci::ColorAf(1.0f,0.0f,0.0f,1.0f)), 
                                              new PhysicsData(new btBoxShape(btVector3(.5f,boxHeight/2,0.5f)), 0, COL_ENEMY, COL_EVERYTHING), 
                                              Vec3f(boxWidth/2,0,0), Quatf(0,0,0), Vec3f(1.f,boxHeight*1.2,1.f));
    pRightWall->m_pPhysicsData->m_pBtRigidBody->setMassProps(0, btVector3(0,0,0));   // disable all rotational movement by setting inertia to zero
    pRightWall->m_pRenderData->m_cellshadePercent = 0.2;
    m_objects.push_back(pRightWall);

    // grid
    float spacing = 1.0f;
    for (float x = -boxWidth/2 + spacing; x < boxWidth/2; x+=spacing)
    {
        ObjectData* pLine = new ObjectData(new RenderData(ci::app::loadResource(RES_SQUARE_OBJ), ci::ColorAf(0.2,0.2,0.2,1.0)),
                                            NULL,
                                            Vec3f(x,0,-0.55), Quatf(0,0,0), Vec3f(0.05,boxHeight,1.f));
        m_gridObjects.push_back(pLine);
    }

    for (float y = -boxHeight/2 + spacing; y < boxHeight/2; y+=spacing)
    {
        ObjectData* pLine = new ObjectData(new RenderData(ci::app::loadResource(RES_SQUARE_OBJ), ci::ColorAf(0.2,0.2,0.2,1.0)),
                                            NULL,
                                            Vec3f(0,y,-0.55), Quatf(0,0,0), Vec3f(boxWidth,0.05,1.f));
        m_gridObjects.push_back(pLine);
    }

    // FBO
    m_fboFormat.setSamples(4);
    m_fbo = gl::Fbo(g_pApp->getWindowWidth()*getWindow()->getContentScale(),
                    g_pApp->getWindowHeight()*getWindow()->getContentScale(),
                    m_fboFormat);

    // screen textures
    m_texTitle = gl::Texture(ci::loadImage(ci::app::loadResource(RES_TITLE_TEX)));
    m_texTitle.setMinFilter(GL_NEAREST);    // disable bilinear filtering
    m_texTitle.setMagFilter(GL_NEAREST);    // disable bilinear filtering

    m_texCredits = gl::Texture(ci::loadImage(ci::app::loadResource(RES_CREDITS_TEX)));
    m_texCredits.setMinFilter(GL_NEAREST);  // disable bilinear filtering
    m_texCredits.setMagFilter(GL_NEAREST);  // disable bilinear filtering

    m_texReady = gl::Texture(ci::loadImage(ci::app::loadResource(RES_READY_TEX)));
    m_texReady.setMinFilter(GL_NEAREST);    // disable bilinear filtering
    m_texReady.setMagFilter(GL_NEAREST);    // disable bilinear filtering

    m_texGo = gl::Texture(ci::loadImage(ci::app::loadResource(RES_GO_TEX)));
    m_texGo.setMinFilter(GL_NEAREST);       // disable bilinear filtering
    m_texGo.setMagFilter(GL_NEAREST);       // disable bilinear filtering

    m_texP1Wins = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P1_WINS_TEX)));
    m_texP1Wins.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_texP1Wins.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_texP2Wins = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P2_WINS_TEX)));
    m_texP2Wins.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_texP2Wins.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_texP3Wins = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P3_WINS_TEX)));
    m_texP3Wins.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_texP3Wins.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_texP4Wins = gl::Texture(ci::loadImage(ci::app::loadResource(RES_P4_WINS_TEX)));
    m_texP4Wins.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_texP4Wins.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    // score object
    m_sDash = gl::Texture(ci::loadImage(ci::app::loadResource(RES_DASH_TEX)));
    m_sDash.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_sDash.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s0 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_0_TEX)));
    m_s0.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s0.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s1 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_1_TEX)));
    m_s1.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s1.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s2 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_2_TEX)));
    m_s2.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s2.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s3 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_3_TEX)));
    m_s3.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s3.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s4 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_4_TEX)));
    m_s4.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s4.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s5 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_5_TEX)));
    m_s5.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s5.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s6 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_6_TEX)));
    m_s6.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s6.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s7 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_7_TEX)));
    m_s7.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s7.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s8 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_8_TEX)));
    m_s8.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s8.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    m_s9 = gl::Texture(ci::loadImage(ci::app::loadResource(RES_9_TEX)));
    m_s9.setMinFilter(GL_NEAREST);   // disable bilinear filtering
    m_s9.setMagFilter(GL_NEAREST);   // disable bilinear filtering

    // Load background music
    // TODO: Cinder looping doesn't work
    // TODO: Cinder getDuration doesn't work: m_bgTrack->getDuration();
    float bpm = 180;
    float bars = 2;
    m_bgTrackLength = 60.f / bpm * 8 * bars;
    m_loopPlaytime = m_bgTrackLength;   // trick audio to think it's overdue for play

    m_bgSoundIntro = ci::audio::load(ci::app::loadResource(RES_BG_INTRO_SOUND));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG1_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG2_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG3_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG4_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG5_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG6_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG7_SOUND)));
    m_bgSounds.push_back(ci::audio::load(ci::app::loadResource(RES_BG8_SOUND)));

    // Setup GUI
    g_pGui->lightColor = ColorA(1, 0, 0, 1);
    g_pGui->addLabel("OPTIONS:");
    g_pGui->addSeparator();
    g_pGui->addParam("Show grid?", &m_grid, true);
    g_pGui->addParam("Show stars?", &m_showStars, true);
    g_pGui->addParam("Play music?", &m_playMusic, true);

    m_shader = gl::GlslProg(ci::app::loadResource(RES_VERT_SHADER), ci::app::loadResource(RES_FRAG_SHADER));

#ifdef _DEBUG
    int numStars = 50;
#else
    int numStars = 1000;
#endif

    for (int i = 0; i < numStars; i++)
    {
        m_stars.push_back(new SquareParticle());
    }
}

ObjectManager::~ObjectManager()
{
    if (m_port.is_open())
    {
        m_port.close();
    }

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        delete(*i);
    }

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        delete (*i);
    }

    for (auto i = m_stars.begin(); i != m_stars.end(); i++)
    {
        delete (*i);
    }
}

Player* ObjectManager::AddPlayer()
{
    Vec3f pos = Vec3f(0,0,0);
    Quatf rot = Quatf(0,0,0);
    ColorAf color = ColorAf(0,0,0,1);

    if (m_players.size() == 0)
    {
        pos = Vec3f(-3, 1.5, -0.02);  // slight z-offset for transparency ordering
        rot = Quatf(0,0,-PI*3/4);
        color = ColorAf(0, 0.7, 0.85, 1);
    }
    else if (m_players.size() == 1)
    {
        pos = Vec3f(3, -1.5, -0.01);  // slight z-offset for transparency ordering
        rot = Quatf(0,0,PI*1/4);
        color = ColorAf(0.42, 0.88, 0.2, 1);
    }
    else if (m_players.size() == 2)
    {
        pos = Vec3f(3, 1.5, 0.01);    // slight z-offset for transparency ordering
        rot = Quatf(0,0,PI*3/4);
        color = ColorAf(0.9, 0.21, 0.47, 1);
    }
    else if (m_players.size() == 3)
    {
        pos = Vec3f(-3, -1.5, 0.02);  // slight z-offset for transparency ordering
        rot = Quatf(0,0,-PI*1/4);
        color = ColorAf(1, 0.56, 0.17, 1);
    }
    else
    {
        ASSERT(false);
    }

    Player* pPlayer = new Player(new RenderData(ci::app::loadResource(RES_SPHERE_OBJ), color),
                                 new PhysicsData(new btSphereShape(0.25f), 1, COL_PLAYER, COL_EVERYTHING), 
                                 pos, rot, Vec3f(0.35f, 0.35f, 0.35f));
    m_players.push_back(pPlayer);
    pPlayer->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));       // cannot move in z-axis
    pPlayer->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));      // can only rotate along z-axis
    pPlayer->m_pPhysicsData->m_pBtRigidBody->setDamping(0.2, 0);                      // "air resistance"

    return pPlayer;
}

void ObjectManager::Start()
{
    m_winScreenTime = 0;
    m_readyToPlay = false;
    m_waitingToStart = true;;
}

void ObjectManager::ResetGame()
{
    if (m_port.is_open())
    {
        boost::asio::write(m_port, boost::asio::buffer("0"));
    }

    m_camera.lookAt(Vec3f(0, 0, m_maxZoom), Vec3f::zero());

    srand(time(NULL));

    m_loopPlaytime = m_bgTrackLength;
    m_waitingToStart = false;

    g_pObjectManager->m_readyScreenTime = 0;
    g_pObjectManager->m_goScreenTime = 0;
    g_pObjectManager->m_gameState = READY;

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        (*i)->Reset();
    }
}

void ObjectManager::Restart()
{
    if (m_port.is_open())
    {
        boost::asio::write(m_port, boost::asio::buffer("p"));
    }
    m_curZoom = m_maxZoom;
    m_camera.lookAt(Vec3f(0, 0, m_curZoom), Vec3f::zero());

    srand(time(NULL));

    m_loopPlaytime = m_bgTrackLength;
    m_waitingToStart = false;

    g_pObjectManager->m_readyScreenTime = 0;
    g_pObjectManager->m_goScreenTime = 0;
    g_pObjectManager->m_gameState = TITLE;

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        (*i)->Reset();
        (*i)->m_scoreWins = 0;
    }

    m_winScreenTime = 0;
    m_titleScreenTime = 0;
    m_readyScreenTime = 0;
    m_goScreenTime = 0;
}

void ObjectManager::CheckWinners()
{
    if (m_gameState != CREDITS)
    {
        bool alive[4] = {false, false, false, false};

        UINT playerIndex = 0;
        for (auto i = m_players.begin(); i != m_players.end(); i++)
        {
//        (*i)->Reset();
            alive[playerIndex] = (*i)->m_alive;
            ++playerIndex;
        }

        if (alive[0] && !alive[1] && !alive[2] && !alive[3])
        {
            m_gameState = P1_WINS;
            m_players[0]->m_scoreWins++;
        }
        else if (!alive[0] && alive[1] && !alive[2] && !alive[3])
        {
            m_gameState = P2_WINS;
            m_players[1]->m_scoreWins++;
        }
        else if (!alive[0] && !alive[1] && alive[2] && !alive[3])
        {
            m_gameState = P3_WINS;
            m_players[2]->m_scoreWins++;
        }
        else if (!alive[0] && !alive[1] && !alive[2] && alive[3])
        {
            m_gameState = P4_WINS;
            m_players[3]->m_scoreWins++;
        }

        if (m_players.size() == 1)
        {
            ci::app::console() << "One player is lonely! Invite some friends!" << std::endl;
            ResetGame();
        }
    }
}

void ObjectManager::AutoZoom()
{
    // THIS IS REALLY STUPID MATH
    // Don't do this. I'm just wasting time.

    Vec2f maxCoord = Vec2f(0,0);
    Vec3f curCoord = Vec3f(0,0,0);
    float oldZoom = m_curZoom;

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        curCoord = (*i)->GetPos();
        if (curCoord.x < 0)
        {
            curCoord.x = -curCoord.x;
        }
        if (curCoord.y < 0)
        {
            curCoord.y = -curCoord.y;
        }

        if (curCoord.x > maxCoord.x)
        {
            maxCoord.x = curCoord.x;
        }
        if (curCoord.y > maxCoord.y)
        {
            maxCoord.y = curCoord.y;
        }
    }

    float aspectRatio = m_camera.getAspectRatio();
    float fov = m_camera.getFov();

    maxCoord += Vec2f(2,2); // a little breathing room to see

    if (maxCoord.x > maxCoord.y * aspectRatio)
    {
        m_curZoom = maxCoord.x / cos((fov / 2) * PI / 180);
    }
    else
    {
        m_curZoom = maxCoord.y * aspectRatio / cos((fov / 2) * PI / 180);
    }

    if (oldZoom > m_curZoom + 0.01)
    {
        m_curZoom = oldZoom - 0.01;
    }

    if (m_curZoom < m_minZoom)
    {
        m_curZoom = m_minZoom;
    }
    else if (m_curZoom > m_maxZoom)
    {
        m_curZoom = m_maxZoom;
    }

    m_camera.lookAt(Vec3f(0, 0, m_curZoom), Vec3f::zero());
}

void ObjectManager::DrawScoreDigit(UINT digit, Area bounds)
{
    switch (digit)
    {
        case 0:
            gl::draw(m_s0, bounds);
            break;
        case 1:
            gl::draw(m_s1, bounds);
            break;
        case 2:
            gl::draw(m_s2, bounds);
            break;
        case 3:
            gl::draw(m_s3, bounds);
            break;
        case 4:
            gl::draw(m_s4, bounds);
            break;
        case 5:
            gl::draw(m_s5, bounds);
            break;
        case 6:
            gl::draw(m_s6, bounds);
            break;
        case 7:
            gl::draw(m_s7, bounds);
            break;
        case 8:
            gl::draw(m_s8, bounds);
            break;
        case 9:
            gl::draw(m_s9, bounds);
            break;
        default:
            ASSERT(false);
            break;
    }
}

void ObjectManager::DrawScore()
{
    UINT playerIndex = 0;

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        UINT wins = (*i)->m_scoreWins;
        UINT scoreHundreds = (wins/100)%10;
        UINT scoreTens = (wins/10)%10;
        UINT scoreOnes = wins%10;

        // set color
        if ((m_gameState == P1_WINS && playerIndex == 0) ||
            (m_gameState == P2_WINS && playerIndex == 1) ||
            (m_gameState == P3_WINS && playerIndex == 2) ||
            (m_gameState == P4_WINS && playerIndex == 3))
        {
            gl::color(1.f, 0.6 + (sin(m_winScreenTime*5) * 0.4), 0.6 + (sin(m_winScreenTime*5) * 0.4), 1.f);
        }
        else
        {
            gl::color(1.f, 1.f, 1.f, 1.f);
        }

        // calculate offsets and sizing
        gl::setMatricesWindow(getWindowSize());
        Area winBounds = getWindowBounds();
        float size = winBounds.getWidth()/16;   // in pixels
        Area texBounds = ci::Area(0,0,size,size); // the digit textures are square
        int xoffset = 0;    // start of the 3 digit display
        int yoffset = 0;    // start of the 3 digit display

        // STEPS:
        // always align 20 pixels from window edge (based on player number)
        // start from left, draw towards right
        // always display 3 digits

        if (playerIndex == 0)       // upper left
        {
            xoffset = 20;
            yoffset = 20;
        } 
        else if (playerIndex == 1)  // lower right
        {
            xoffset = winBounds.getWidth() - (texBounds.getWidth() * 3 * 0.8) - 20;
            yoffset = winBounds.getHeight() - (texBounds.getHeight()) - 20;
        }
        else if (playerIndex == 2)  // upper right
        {
            xoffset = winBounds.getWidth() - (texBounds.getWidth() * 3 * 0.8) - 20;
            yoffset = 20;
        }

        else if (playerIndex == 3)  // lower left
        {
            xoffset = 20;
            yoffset = winBounds.getHeight() - (texBounds.getHeight()) - 20;
        }
        else
        {
            ASSERT(false);
        }

        texBounds = texBounds + Vec2f(xoffset, yoffset);

        // for all 3 digits
        DrawScoreDigit(scoreHundreds, texBounds);
        texBounds = texBounds + Vec2f(texBounds.getWidth() * 0.8, 0);
        DrawScoreDigit(scoreTens, texBounds);
        texBounds = texBounds + Vec2f(texBounds.getWidth() * 0.8, 0);
        DrawScoreDigit(scoreOnes, texBounds);

        ++playerIndex;
    }
}

void ObjectManager::UpdateSoundTimer(float dt)
{
    m_loopPlaytime += dt;

    if (m_gameState == READY)
    {
        if (m_bgTrackLength/2 <= m_loopPlaytime && !m_waitingToStart)
        {
            m_loopPlaytime = 0;
            if (m_playMusic)
            {
#if PLAY_SOUND
                ci::audio::Output::play(m_bgSoundIntro);
#endif
            }
        }
    }
    else if (m_gameState == PLAYING)
    {
        if (m_bgTrackLength <= m_loopPlaytime)
        {
            m_loopPlaytime = 0;
            if (m_playMusic)
            {
#if PLAY_SOUND
                ci::audio::Output::play(m_bgSounds[rand()%m_bgSounds.size()]);  // play a random background song clip
#endif
            }
        }
    }
}


void ObjectManager::Update(float dt)
{
    // Update assets
    if (m_grid)
    {
        for (auto i = m_gridObjects.begin(); i != m_gridObjects.end(); i++)
        {
            (*i)->Update(dt);
        }
    }

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Update(dt);
    }

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        (*i)->Update(dt);
    }

    if (m_showStars)
    {
        for (auto i = m_stars.begin(); i != m_stars.end(); i++)
        {
            (*i)->Update(dt);
        }
    }

    if (m_gameState == TITLE)
    {
        m_titleScreenTime += dt;
    }
    else if (m_gameState == READY)
    {
        m_readyScreenTime += dt;
        if (m_readyScreenTime >= 2.0f)
        {
            m_readyToPlay = true;
        }
        else
        {
            m_curZoom = m_maxZoom - ((m_maxZoom -m_minZoom) * (m_readyScreenTime/2.0f));
            m_camera.lookAt(Vec3f(0, 0, m_curZoom), Vec3f::zero());
        }

        if (m_waitingToStart && m_bgTrackLength/2 <= m_loopPlaytime)
        {
            // wait for intro song to be done before we transition to Playing state
            m_loopPlaytime = m_bgTrackLength;
            m_waitingToStart = false;
            m_gameState = PLAYING;
        }
    }
    else if (m_gameState == PLAYING)
    {
        m_goScreenTime += dt;
        AutoZoom();
    }
    else if (m_gameState == P1_WINS ||
             m_gameState == P2_WINS ||
             m_gameState == P3_WINS ||
             m_gameState == P4_WINS)
    {
        m_winScreenTime += dt;
        if (m_winScreenTime >= 3.0f && m_loopPlaytime > m_bgTrackLength)
        {
            ResetGame();
        }
    }
    else if (m_gameState == CREDITS)
    {
        m_creditScreenTime += dt;
    }

}

void ObjectManager::Draw()
{
    // Render state
#if ! defined(CINDER_GLES)
    m_wireframe ? gl::enableWireframe() : gl::disableWireframe();
#endif

    m_fbo.bindFramebuffer();
    ci::gl::enableVerticalSync();   // TODO: why can't this be set once at setup?
    ci::gl::enableAlphaBlending();
    ci::gl::enableDepthRead();
    ci::gl::enable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    ci::gl::clear(ci::ColorA(0.7f,0.7f,0.7f,1.0f));

    // Draw assets
    gl::setMatrices(m_camera);

    if (m_showStars)
    {
        for (auto i = m_stars.begin(); i != m_stars.end(); i++)
        {
            (*i)->Draw();
        }
    }

    if (m_grid)
    {
        float zoomRatio = (m_curZoom - m_minZoom) / (m_maxZoom - m_minZoom);    // from 0 to 1
        float alpha = 0;
        float clamp = 0.2;

        if (zoomRatio < clamp)
        {
            alpha = 0;
        }
        else
        {
            alpha = (zoomRatio - clamp) * (1/(1-clamp));
        }

        for (auto i = m_gridObjects.begin(); i != m_gridObjects.end(); i++)
        {
            (*i)->m_pRenderData->m_color.a = alpha;
            (*i)->Draw();
        }
    }

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Draw();
    }

    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        (*i)->Draw();
    }

    m_fbo.unbindFramebuffer();
    ci::gl::enableVerticalSync();   // TODO: why can't this be set once at setup?
    ci::gl::enableAlphaBlending();
    ci::gl::enableDepthRead(false);
    ci::gl::enableDepthWrite(false);
    ci::gl::disable(GL_CULL_FACE);

    ci::gl::clear(ci::ColorA(0.7f,0.7f,0.7f,1.0f));

    // Post process shader on FBO
    gl::color(1.f, 1.f, 1.f, 1.f);
    gl::setMatricesWindow(getWindowSize(), false);
    ci::gl::Texture fboTex = m_fbo.getTexture();

    fboTex.enableAndBind();
    m_shader.bind();
    m_shader.uniform("tex0", 0);
    m_shader.uniform("resolution", Vec2f(fboTex.getWidth(), fboTex.getHeight()));
    m_shader.uniform("timer", static_cast<float>(m_loopPlaytime));
    m_shader.uniform("resolutionScale", getWindow()->getContentScale());

    gl::drawSolidRect(getWindowBounds());

    int playerCount = 1;
    for (auto i = m_players.begin(); i != m_players.end(); i++)
    {
        switch (playerCount)
        {
            case 1:
                m_shader.uniform("shockPos1", (*i)->m_deadPosition);
                if ((*i)->m_deadTimecount > 10) // stop effect after 10 seconds
                {
                    m_shader.uniform("shockTime1", 0);
                }
                else
                {
                    m_shader.uniform("shockTime1", (*i)->m_deadTimecount);
                }
                break;
            case 2:
                m_shader.uniform("shockPos2", (*i)->m_deadPosition);
                if ((*i)->m_deadTimecount > 10) // stop effect after 10 seconds
                {
                    m_shader.uniform("shockTime2", 0);
                }
                else
                {
                    m_shader.uniform("shockTime2", (*i)->m_deadTimecount);
                }
                break;
            case 3:
                m_shader.uniform("shockPos3", (*i)->m_deadPosition);
                if ((*i)->m_deadTimecount > 10) // stop effect after 10 seconds
                {
                    m_shader.uniform("shockTime3", 0);
                }
                else
                {
                    m_shader.uniform("shockTime3", (*i)->m_deadTimecount);
                }
                break;
            case 4:
                m_shader.uniform("shockPos4", (*i)->m_deadPosition);
                if ((*i)->m_deadTimecount > 10) // stop effect after 10 seconds
                {
                    m_shader.uniform("shockTime4", 0);
                }
                else
                {
                    m_shader.uniform("shockTime4", (*i)->m_deadTimecount);
                }
                break;
            default:
                break;
        }
        playerCount++;
    }

    switch (playerCount)
    {
        case 1:
            m_shader.uniform("shockPos1", Vec2f(0,0));
            m_shader.uniform("shockTime1", 0);
            m_shader.uniform("shockPos2", Vec2f(0,0));
            m_shader.uniform("shockTime2", 0);
            m_shader.uniform("shockPos3", Vec2f(0,0));
            m_shader.uniform("shockTime3", 0);
            m_shader.uniform("shockPos4", Vec2f(0,0));
            m_shader.uniform("shockTime4", 0);
            break;
        case 2:
            m_shader.uniform("shockPos2", Vec2f(0,0));
            m_shader.uniform("shockTime2", 0);
            m_shader.uniform("shockPos3", Vec2f(0,0));
            m_shader.uniform("shockTime3", 0);
            m_shader.uniform("shockPos4", Vec2f(0,0));
            m_shader.uniform("shockTime4", 0);
            break;
        case 3:
            m_shader.uniform("shockPos3", Vec2f(0,0));
            m_shader.uniform("shockTime3", 0);
            m_shader.uniform("shockPos4", Vec2f(0,0));
            m_shader.uniform("shockTime4", 0);
            break;
        case 4:
            m_shader.uniform("shockPos4", Vec2f(0,0));
            m_shader.uniform("shockTime4", 0);
            break;
        default:
            break;
    }
    playerCount++;

    m_shader.unbind();
    fboTex.unbind();
    fboTex.disable();

    // Draw game screens
    gl::setMatricesWindow(getWindowSize());
    ci::Area bounds = getWindowBounds();
    int dim = bounds.getHeight() < bounds.getWidth() ? bounds.getHeight() : bounds.getWidth();
    float scale = 0.8f;
    float texRatio = 0.25;  // height / width
    dim *= scale;
    if (m_gameState == TITLE || m_gameState == CREDITS)
    {
        dim *= 2.5;
    }
    if (m_gameState == CREDITS)
    {
        texRatio = 0.5;
    }
    if ((m_gameState == P1_WINS || m_gameState == P2_WINS) || (m_gameState == P3_WINS || m_gameState == P4_WINS))
    {
        texRatio = 1;
    }
    int xoffset = (bounds.getWidth() - dim) / 2;
    int yoffset = (bounds.getHeight() - (dim*texRatio)) / 2;
    bounds.set(xoffset, yoffset, bounds.getWidth() - xoffset, bounds.getHeight() - yoffset);

    switch (m_gameState)
    {
        case TITLE:
            gl::draw(m_texTitle, bounds);
            break;
        case READY:
            gl::draw(m_texReady, bounds);
            break;
        case PLAYING:
            if (m_goScreenTime < 1.0f)
            {
                gl::color(1.f,1.f,1.f,1.f-(m_goScreenTime/1.0f));
                gl::draw(m_texGo, bounds);
            }
            break;
        case P1_WINS:
            gl::draw(m_texP1Wins, bounds);
            DrawScore();
            break;
        case P2_WINS:
            gl::draw(m_texP2Wins, bounds);
            DrawScore();
            break;
        case P3_WINS:
            gl::draw(m_texP3Wins, bounds);
            DrawScore();
            break;
        case P4_WINS:
            gl::draw(m_texP4Wins, bounds);
            DrawScore();
            break;
        case CREDITS:
            gl::draw(m_texCredits, bounds);
            break;
        default:
            break;
    }

#if ! defined(CINDER_GLES)
    gl::disableWireframe();
#endif

    // Draw statistics
    if (m_displayStats)
    {
        g_pStats->display();
    }

    // Draw gui
    if (m_displayGui)
    {
        g_pGui->draw();
    }
}
