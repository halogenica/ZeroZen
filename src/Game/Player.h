
#pragma once

#include "Common.h"
#include "ObjectData.h"
#include <vector>
#include <algorithm>

namespace gen
{
// protos
class SpriteSheet;
class Bullet;
class Particle;

class Player: public ObjectData
{
public:
    Player(RenderData*          pRenderData,
           PhysicsData*         pPhysicsData,
           const ci::Vec3f&     pos,
           const ci::Quatf&     rot,
           const ci::Vec3f&     scale);
    ~Player();
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Shoot();
    virtual void Fly();
    virtual void FlyOff();
    virtual void QueueFly();
    virtual void Reset();
    virtual void Collision(ObjectData* pCollider);

    RenderData*     m_pRenderDataSilhouette;
    std::vector<Particle*> m_particles; // collection of particles emitted by the ship
    std::vector<Bullet*> m_bullets;     // collection of bullets
    UINT            m_numBurstBullets;  // the number of bullets in a burst
    bool            m_alive;
    bool            m_flying;
    float           m_flyTime;
    UINT            m_scoreWins;
    ci::Vec2f       m_deadPosition;
    float           m_deadTimecount;

private:
    ci::Vec3f       m_startingPos;
    ci::Quatf       m_startingRot;
    float           m_spinRate;
    float           m_spinPos;
    bool            m_queueFly;
};
}
