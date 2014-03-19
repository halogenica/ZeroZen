#include "Bullet.h"
#include "ObjectData.h"
#include "RenderData.h"
#include "PhysicsData.h"
#include "SpriteSheet.h"
#include "LineParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

Bullet::Bullet(RenderData*          pRenderData,   // pRenderData can be NULL
               PhysicsData*         pPhysicsData,  // pPhysicsData can be NULL
               const ci::Vec3f&     pos,
               const ci::Quatf&     rot,
               const ci::Vec3f&     scale) : 
                   ObjectData(pRenderData, pPhysicsData, pos, rot, scale)
{
    m_spawned = false;
    m_spawnedJustToggledOff = false;
}

Bullet::~Bullet()
{
    for (auto i = m_particles.begin(); i != m_particles.end(); i++)
    {
        delete(*i);
    }
}

void Bullet::Draw()
{
    if (m_spawned)
    {
        ObjectData::Draw();

        for (auto i = m_particles.begin(); i != m_particles.end(); i++)
        {
            (*i)->Draw();
        }
    }
}

void Bullet::Update(float dt)
{
    if (m_spawned)
    {
        ObjectData::Update(dt);
        
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
        
        if (rand()%100 <= 3)
        {
            Vec3f startPos = Vec3f(0, (float)(rand()%5+5)/25*1, 0);
            Vec3f endPos = startPos + Vec3f(0, (float)(rand()%5+5)/25*1, 0);
            float randRot = (float)(rand()%10000-5000)/5000*PI/8;
            startPos.rotateZ(m_rot.getRoll() + PI + randRot);
            startPos += m_pos;
            endPos.rotateZ(m_rot.getRoll() + PI + randRot);
            endPos += m_pos;
            
            m_particles.push_back(new LineParticle(startPos, endPos));
        }
    }
    else if (m_spawnedJustToggledOff)
    {
        ObjectData::Update(dt);
        m_spawnedJustToggledOff = false;
        SetPos(Vec3f(0.f,0.f,-100.f));  // total hack, just move them outside the game world
    }
}

void Bullet::Collision(ObjectData* pCollider)
{
    m_spawned = false;

    if (pCollider->m_pPhysicsData->GetCollisionGroup() == COL_PLAYER)
    {
#if PLAY_SOUND
        ci::audio::Output::play(ci::audio::load(ci::app::loadResource(RES_HIT1_SOUND)));
#endif
    }
    else if (pCollider->m_pPhysicsData->GetCollisionGroup() == COL_ENEMY)
    {
#if PLAY_SOUND
        ci::audio::Output::play(ci::audio::load(ci::app::loadResource(RES_HIT2_SOUND)));
#endif
    }

    m_spawnedJustToggledOff = true;

}
