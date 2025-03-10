#pragma once
#include "Player.h" // Header File
#include "GameInstance.h"
#include "Functions.h"

#include "AllWeapons.h"

Player::Player(InputWidget* parent)
	: InputWidget(parent),
	flashlight(this),
	inventory(),
	// TODO: Implement Start Weapon loading
	collisionBox(this, getPosition(), getSize())
{
	// Player Sprite Initialization
	IMovable::setPosition(windowCenter);
	playerSprite.setPosition(windowCenter);
	collisionBox.setPos(windowCenter);

	CollisionManager::getInstance().registerPlayer(getCollision());

	IMovable::setColor(sf::Color::White);
	playerSprite.setColor(sf::Color::White);

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

	shapes = { &flashlight, &playerSprite };

	if (!playerTextures.empty())
	{
		const sf::Vector2f spriteSize = { 100.0f, 100.0f };

		playerSprite.setTexture(playerTextures[0], true);
		animationSpeed = 0.2f;

		setSize(spriteSize);
	}
	else
	{
		std::cerr << "Texture not set. Cannot adjust size.\n";
	}

}

Player::~Player()
{
	WidgetElement::~WidgetElement();
	CollisionManager::getInstance().unregisterPlayer();
}

Player* Player::spawn(const sf::Vector2f& spawnPos)
{
	inventory.clear();

	inventory.addWeapon(std::make_unique<Pistol>());

	setPosition(spawnPos);
	gameInstance->resetViewPos();

	return this;
}

void Player::tick_move(const float& deltaTime)
{
	// Movement

	constexpr float WALKING_SPEED = 350.0f;
	constexpr float LERP_SMOOTHNESS = 0.01f;

	float x = 0.0f, y = 0.0f, multiplier = 1.0f;

	if (sf::Keyboard::isKeyPressed(KEY_LSHIFT))
		multiplier *= 1.7f;

	if (sf::Keyboard::isKeyPressed(KEY_W)) y -= WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_A)) x -= WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_S)) y += WALKING_SPEED;
	if (sf::Keyboard::isKeyPressed(KEY_D)) x += WALKING_SPEED;

	sf::Vector2f targetVelo = sf::Vector2f{ x, y } * multiplier;
	sf::Vector2f currVelo = getVelocity();
	zeroPrecision(currVelo);

	if (!shouldZero(targetVelo - currVelo))
		currVelo = (lerp(currVelo, targetVelo, LERP_SMOOTHNESS));
	else
		currVelo = (targetVelo);

	direction = { currVelo.x / WALKING_SPEED, currVelo.y / WALKING_SPEED };
	sf::Vector2f offset = currVelo * deltaTime;

	addPosition(offset);
	setVelocity(currVelo);

	 // Rotation

	const sf::Vector2f playerPos = getPosition();
	const sf::Vector2f mousePos = gameInstance->getMousePos();
	float rotation = getRotation();
	const float targetRot = getLookAtRot(playerPos, mousePos);

	if (!shouldZero(rotation - targetRot))
	{
		const float ROT_LERP = LERP_SMOOTHNESS * 10.0f * multiplier;
		setRotation(lerp(rotation, targetRot, ROT_LERP));
	}
}

void Player::tick(const float& deltaTime)
{
	constexpr bool bDrawFlashlight = true;
	
	InputWidget::tick(deltaTime);
	if (!gameInstance->getIsPaused())
		tick_move(deltaTime);

	// Flashlight update
	if (bDrawFlashlight)
	{
		flashlight.tick(deltaTime);
		for (sf::Drawable* elem : shapes)
		{
			flashlight.drawOtherScene(elem);
		}
	}

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

bool Player::handleEvent(sf::Event* eventRef)
{
	// If gameplay is paused, use gameInstance to relay event to activeWidget
	if (gameInstance->getIsPaused())
		return gameInstance->handleEvent(eventRef);
	// if not, just continue with normal distributing of events
	return InputWidget::handleEvent(eventRef);
}

sf::Keyboard::Key Player::onKeyPressed(sf::Event* eventRef)
{
	const sf::Keyboard::Key inputKey = eventRef->key.code;
	if (inputKey == sf::Keyboard::Escape)
		gameInstance->handleEvent(eventRef);
	return inputKey;
}

bool Player::onMouseClickL(sf::Event* eventRef)
{
	return (inventory.getActiveWeapon()->activate(ItemUse::ATTACK) >= UseResult::SUCCESS);
}

bool Player::onMouseClickR(sf::Event* eventRef)
{
	flashlight.toggleMaskMode();
	return true;
}

bool Player::onMouseReleaseL(sf::Event* eventRef)
{
	return (inventory.getActiveWeapon()->activate(ItemUse::CANCEL_LOAD) == UseResult::SUCCESS);
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

void Player::addPosition(const sf::Vector2f& deltaPos)
{
	IMovable::addPosition(deltaPos);
	playerSprite.move(deltaPos);
	collisionBox.setPos(collisionBox.getPos() + deltaPos);
	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->addPosition(deltaPos);

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

	const sf::Vector2u textureSize = playerSprite.getTexture()->getSize();
	const sf::Vector2f originalSize(textureSize.x, textureSize.y);

	// * 2.0f because sprite isn't perfectly aligned
	playerSprite.setOrigin(originalSize.x / 2.0f, originalSize.y / 2.0f);
	playerSprite.setScale((newSize.x * 2.0f) / originalSize.x, (newSize.y * 2.0f) / originalSize.y);
	
	collisionBox.setSize(newSize);

	if (inventory.getActiveWeapon())
		inventory.getActiveWeapon()->setSize(newSize);

}

void Player::onCollision(IHasCollision* other)
{
	other->collideWithPlayer(*this);
}

void Player::collideWithEnemy(Enemy& enemy)
{
	// If enemy, hurt
}

void Player::collideWithProjectile(Projectile& projectile)
{
	// idk what to put here now
}