#pragma once
#include "Flashlight.h"
#include "GameInstance.h"


Flashlight::Flashlight()
    : WidgetElement(nullptr),
    animWait(ANIM_FRAME_WAIT)
{
    static constexpr float SPRITE_SIZE = 512.0f; // Actual sprite image dimensions
    flashlightSprite.setOrigin(SPRITE_SIZE / 2.0f, SPRITE_SIZE / 2.0f); // Center the sprite's origin

    sf::Color spriteTint = flashlightSprite.getColor();
    spriteTint.a = static_cast<sf::Uint8>(spriteTint.a * 0.75f);
    flashlightSprite.setColor(spriteTint);

    if (!circleShader.loadFromMemory(circleShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load flashlight Circle shader.");
    }
    if (!coneShader.loadFromMemory(coneShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load flashlight Cone shader.");
    }
    if (!testShader.loadFromMemory(testShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load test shader.");
    }

    static const std::string texturePath = "Content/Textures/FlashlightCircle/";
    for (int i = 52; i <= 60; i++)
    {
        std::string fileName = "512x512 textures (" + std::to_string(i) + ").png";
        sf::Texture newTexture;
        if (!newTexture.loadFromFile(texturePath + fileName))
        {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            continue;
        }
        textures.push_back(newTexture);
    }

    if (textures.empty())
    {
        throw std::runtime_error("No flashlight textures were loaded!");
    }

    // Create the render texture with the window size
    if (!sceneRenderTexture.create(windowSize.x, windowSize.y))
    {
        throw std::runtime_error("Failed to create scene render texture.");
    }

    sceneSprite.setTexture(sceneRenderTexture.getTexture());

    shapes = { &sceneSprite };

    playAnim(EAnimation::IDLE_ANIM);
}

void Flashlight::tick(const float& deltaTime)
{
    IWidgetAnimation::tick_anim(deltaTime);

    tick_shader(deltaTime);

    tick_display(deltaTime);
}

void Flashlight::tick_shader(const float& deltaTime)
{
    currShader = getActiveShader();

    // If no shader active, cancel ticking calculation
    if (!currShader)
        return;

    // Update flashlight position and rotation
    sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();
    setPosition(playerPos);

    sf::Vector2f mousePos = gameInstance().getMousePos();
    float newRot = getLookAtRot(playerPos, mousePos);
    setRotation(newRot);


    // Transform player's world position to view-space coordinates for shader
    sf::Vector2f viewOffset = view->getCenter() - (view->getSize() / 2.0f);
    sf::Vector2f lightPos = playerPos - viewOffset;

    // Calculation for Cone specific mouse direction
    if (!gameInstance().getIsPaused() && bUseCone)
    {
        // Normalize direction vector
        sf::Vector2f mouseDir = mousePos - playerPos;
        if (mouseDir != sf::Vector2f(0.0f, 0.0f))
        {
            float len = std::sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);
            if (len > 0.0f)
            {
                mouseDir /= len;
            }
        }
        coneDir = mouseDir;
    }

    switch (flashlightShape)
    {
    case Flashlight::Type::CIRCLE:
        flashlightSprite.setScale(usedScale);
        circleShader.setUniform("lightPos", lightPos);
        circleShader.setUniform("radius", radius);
        circleShader.setUniform("u_viewSize", sf::Glsl::Vec2(viewSize));
        circleShader.setUniform("viewportHeight", view->getSize().y);
        break;
    case Flashlight::Type::CONE:
        flashlightSprite.setScale(usedScale * 2.0f);
        coneShader.setUniform("lightPos", lightPos);
        coneShader.setUniform("radius", radius * 2.0f);
        coneShader.setUniform("direction", coneDir);
        coneShader.setUniform("angle", degreesToRadians(30.0f)); // 60° cone (30° half-angle)
        coneShader.setUniform("u_viewSize", sf::Glsl::Vec2(viewSize));
        coneShader.setUniform("viewportHeight", view->getSize().y);
        break;
    default:
        break;
    }
}

void Flashlight::tick_display(const float& deltaTime)
{
    // Set the render texture's view to match the player's view
    sceneRenderTexture.setView(*view);
    // Ensure the render texture sprite is positioned properly
    sceneSprite.setPosition(view->getCenter() - (view->getSize() / 2.0f));
    // Clear with fully transparent background and render
    sceneRenderTexture.clear(sf::Color::Black);
    sceneRenderTexture.display();
}

void Flashlight::tick_idleAnim(const float& deltaTime)
{
    animWait.addValue(-deltaTime);
    if (animWait.isEmpty())
    {
        animFrameCount++;
        int textureIndex = animFrameCount % textures.size();
        static int lastTextureIndex = -1; // Track last assigned texture index
        if (textureIndex != lastTextureIndex)
        {
            flashlightSprite.setTexture(textures[textureIndex]);
            lastTextureIndex = textureIndex;
        }

        animWait.fill_to_max();
    }
}

void Flashlight::setMaskMode(const Flashlight::Type& type)
{
    flashlightShape = type;
    bUseCone = (type == Type::CONE);
}

void Flashlight::toggleMaskMode()
{
    bUseCone = !bUseCone;
    flashlightShape = (bUseCone ? Type::CONE : Type::CIRCLE);

}

sf::Shader* Flashlight::getActiveShader()
{
    switch (flashlightShape)
    {
    case Flashlight::Type::TEST:
        return &testShader;
        break;
    case Flashlight::Type::CIRCLE:
        return &circleShader;
        break;
    case Flashlight::Type::CONE:
        return &coneShader;
        break;
    default:
        break;
    }
    return nullptr;
}

void Flashlight::drawOtherScene(sf::Drawable* drawable)
{
	drawOtherScene(*drawable);
}

void Flashlight::drawOtherScene(sf::Drawable& drawable)
{
    // Draw your scene here (make sure to render the actual scene)
    sceneRenderTexture.draw(drawable);
    sf::BlendMode blendMode = sf::BlendMin;
    sceneRenderTexture.draw(flashlightSprite, blendMode);

    // Use the shader to draw the flashlight effect
    sf::Shader* currShader = getActiveShader();
    if (currShader != nullptr)
        sceneRenderTexture.draw(sceneSprite, currShader);

    sceneRenderTexture.display(); // Update the render texture
}

float Flashlight::getRadius() const
{
    return radius;
}

void Flashlight::setRadius(const float& newRadius)
{
    radius = newRadius;
    float radiusRatio = (newRadius / SHADER_RADIUS);
    usedScale = (SPRITE_SCALE * radiusRatio);
}

void Flashlight::setPosition(const sf::Vector2f& newPos)
{
    if (newPos != getPosition())
    {
        IMovable::setPosition(newPos);
        flashlightSprite.setPosition(newPos);
    }
}

void Flashlight::setRotation(const float& newRot)
{
    if (!(gameInstance().getIsPaused() || bUseCone))
    {
        if (newRot != getRotation())
        {
            IMovable::setRotation(newRot);
            flashlightSprite.setRotation(newRot);
        }
    }
}