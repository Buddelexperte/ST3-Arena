#pragma once
#include "Player.h" // Header File
#include "GameInstance.h"
#include "Functions.h"

#include "AllWeapons.h"
#include "Enemy.h"


Player::Player()
	: 
	Entity(EntityType::Player),
	inventory(this), // Player inventory
	hud(nullptr), // Player HUD (no owner)
	invincibility(0.5f), // 0.5 seconds of invincibility after hit
	collisionBox(this, getPosition(), HITBOX_SIZE) // Collision box centered on player, hals as big as sprite
{
	std::cout << "### Creating player controller + pawn" << std::endl;
	// Player Sprite Initialization
	setPosition(viewCenter);
	setColor(sf::Color::White);

	enableCollision();
	std::cout << "- Player collision configured" << std::endl;

	// Sprite & textures
	for (int i = 4; i <= 6; i++)
	{
		sf::Texture newTexture;
		std::string texturePath = "Content/Textures/player/frames/player" + std::to_string(i) + ".png";
		if (!newTexture.loadFromFile(texturePath)) {
			std::cerr << "Failed to load texture: " << texturePath << std::endl;
			continue;
		}
		playerTextures.push_back(newTexture);
	}
	std::cout << "- Player textures loaded" << std::endl;

	if (!playerTextures.empty())
	{
		const sf::Vector2f spriteSize = { 100.0f, 100.0f };

		playerSprite.setTexture(playerTextures[0], true);
		animationSpeed = 0.2f;

		setSize(spriteSize);
		std::cout << "- Player textures applied" << std::endl;
	}
	else
	{
		std::cerr << "Texture not set. Cannot adjust size.\n";
	}

	shapes = { &flashlight, &playerSprite };

	std::cout << "Player created\n" << std::endl;
}

void Player::spawn()
{
	const sf::Vector2f spawnPos = sf::Vector2( 0.0f, 0.0f );
	setPosition(spawnPos);
	setVelocity(sf::Vector2f(0.0f, 0.0f));

	flashlight.setMaskMode(Flashlight::Type::CIRCLE);
	inventory.reset();
	resetHealth(); // 100% hp
	invincibility.setValue(2.0f); // 2 seconds invincibility (Spawn protection)
}

void Player::spawn(SpawnInformation spawnInfo)
{
	spawn();
}

void Player::tick(const float& deltaTime)
{
	// Gameplay ticks
	tick_gameplay(deltaTime);

	// Flashlight tick
	tick_flashlight(deltaTime);

	// Tick inventory
	tick_inv(deltaTime);
}

void Player::tick_flashlight(const float& deltaTime)
{
	static constexpr bool bDrawFlashlight = true;

	if (bDrawFlashlight)
	{
		flashlight.tick(deltaTime);
	}
}

void Player::tick_inv(const float& deltaTime)
{
	inventory.tick(deltaTime);
}

void Player::tick_gameplay(const float& deltaTime)
{
	// Checking for "holding" down a key
	idleInputs();

	bool bIsPaused = gameInstance().getIsPaused();

	if (bIsPaused)
		return;

	// Player gameplay ticks
	tick_move(deltaTime); // Movement inputs
	tick_animation(deltaTime); // Animation update
	tick_health(deltaTime);


	// Component gameplay ticks
	invincibility.addValue(-deltaTime); // Invinvibility update
	inventory.getActiveWeapon()->tick(deltaTime); // Weapon cooldown update
}

void Player::tick_move(const float& deltaTime)
{
	// Movement
	static constexpr float WALKING_SPEED = 350.0f;
	static constexpr float ROT_LERP_MULTIPLIER = 2.0f;

	float x = 0.0f, y = 0.0f, multiplier = 1.0f;

	if (sf::Keyboard::isKeyPressed(KEY_LSHIFT))
		multiplier *= 1.7f;

	if (sf::Keyboard::isKeyPressed(KEY_W)) y -= WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_A)) x -= WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_S)) y += WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_D)) x += WALKING_SPEED;

	sf::Vector2f targetVelo = sf::Vector2f{ x, y } * multiplier;
	sf::Vector2f velocity = getVelocity();
	zeroPrecision(velocity);

	sf::Vector2f veloDiff = targetVelo - velocity;
	if (shouldZero(veloDiff))
	{
		velocity = targetVelo;
	}
	else
	{
		float factor = lerpFactor(deltaTime, ACC_LERP_FACTOR);
		velocity = lerp(velocity, targetVelo, factor);
	}

	sf::Vector2f deltaPos = velocity * deltaTime;

	addPosition(deltaPos);
	setVelocity(velocity);

	 // Rotation
	sf::Vector2f pos = getPosition();
	const sf::Vector2f mousePos = gameInstance().getMousePos();
	float rotation = getRotation(); // Current rotation (0 to 360)
	const float targetRot = getLookAtRot(pos, mousePos);

	float angleDiff = getShortestAngle(rotation, targetRot);

	if (shouldZero(angleDiff))
	{
		rotation = targetRot;
	}
	else
	{
		float factor = lerpFactor(deltaTime, ROTATION_LERP);
		rotation = lerp(rotation, rotation + angleDiff, factor);
	}

	rotation = fmod(rotation + 360.f, 360.f);
	setRotation(rotation);
}

void Player::tick_health(const float&)
{
	// Add death code
}

void Player::tick_animation(const float& deltaTime)
{
	if (!playerTextures.empty())
	{
		animationAccu += deltaTime;
		if (animationAccu >= animationSpeed)
		{
			currentFrame = (static_cast<size_t>(currentFrame + 1)) % playerTextures.size();
			playerSprite.setTexture(playerTextures[currentFrame], true);
			animationAccu -= animationSpeed;
		}
	}
}

void Player::releaseToPool()
{
	GenericPool<Player>::instance().release(std::unique_ptr<Player>(this));
}

bool Player::handleEvent(sf::Event* eventRef)
{
	// If gameplay is paused, use gameInstance to relay event to activeWidget
	GameState gameState = gameInstance().getGameState();

	switch (gameState)
	{
	case IN_GAME:
		break;
	default:
		return gameInstance().handleEvent(eventRef);
	}

	// if not, just continue with normal distributing of events
	return IHasInput::handleEvent(eventRef);
}

sf::Keyboard::Key Player::onKeyPressed(sf::Event* eventRef)
{
	const sf::Keyboard::Key inputKey = eventRef->key.code;

	switch (inputKey)
	{
	case KEY_ESC:
	case KEY_TAB:
		gameInstance().handleEvent(eventRef);
		break;
	default:
		break;
	}

	return inputKey;
}

bool Player::onMouseClickL(sf::Event* eventRef)
{
	UseResult result = UseResult::FAILURE;

	if (inventory.getActiveWeapon() != nullptr)
	{
		result = inventory.shootWeapon();
	}
	return (result >= UseResult::SUCCESS);
}

void Player::onMouseDownL()
{
	if (gameInstance().getIsPaused())
		return;

	inventory.loadUpWeapon();
}

bool Player::onMouseClickR(sf::Event* eventRef)
{
	flashlight.toggleMaskMode();
	return true;
}

bool Player::onMouseReleaseL(sf::Event* eventRef)
{
	return (inventory.loadUpWeapon_cancel() == UseResult::SUCCESS);
}

float Player::onMouseScrolled(sf::Event* eventRef)
{
	return eventRef->mouseWheelScroll.delta;
}

// Setter and Getter methods

void Player::setRenderInfo(const RenderInfo& newRenderInfo)
{
	IMovable::setRenderInfo(newRenderInfo);

	playerSprite.setPosition(newRenderInfo.pos);
	playerSprite.setRotation(newRenderInfo.rot);

	collisionBox.setPos(newRenderInfo.pos);
	collisionBox.setSize(newRenderInfo.size);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->setRenderInfo(newRenderInfo);
}

void Player::setPosition(const sf::Vector2f& newPos)
{
	IMovable::setPosition(newPos);

	playerSprite.setPosition(newPos);
	collisionBox.setPos(newPos);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->setPosition(newPos);
}

void Player::addPosition(const sf::Vector2f& deltaPos, const bool& bVelocityBased)
{
	IMovable::addPosition(deltaPos, bVelocityBased);

	playerSprite.move(deltaPos);
	collisionBox.setPos(collisionBox.getPos() + deltaPos);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->addPosition(deltaPos, bVelocityBased);

}

void Player::setRotation(const float& newRot)
{
	IMovable::setRotation(newRot);

	playerSprite.setRotation(newRot - 90.0f);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->setRotation(newRot);
}

void Player::setSize(const sf::Vector2f& newSize)
{
	if (!playerSprite.getTexture()) return;
	IMovable::setSize(newSize);

	sf::Vector2u textureSize = playerSprite.getTexture()->getSize();
	sf::Vector2u originalSize(textureSize.x, textureSize.y);

	// * 2.0f because sprite isn't perfectly aligned
	playerSprite.setOrigin(originalSize.x / 2.0f, originalSize.y / 2.0f);
	playerSprite.setScale((newSize.x * 2.0f) / originalSize.x, (newSize.y * 2.0f) / originalSize.y);
	
	collisionBox.setSize(HITBOX_SIZE);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->setSize(newSize);
}

void Player::setColor(const sf::Color& newColor)
{
	IMovable::setColor(sf::Color::White);

	playerSprite.setColor(sf::Color::White);
}

void Player::onCollision(IHasCollision* other)
{
	other->collideWithPlayer(*this);
}

void Player::collideWithEnemy(Enemy& enemy)
{
	hurt(enemy.getDamage());

	// Trigger perks
	PerkTriggerInfo triggerInfo(PerkTrigger::OnEnemyContact, getPosition(), &enemy);

	inventory.triggerPerks(triggerInfo);
}

void Player::collideWithProjectile(Projectile& projectile)
{
	// Empty for now (probably for long)
}

void Player::hurt(const float& delta)
{
	if (invincibility.isEmpty())
	{
		const float actualDelta = (delta + inventory.getHurtBias()) * inventory.getHurtMultiplier();

		IHasHealth::hurt(actualDelta);
		invincibility.fill_to_max();

		// Trigger related Perks
		PerkTriggerInfo triggerInfo(PerkTrigger::OnPlayerDamaged, getPosition());
		inventory.triggerPerks(triggerInfo);
	}
}

void Player::heal(const float& delta)
{
	IHasHealth::hurt(delta);
	PerkTriggerInfo triggerInfo(PerkTrigger::OnPlayerHeal, getPosition());
	inventory.triggerPerks(triggerInfo);
}