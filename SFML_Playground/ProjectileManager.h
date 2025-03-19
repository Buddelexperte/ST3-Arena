#pragma once
#include <unordered_set>

#include "GameInstance.h"
#include "ProjectilePool.h"
#include "Renderer.h"

class ProjectileManager : public sf::Drawable
{
private:
	GI_Arena& gameInstance = GI_Arena::getInstance();

	static int projectileID;

	ProjectilePool projectilePool;
	EntityRenderer projectileRenderer; // Manages draw calls
	std::unordered_map<size_t, std::unique_ptr<Projectile>> activeProjectiles; // Random Access to Enemies
	std::unordered_set<size_t> pendingKill;

	// SINGLETON
	ProjectileManager();
	ProjectileManager(const ProjectileManager&) = delete;
	ProjectileManager& operator=(const ProjectileManager&) = delete;

	void tick_kill(const float&);
	void tick_projectiles(const float&);

	void deleteProjectile(const size_t&);

public:
	static ProjectileManager& getInstance()
	{
		static ProjectileManager instance;
		return instance;
	}

	int getNumActiveProjectiles() const;
	void callDelete(const size_t&);
	void callUpdate(const size_t&, const InfoType&);

	void createProjectile(const IMovable::RenderInfo& renderInfo, const float& damage);

	void tick(const float& deltaTime);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
};