#pragma once
#include "BaseClasses.h" // Header File
#include "GameInstance.h"
#include "Functions.h"

Player::Player(InputWidget* parent)
	: InputWidget(parent),
	collisionBox(renderInfo.pos, renderInfo.size)
{

	// Player Sprite Initialization
	renderInfo.pos = windowCenter;
	playerSprite.setPosition(renderInfo.pos);
	collisionBox.setPos(renderInfo.pos);

	renderInfo.color = sf::Color::White;
	playerSprite.setColor(renderInfo.color);

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


	shapes = { &playerSprite };

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

void Player::calcMovement(const float& deltaTime)
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

	sf::Vector2f targetVelo = sf::Vector2f{ x, y } *multiplier;
	zeroPrecision(renderInfo.velocity);

	if (!shouldZero(targetVelo - renderInfo.velocity))
		renderInfo.velocity = lerp(renderInfo.velocity, targetVelo, LERP_SMOOTHNESS);
	else
		renderInfo.velocity = targetVelo;

	direction = { renderInfo.velocity.x / WALKING_SPEED, renderInfo.velocity.y / WALKING_SPEED };
	const sf::Vector2f delta = renderInfo.velocity * deltaTime;
	addPos(delta);

	 // Rotation

	const sf::Vector2f playerPos = getPos();
	const sf::Vector2f mousePos = gameInstance->getMousePos();
	const float targetRot = getLookAtRot(playerPos, mousePos);
	float rotation = getRot();

	zeroPrecision(rotation);
	if (rotation != targetRot)
	{
		const float ROT_LERP = LERP_SMOOTHNESS * 10.0f * multiplier;
		setRot(lerp(rotation, targetRot, ROT_LERP));
	}
}

void Player::tick(const float& deltaTime)
{
	InputWidget::tick(deltaTime);
	if (!gameInstance->getIsPaused()) 
		calcMovement(deltaTime);

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

sf::Keyboard::Key Player::keyboardInput(sf::Event* eventRef)
{
	const sf::Keyboard::Key inputKey = eventRef->key.code;
	if (inputKey == sf::Keyboard::Escape)
		gameInstance->handleEvent(eventRef);
	return inputKey;
}

sf::Mouse::Button Player::mouseInput(sf::Event* eventRef)
{
	const sf::Mouse::Button mouseInput = eventRef->mouseButton.button;
	if (mouseInput == sf::Mouse::Left || mouseInput == sf::Mouse::Right)
		gameInstance->handleEvent(eventRef);
	return mouseInput;
}

float Player::scrollInput(sf::Event* eventRef)
{
	return eventRef->mouseWheelScroll.delta;
}

// Setter and Getter methods

void Player::setPos(const sf::Vector2f& newPos)
{
	playerSprite.setPosition(newPos);
	renderInfo.pos = newPos;
	// Update the collision rectangle to reflect the new position
	collisionBox.setPos(newPos);
}

void Player::addPos(const sf::Vector2f& delta)
{
	playerSprite.move(delta);
	renderInfo.pos += delta;
	// Update the collision rectangle to reflect the movement
	collisionBox.setPos(collisionBox.getPos() + delta);
}

sf::Vector2f Player::getPos() const
{
	return playerSprite.getPosition();
}

void Player::setRot(const float& newRot)
{
	playerSprite.setRotation(newRot - 90.0f);
}

float Player::getRot() const
{
	return playerSprite.getRotation() + 90.0f;
}

void Player::setSize(const sf::Vector2f& newSize)
{
	if (!playerSprite.getTexture()) return;

	const sf::Vector2u textureSize = playerSprite.getTexture()->getSize();
	const sf::Vector2f originalSize(textureSize.x, textureSize.y);

	// * 2.0f because sprite isn't perfectly aligned
	playerSprite.setOrigin(originalSize.x / 2.0f, originalSize.y / 2.0f);
	playerSprite.setScale((newSize.x * 2.0f) / originalSize.x, (newSize.y * 2.0f) / originalSize.y);
	
	collisionBox.setSize(newSize);
}

void Player::tick_collision(const float& deltaTime)
{
	return;
}

bool Player::isColliding(const sf::FloatRect& otherBox) const
{
	return collisionBox.isColliding(otherBox);
}

bool Player::isColliding(const sf::Vector2f& otherPos) const
{
	return collisionBox.isColliding(otherPos);
}

void Player::onCollision(ICollidable* other)
{
	return;
}