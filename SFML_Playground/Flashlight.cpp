#pragma once

#include "Flashlight.h"
#include "GameInstance.h"
#include "Player.h"


Flashlight::Flashlight(InputWidget* parent)
    : WidgetElement(parent)
{
    flashlightSprite.setOrigin(512.0f / 2.0f, 512.0f / 2.0f);
    sf::Color color = flashlightSprite.getColor();
    color.a = 200;
    flashlightSprite.setColor(color);

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
        if (!newTexture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture: " << texturePath << std::endl;
            continue;
        }
        textures.push_back(newTexture);
    }

    // Create the render texture with the window size
    if (!sceneRenderTexture.create(windowSize.x, windowSize.y))
    {
        throw std::runtime_error("Failed to create scene render texture.");
    }

    sceneRenderTexture.setSmooth(false);

    sceneSprite.setTexture(sceneRenderTexture.getTexture());

    shapes = { &sceneSprite };
}

void Flashlight::tick(const float& deltaTime)
{
    WidgetElement::tick(deltaTime);

    sf::Vector2f viewSize = gameInstance->getView()->getSize();

    currShader = getActiveShader();
    currShader->setUniform("u_viewSize", sf::Glsl::Vec2(viewSize));
    currShader->setUniform("viewportHeight", static_cast<float>(window->getSize().y));

    // Update flashlight texture
    static int steps = 0;
    if (++steps % 100 == 0) {
        int textureIndex = steps % textures.size();
        static int lastTextureIndex = -1; // Track last assigned texture index
        if (textureIndex != lastTextureIndex) {
            flashlightTexture = textures[textureIndex];
            flashlightSprite.setTexture(flashlightTexture);
            lastTextureIndex = textureIndex;
        }
    }

    // Update flashlight position and rotation
    static sf::Vector2f lastPos;
    static float lastRot;
    sf::Vector2f newPos = gameInstance->getPlayer()->getPosition();
    if (newPos != lastPos) {
        flashlightSprite.setPosition(newPos);
        lastPos = newPos;
    }

    if (!(gameInstance->getIsPaused() || bUseCone))
    {
        float newRot = getLookAtRot(newPos, gameInstance->getMousePos());
        if (newRot != lastRot) {
            flashlightSprite.setRotation(newRot);
            lastRot = newRot;
        }
    }

    // Transform player's world position to view-space coordinates for shader
    sf::Vector2f viewOffset = view->getCenter() - (view->getSize() / 2.0f);
    sf::Vector2f lightPos = newPos - viewOffset;

    static sf::Vector2f lastMouseDir = { 0.0f, 0.0f };
    sf::Vector2f mouseDir = lastMouseDir;
    if (!gameInstance->getIsPaused() && bUseCone)
    {
        // Calculate direction vector to mouse
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window); // Mouse in window coordinates
        sf::Vector2f mouseWorldPosition = window->mapPixelToCoords(mousePosition, *view); // Transform to world coords

        // Normalize direction vector
        mouseDir = mouseWorldPosition - newPos;
        if (mouseDir != sf::Vector2f(0, 0))
        {
            float len = std::sqrt(mouseDir.x * mouseDir.x + mouseDir.y * mouseDir.y);
            if (len > 0.0001f)
            {
                mouseDir /= len;
            }
        }
        lastMouseDir = mouseDir;
    }

    switch (shaderType)
    {
    case Flashlight::CIRCLE:
        flashlightSprite.setScale(SPRITE_SCALE);
        flashlightShader_Circle.setUniform("lightPos", lightPos);
        flashlightShader_Circle.setUniform("radius", radius);
        flashlightShader_Circle.setUniform("viewportHeight", view->getSize().y);
        break;
    case Flashlight::CONE:
        flashlightSprite.setScale(SPRITE_SCALE * 2.0f);
        flashlightShader_Cone.setUniform("lightPos", lightPos);
        flashlightShader_Cone.setUniform("direction", mouseDir);
        flashlightShader_Cone.setUniform("radius", radius * 2.0f);
        flashlightShader_Cone.setUniform("angle", degreesToRadians(30.0f)); // 60° cone (30° half-angle)
        flashlightShader_Cone.setUniform("viewportHeight", view->getSize().y);
        break;
    default:
        break;
    }

    // Set the render texture's view to match the player's view
    sceneRenderTexture.setView(*view);

    // Render flashlight effect
    sceneRenderTexture.clear(sf::Color(0, 0, 0, 0)); // Clear with fully transparent background

    sceneRenderTexture.display();

    // Ensure the render texture sprite is positioned properly
    sceneSprite.setPosition(view->getCenter() - (view->getSize() / 2.0f));
    return;
}

void Flashlight::setMaskMode(const bool bCone)
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
    sceneRenderTexture.draw(flashlightSprite, sf::BlendMin);
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