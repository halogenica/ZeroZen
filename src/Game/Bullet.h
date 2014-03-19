
#pragma once

#include "Common.h"
#include "ObjectData.h"
#include <vector>

namespace gen
{
// protos
class SpriteSheet;
class Particle;

class Bullet : public ObjectData
{
public:
    Bullet(RenderData*          pRenderData,
           PhysicsData*         pPhysicsData,
           const ci::Vec3f&     pos,
           const ci::Quatf&     rot,
           const ci::Vec3f&     scale);
    virtual ~Bullet();
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Collision(ObjectData* pCollider);

    std::vector<Particle*> m_particles; // collection of particles emitted by the ship
    bool            m_spawned;

private:
    bool            m_spawnedJustToggledOff;
};
}
