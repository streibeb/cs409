//
//  World.cpp
//

#include <cassert>
#include <iostream>

#include "../../ObjLibrary/ObjModel.h"
#include "../../ObjLibrary/Vector3.h"

#include "PhysicsObjectId.h"
#include "PhysicsObject.h"
#include "ExplosionManagerInterface.h"
#include "ExplosionManager.h"
#include "WorldInterface.h"
#include "World.h"

using namespace std;



World :: World ()
		: mp_explosion_manager(new ExplosionManager())
{
	assert(invariant());
}

World :: World (const World& original)
		: mp_explosion_manager(original.mp_explosion_manager->getClone())
{
	assert(invariant());
}

World :: ~World ()
{
	delete mp_explosion_manager;
}

World& World :: operator= (const World& original)
{
	if(&original != this)
	{
		delete mp_explosion_manager;

		mp_explosion_manager = original.mp_explosion_manager->getClone();
	}

	assert(invariant());
	return *this;
}



bool World :: isInitialized () const
{
	assert(mp_explosion_manager != NULL);
	return mp_explosion_manager->isInitialized();
}

void World :: draw (const Vector3& camera_forward,
                              const Vector3& camera_up) const
{
	assert(isInitialized());
	assert(camera_forward.isNormal());
	assert(camera_up.isNormal());
	assert(camera_forward.isOrthogonal(camera_up));
    
    drawSkybox();
    
    // Draw saturn
    planet.draw();
    
    // Draw moons
    for (int i = 0; i < MOON_COUNT; i++)
    {
        moons[i].draw();
    }
    
    // draw player ship
    player_ship.draw();
    
    // Draw ring particles
    g_rings.draw(player_ship.getCameraCoordinateSystem());
    
    // Draw NPC ships
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].isAlive()) ships[i].draw();
    }
    // Draw bullets
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (bullets[i].isAlive()) bullets[i].draw();
    }
    
    // Draw ring
    glPushMatrix();
        glTranslatef(ringInfo.position.x, ringInfo.position.y, ringInfo.position.z);
        glScalef(ringInfo.radius, ringInfo.radius, ringInfo.radius);
        ring_list.draw();
    glPopMatrix();
    
	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->draw(camera_forward, camera_up);
}

void World::drawSkybox() const
{
    const Vector3& center_position = player_ship.getCameraCoordinateSystem().getPosition();
    
    glDepthMask(GL_FALSE);
    glPushMatrix();
        glTranslated(center_position.x, center_position.y, center_position.z);
        skybox.draw();
    glPopMatrix();
    glDepthMask(GL_TRUE);
}

void World :: init ()
{
	if(!isInitialized())
	{
        // Skybox Init
        ObjModel sb;
        sb.load("Models/Skybox.obj");
        skybox = sb.getDisplayList();
        
        // Planet Init
        ObjModel p;
        p.load(planetInfo.filename);
        DisplayList p_dl = p.getDisplayList();
        PhysicsObjectId p_id = PhysicsObjectId(PhysicsObjectId::TYPE_PLANETOID,
                                             PhysicsObjectId::FLEET_NATURE,
                                             0);
        planet.initPlanetoid(p_id, planetInfo.position, planetInfo.radius, p_dl, 1.f);
        
        // Moon Init
        for (int i = 0; i < MOON_COUNT; i++)
        {
            ObjModel m;
            m.load(moonInfo[i].filename);
            DisplayList m_dl = m.getDisplayList();
            PhysicsObjectId m_id = PhysicsObjectId(PhysicsObjectId::TYPE_PLANETOID,
                                                   PhysicsObjectId::FLEET_NATURE,
                                                   i + 1);
            moons[i].initPlanetoid(m_id, moonInfo[i].position, moonInfo[i].radius, m_dl, 1.f);
        }
        
        // Ring init
        RingParticle::load();
        g_rings.init(RING_HALF_THICKNESS,
                     RING_INNER_RADIUS,
                     RING_OUTER_RADIUS_BASE,
                     RING_DENSITY_MAX,
                     RING_DENSITY_FACTOR);
        
        for(unsigned int i = 0; i < MOON_COUNT; i++)
        {
            g_rings.addHole(moons[i].getPosition(),
                            moons[i].getRadius() + RING_MOON_PADDING);
        }
        ObjModel r;
        r.load("Models/Ring.obj");
        ring_list = r.getDisplayList();
        
        // Ship Init
        ObjModel s;
        s.load("Models/Grapple.obj");
        DisplayList s_dl = s.getDisplayList();
        for (int i = 0; i < SHIP_COUNT; i++)
        {
            PhysicsObjectId s_id = PhysicsObjectId(PhysicsObjectId::TYPE_SHIP,
                                                   PhysicsObjectId::FLEET_ENEMY,
                                                   i);
            
            int moonIndex = rand() % MOON_COUNT;
            Vector3 pos = moonInfo[moonIndex].position
                            + Vector3::getRandomUnitVector()
                            * (moonInfo[moonIndex].radius + 500.0);
            
            ships[i].initPhysics(s_id, pos, 100.f, Vector3::getRandomUnitVector(), s_dl, 1.f);
            ships[i].setHealth(1);
            ships[i].setAmmo(0);
            ships[i].setSpeed(0.f);
        }
        
        // Player Ship Init
        PhysicsObjectId ps_id = PhysicsObjectId(PhysicsObjectId::TYPE_SHIP,
                                                PhysicsObjectId::FLEET_PLAYER,
                                                0);
        Vector3 pos = moons[0].getPosition() + Vector3(0.f, 15000.f, 0.f);
        player_ship.initPhysics(ps_id, pos, 10.f, Vector3::getRandomUnitVector(), s_dl, 1.f);
        player_ship.setHealth(10);
        player_ship.setAmmo(8);
        player_ship.setSpeed(250.f);
        
        //Bullet init
        ObjModel b;
        b.load("Models/Bolt.obj");
        DisplayList b_dl = b.getDisplayList();
        for (int i = 0; i < BULLET_COUNT; i++)
        {
            bullets[i].initPhysics(PhysicsObjectId::TYPE_BULLET, {0, 0, 0}, 10.f, {0, 0, 0}, b_dl, 1.f);
        }

		assert(mp_explosion_manager != NULL);
		mp_explosion_manager->init(EXPLOSION_FILENAME.c_str(), 15);
	}

	assert(invariant());
}

void World :: reset ()
{
	assert(isInitialized());

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->removeAll();

	assert(invariant());
}

void World :: updateAll ()
{
	assert(isInitialized());
    
    player_ship.update(*this);
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        ships[i].update(*this);
    }
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        bullets[i].update(*this);
    }
    handleCollisions();

	assert(mp_explosion_manager != NULL);
	mp_explosion_manager->update();

	assert(invariant());
}

///////////////////////////////////////////////////////////////
//
//  Helper function not inherited from anywhere
//

bool World :: invariant () const
{
	if(mp_explosion_manager == NULL) return false;
	return true;
}

void World::handleCollisions()
{
    handleShipCollisions(player_ship);
    
    for (int i = 0; i < SHIP_COUNT; i++)
    {
        if (ships[i].isDying() || !ships[i].isAlive()) continue;
        handleShipCollisions(ships[i]);
    }
    
    for (int i = 0; i < BULLET_COUNT; i++)
    {
        if (!bullets[i].isAlive()) continue;
        handleBulletCollisions(bullets[i]);
    }
}

void World::handleShipCollisions(Ship& ship)
{
    // Ring Particles
    if (g_rings.handleRingParticleCollision(ship.getPosition(), ship.getRadius()))
    {
        resolvePlanetoidCollision(ship);
    }
    
    // Planetoids
    if (GeometricCollisions::sphereVsSphere(ship.getPosition(),
                                            ship.getRadius(),
                                            planet.getPosition(),
                                            planet.getRadius()))
    {
        resolvePlanetoidCollision(ship);
    }
    
    for (int j = 0; j < MOON_COUNT; j++)
    {
        if (GeometricCollisions::sphereVsSphere(ship.getPosition(),
                                                ship.getRadius(),
                                                moons[j].getPosition(),
                                                moons[j].getRadius()))
        {
            resolvePlanetoidCollision(ship);
        }
    }
    
    // Ships
    for (int j = 0; j < SHIP_COUNT; j++)
    {
        if (ship.getId() >= ships[j].getId()) continue;
        if (ships[j].isDying() || !ships[j].isAlive()) continue;
        if (GeometricCollisions::sphereVsSphere(ship.getPosition(),
                                                ship.getRadius(),
                                                ships[j].getPosition(),
                                                ships[j].getRadius()))
        {
            resolveShipCollision(ship, ships[j]);
        }
    }
}

void World::handleBulletCollisions(Bullet& bullet)
{
    // Ring Particles
    if (g_rings.handleRingParticleCollision(bullet.getPosition(), 0.0f))
    {
        resolvePlanetoidCollision(bullet);
    }
    
    // Planetoids
    if (GeometricCollisions::sphereVsSphere(bullet.getPosition(),
                                            0.0f,
                                            planet.getPosition(),
                                            planet.getRadius()))
    {
        resolvePlanetoidCollision(bullet);
    }
    
    for (int j = 0; j < MOON_COUNT; j++)
    {
        if (GeometricCollisions::sphereVsSphere(bullet.getPosition(),
                                                0.0f,
                                                moons[j].getPosition(),
                                                moons[j].getRadius()))
        {
            resolvePlanetoidCollision(bullet);
        }
    }
    
    // Ships
    for (int j = 0; j < SHIP_COUNT; j++)
    {
        if (ships[j].isDying() || !ships[j].isAlive()) continue;
        if (GeometricCollisions::sphereVsSphere(bullet.getPosition(),
                                                0.0f,
                                                ships[j].getPosition(),
                                                ships[j].getRadius()))
        {
            resolveBulletCollision(bullet, ships[j]);
        }
    }
    
}

void World::resolvePlanetoidCollision(PhysicsObject& obj)
{
    obj.markDead(false);
}

void World::resolveShipCollision(Ship& obj1, Ship& obj2)
{
    if (obj1.getId() == obj2.getId()) return;
    
    obj1.markDead(false);
    obj2.markDead(false);
}

void World::resolveBulletCollision(Bullet& b, Ship& obj)
{
    b.markDead(false);
    obj.addHealth(-1.0f);
}
