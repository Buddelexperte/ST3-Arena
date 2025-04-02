#pragma once
#include "Flashlight.h"
#include "GameInstance.h"


Flashlight::Flashlight()
    : animWait(ANIM_FRAME_WAIT)
{
    constexpr float SPRITE_SIZE = 512.0f;
    flashlightSprite.setOrigin(SPRITE_SIZE / 2.0f, SPRITE_SIZE / 2.0f);

    sf::Color spriteTint = flashlightSprite.getColor();
    spriteTint.a = 180;
    flashlightSprite.setColor(spriteTint);

    if (!flashlightShader_Circle.loadFromMemory(circleShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load flashlight Circle shader.");
    }
    if (!flashlightShader_Cone.loadFromMemory(coneShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load flashlight Cone shader.");
    }
    if (!testShader.loadFromMemory(debugShader_Code, sf::Shader::Fragment))
    {
        throw std::runtime_error("Failed to load test shader.");
    }

    for (int i = 52; i <= 60; i++)
    {
        sf::Texture newTexture;
        std::string texturePath = "Content/Textures/512x512 textures (" + std::to_string(i) + ").png";
        if (!newTexture.loadFromFile(texturePath))
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

    sceneRenderTexture.setSmooth(false);

    sceneSprite.setTexture(sceneRenderTexture.getTexture());

    shapes = { &sceneSprite };

    addDeathLight(sf::Vector2f(0.0f, 0.0f));
}

void Flashlight::tick(const float& deltaTime)
{
    tick_animation(deltaTime);

    tick_shader(deltaTime);

    tick_deathLights(deltaTime);

    tick_display(deltaTime);
}

void Flashlight::tick_animation(const float& deltaTime)
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

        animWait.reset();
    }
}

void Flashlight::tick_shader(const float& deltaTime)
{
    currShader = getActiveShader();
    if (!currShader)
        return;

    // Update flashlight position and rotation
    sf::Vector2f playerPos = gameInstance().getPlayer()->getPosition();
    setPosition(playerPos);

    float newRot = getLookAtRot(playerPos, gameInstance().getMousePos());
    setRotation(newRot);


    // Transform player's world position to view-space coordinates for shader
    sf::Vector2f viewOffset = view->getCenter() - (view->getSize() / 2.0f);
    sf::Vector2f lightPos = playerPos - viewOffset;

    // Calculation for Cone specific mouse direction
    if (!gameInstance().getIsPaused() && bUseCone)
    {
        // Calculate direction vector to mouse
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window); // Mouse in window coordinates
        sf::Vector2f mouseWorldPosition = window->mapPixelToCoords(mousePosition, *view); // Transform to world coords

        // Normalize direction vector
        sf::Vector2f mouseDir = mouseWorldPosition - playerPos;
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

    switch (shaderType)
    {
    case Flashlight::CIRCLE:
        flashlightSprite.setScale(SPRITE_SCALE);
        flashlightShader_Circle.setUniform("lightPos", lightPos);
        flashlightShader_Circle.setUniform("radius", radius);
        flashlightShader_Circle.setUniform("u_viewSize", sf::Glsl::Vec2(viewSize));
        flashlightShader_Circle.setUniform("viewportHeight", view->getSize().y);
        break;
    case Flashlight::CONE:
        flashlightSprite.setScale(SPRITE_SCALE * 2.0f);
        flashlightShader_Cone.setUniform("lightPos", lightPos);
        flashlightShader_Cone.setUniform("radius", radius * 2.0f);
        flashlightShader_Cone.setUniform("direction", coneDir);
        flashlightShader_Cone.setUniform("angle", degreesToRadians(30.0f)); // 60° cone (30° half-angle)
        flashlightShader_Cone.setUniform("u_viewSize", sf::Glsl::Vec2(viewSize));
        flashlightShader_Cone.setUniform("viewportHeight", view->getSize().y);
        break;
    default:
        break;
    }
}

void Flashlight::tick_deathLights(const float& deltaTime)
{
    if (!bRenderDeathLights)
        return;

    // Remove expired death lights
    std::vector<sf::Glsl::Vec2> lightPositions;
    for (auto it = deathLights.begin(); it != deathLights.end();)
    {
        it->timeRemaining -= deltaTime;
        if (shouldZero(it->timeRemaining))
        {
            it = deathLights.erase(it);
        }
        else
        {
            lightPositions.push_back(sf::Glsl::Vec2(it->position - viewCenter + viewHalfSize));
            it++;
        }
    }

    // Pass death light information to shader using uniform params
    currShader->setUniform("numDeathLights", static_cast<int>(lightPositions.size()));
    if (lightPositions.size() > 0)
    {
        currShader->setUniformArray("deathLightPos", lightPositions.data(), lightPositions.size());
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

void Flashlight::setMaskMode(const bool& bCone)
{
    bUseCone = bCone;
    shaderType = (bUseCone ? CONE : CIRCLE);
}

void Flashlight::toggleMaskMode()
{
    bUseCone = !bUseCone;
    shaderType = (bUseCone ? CONE : CIRCLE);

}

sf::Shader* Flashlight::getActiveShader()
{
    switch (shaderType)
    {
    case Flashlight::TEST:
        return &testShader;
        break;
    case Flashlight::CIRCLE:
        return &flashlightShader_Circle;
        break;
    case Flashlight::CONE:
        return &flashlightShader_Cone;
        break;
    default:
        break;
    }
    return nullptr;
}

void Flashlight::drawOtherScene(sf::Drawable* drawable)
{
    // Draw your scene here (make sure to render the actual scene)
    sceneRenderTexture.draw(*drawable);
    sf::BlendMode blendMode = sf::BlendMin;
    sceneRenderTexture.draw(flashlightSprite, blendMode);

    // Use the shader to draw the flashlight effect
    sf::Shader* currShader = getActiveShader();
    if (currShader != nullptr)
        sceneRenderTexture.draw(sceneSprite, currShader);

    sceneRenderTexture.display(); // Update the render texture
}

void Flashlight::setRadius(const float& newRadius)
{
    radius = newRadius;
    float radiusRatio = (newRadius / SHADER_RADIUS);
    sf::Vector2f newScale = { (SPRITE_SCALE * radiusRatio) };
    flashlightSprite.setScale(newScale);
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

void Flashlight::addDeathLight(const sf::Vector2f& position)
{
    deathLights.push_back({ position, 5.0f }); // Lasts 5 seconds
}
