#include "Player.h"
#include "ObjectData.h"
#include "RenderData.h"
#include "PhysicsData.h"
#include "SpriteSheet.h"
#include "Bullet.h"
#include "ObjectManager.h"
#include "App.h"
#include "LineParticle.h"
#include "SquareParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

Player::Player(RenderData*          pRenderData,   // pRenderData can be NULL
               PhysicsData*         pPhysicsData,  // pPhysicsData can be NULL
               const ci::Vec3f&     pos,
               const ci::Quatf&     rot,
               const ci::Vec3f&     scale) : 
                   ObjectData(pRenderData, pPhysicsData, pos, rot, scale), 
                   m_startingPos(m_pos), 
                   m_startingRot(m_rot),
                   m_alive(true),
                   m_flying(false),
                   m_flyTime(0),
                   m_queueFly(false),
                   m_scoreWins(0),
                   m_deadPosition(0,0),
                   m_deadTimecount(0),
                   m_spinRate(0),
                   m_spinPos(0)
{
#ifdef _DEBUG
    m_numBurstBullets = 5;
    UINT totalBullets = m_numBurstBullets*3;    // debug mode with bullet physics is very slow
#else
    m_numBurstBullets = 10;
    UINT totalBullets = m_numBurstBullets*10;
#endif

    m_pRenderDataSilhouette = new RenderData(ci::app::loadResource(RES_SHIP_OBJ), ci::ColorAf(0,0,1,0));
    m_pRenderDataSilhouette->m_cellshadePercent = 0.4f;

    for (UINT i = 0; i < totalBullets; i++)
    {
        Bullet* pBullet = new Bullet(new RenderData(ci::app::loadResource(RES_SPHERE_OBJ), ci::ColorAf(1.0f, 0.0f, 0.0f, 1.0f)), 
                                     new PhysicsData(new btSphereShape(0.1f), 100, COL_BULLET, COL_ENEMY | COL_PLAYER), 
                                     Vec3f(0,0,-100), Quatf(0,0,0), Vec3f(0.2f,0.2f,0.2f));
        pBullet->m_spawned = false;
        m_bullets.push_back(pBullet);
        pBullet->m_pRenderData->m_cellshadePercent = 0.2;

//        pBullet->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));     // cannot move in z-axis
//        pBullet->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));    // can only rotate along z-axis
    }
}

Player::~Player()
{
    delete m_pRenderDataSilhouette;

    for (auto i = m_particles.begin(); i != m_particles.end(); i++)
    {
        delete(*i);
    }

    for (auto i = m_bullets.begin(); i != m_bullets.end(); i++)
    {
        delete(*i);
    }
}

void Player::Draw()
{
    ObjectData::Draw();

    m_pRenderDataSilhouette->Draw(m_pos, Quatf(Vec3f(0,1,0), m_spinPos)*m_rot, m_scale*1.25);

    for (auto i = m_particles.begin(); i != m_particles.end(); i++)
    {
        (*i)->Draw();
    }

    for (auto i = m_bullets.begin(); i != m_bullets.end(); i++)
    {
        (*i)->Draw();
    }
}

void Player::Update(float dt)
{
    ObjectData::Update(dt);

    if (g_pObjectManager->m_gameState == PLAYING && m_queueFly)
    {
        m_queueFly = false;
        Fly();
    }

    for (auto i = m_particles.begin(); i != m_particles.end();)
    {
        (*i)->Update(dt);

        if(!(*i)->m_alive)
        {      
            delete(*i);
            i = m_particles.erase(i);
        }
        else
        {
            i++;
        }
    }

    for (auto i = m_bullets.begin(); i != m_bullets.end(); i++)
    {
        (*i)->Update(dt);
    }

    if (m_flying)
    {
        if (rand()%100 <= std::max(20 - m_flyTime*28, 5.f))
        {
            Vec3f startPos = Vec3f(0, (float)(rand()%20+5)/25*1, 0);
            Vec3f endPos = startPos + Vec3f(0, (float)(rand()%10+15)/25*1, 0);
            float randRot = (float)(rand()%10000-5000)/5000*PI/7;
            startPos.rotateZ(m_rot.getRoll() + PI + randRot);
            startPos += m_pos;
            endPos.rotateZ(m_rot.getRoll() + PI + randRot);
            endPos += m_pos;

            m_particles.push_back(new LineParticle(startPos, endPos));
        }
/*
        float quant = m_pRenderData->m_pSpriteSheet->m_numTilesWide * m_pRenderData->m_secondsPerSprite;
        if ((m_flyStage1Time < quant) &&
            (m_flyStage1Time+dt > quant))
        {
            m_pRenderData->m_spriteRange.x += m_pRenderData->m_pSpriteSheet->m_numTilesWide;    // the next row in the sprite sheet has the fly stage 2 data
            m_pRenderData->m_spriteRange.y += m_pRenderData->m_pSpriteSheet->m_numTilesWide;    // the next row in the sprite sheet has the fly stage 2 data
        }
*/
        m_flyTime += dt;

        if (m_spinRate < 0.04)
        {
            m_spinRate += 0.00003;
        }
        m_spinPos += m_spinRate;

        while (m_spinPos > 2*PI)
        {
            m_spinPos -= 2*PI;
        }
    }
    else
    {

        if (m_spinRate > 0.01)
        {
            m_spinRate -= 0.00003;
            m_spinPos += m_spinRate;
        }
        else
        {
            while (m_spinPos > 2*PI)
            {
                m_spinPos -= 2*PI;
            }

            if (m_spinPos + m_spinRate < 2*PI)
            {
                m_spinPos += m_spinRate;
            }
            else
            {
                m_spinRate = 0;
                m_spinPos = 0;
            }
        }
    }

    if (!m_alive)
    {
        m_deadTimecount += dt;
    }
}

void Player::Shoot()
{
    // Play sound
#if PLAY_SOUND
    ci::audio::Output::play(ci::audio::load(ci::app::loadResource(RES_EXPLOSION2_SOUND)));
#endif
    // Get orientation of player
    Vec3f orient = Vec3f(0,1,0);
    if (m_rot.getAngle() != 0)
    {
        orient.rotate(m_rot.getAxis(), m_rot.getAngle());
    }

    // Fire a burst of bullets
    UINT bulletsFired = 0;
    for (auto itBullet = m_bullets.begin(); itBullet != m_bullets.end() && bulletsFired < m_numBurstBullets; itBullet++)
    {
        if (!(*itBullet)->m_spawned)
        {
            /*
            Vec3f bulletPos = orient;
            bulletPos.x += ((float)(rand()%20000-10000))/100000;
            bulletPos.y += ((float)(rand()%20000-10000))/100000;
            */
            Vec3f bulletPos = orient * 0.7f;

            (*itBullet)->SetPos(m_pos + bulletPos);
            (*itBullet)->m_spawned = true;
            bulletsFired++;

            Vec3f randOrient = Vec3f(((float)(rand()%20000-10000))/50000, 1 + ((float)(rand()%20000-10000))/50000, 0);
            if (m_rot.getAngle() != 0)
            {
                randOrient.rotate(m_rot.getAxis(), m_rot.getAngle());
            }

            randOrient = randOrient * 10;
            (*itBullet)->SetRot(ci::Quatf(Vec3f(0,0,1),atan2(randOrient.y, randOrient.x) - PI/2));
            (*itBullet)->m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));     // cannot move in z-axis
            (*itBullet)->m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));    // can only rotate along z-axis
            (*itBullet)->m_pPhysicsData->m_pBtRigidBody->setLinearVelocity(btVector3(randOrient.x, randOrient.y, 0));
        }
    }

    // Apply backwards impulse
    orient.rotateZ(PI);
    btVector3 impulse = btVector3(orient.x,orient.y,orient.z);
    impulse = impulse * 10;
    m_pPhysicsData->m_pBtRigidBody->applyImpulse(impulse,btVector3(0,0,0));
}

void Player::Fly()
{
    m_flying = true;
    m_flyTime = 0;
#if PLAY_SOUND
    ci::audio::Output::play(ci::audio::load(ci::app::loadResource(RES_EXPLOSION1_SOUND)));
#endif
//    m_pRenderData->SetSpriteRange(Vec2i(m_startSpriteIndex,m_startSpriteIndex+7));
}

void Player::FlyOff()
{
    m_flying = false;
    m_flyTime = 0;
    m_queueFly = false;
//    m_pRenderData->SetSpriteRange(Vec2i(m_startSpriteIndex,m_startSpriteIndex));
}

void Player::QueueFly()
{
    m_queueFly = true;
}

void Player::Reset()
{
    if (m_flying)
    {
        m_queueFly = true;
    }

    SetPos(m_startingPos);
    SetRot(m_startingRot);
    m_pPhysicsData->m_pBtRigidBody->setLinearFactor(btVector3(1,1,0));       // cannot move in z-axis
    m_pPhysicsData->m_pBtRigidBody->setAngularFactor(btVector3(0,0,1));      // can only rotate along z-axis
    m_pPhysicsData->m_pBtRigidBody->setDamping(0.2, 0);                      // "air resistance"
    m_alive = true;
    m_flying = false;
    m_flyTime = 0;
    m_deadPosition = Vec2f(0,0);
    m_deadTimecount = 0;
}

void Player::Collision(ObjectData* pCollider)
{
    if (pCollider->m_pPhysicsData->GetCollisionGroup() == COL_BULLET)
    {
        // lose some health?
        btVector3 vel = pCollider->m_pPhysicsData->m_pBtRigidBody->getLinearVelocity();
        btVector3 impulse = vel.normalized() * 5;
        m_pPhysicsData->m_pBtRigidBody->applyImpulse(impulse,btVector3(0,0,0));
        if (g_pObjectManager->m_port.is_open())
        {
            boost::asio::write(g_pObjectManager->m_port, boost::asio::buffer("4"));
        }
    }
    else if (pCollider->m_pPhysicsData->GetCollisionGroup() == COL_ENEMY)
    {
        // dead!
#if PLAY_SOUND
        ci::audio::Output::play(ci::audio::load(ci::app::loadResource(RES_EXPLOSION3_SOUND)));
#endif
        m_deadPosition = g_pObjectManager->m_camera.worldToScreen(m_pos, g_pApp->getWindowWidth(), g_pApp->getWindowHeight());
        m_deadPosition.y = g_pApp->getWindowHeight() - m_deadPosition.y;    // inverse the Y component since the FBO is displayed "upside down"
        m_deadTimecount = 0;

        SetPos(m_startingPos + Vec3f(0,0,1000));    // total hack, place the player behind the camera
        m_alive = false;
        if (g_pObjectManager->m_port.is_open())
        {
            boost::asio::write(g_pObjectManager->m_port, boost::asio::buffer("9"));
        }
        g_pObjectManager->CheckWinners();
    }
}
