#pragma once
#include "SFML_Arena.h"

class Flashlight : public WidgetElement
{
private:
    const std::string flashlightShaderCode = R"(
        uniform vec2 lightPos;
        uniform float radius;
        uniform float viewportHeight;

        void main() {
            vec2 pos = gl_FragCoord.xy;
            float adjustedY = viewportHeight - pos.y; // Flip Y coordinate
            vec2 adjustedPos = vec2(pos.x, adjustedY);
            float dist = length(adjustedPos - lightPos);

            // Default to black background (fully opaque)
            vec4 color = vec4(0.0, 0.0, 0.0, 1.0); // Fully black (opaque)

            // Calculate alpha based on distance from lightPos
            if (dist < radius) {
                float alpha = smoothstep(radius, radius * 0.7, dist); // Creates a soft edge
                color = vec4(0.0, 0.0, 0.0, 1.0 - alpha); // Black with varying transparency (revealing the scene)
            }

            gl_FragColor = color; // Output the color
        }
    )";

    sf::Shader flashlightShader;
    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;

    const float SHADER_RADIUS = 320.0f;
    float radius = SHADER_RADIUS;
    const sf::Vector2f SPRITE_SCALE = { 1.4f, 1.4f };
    const sf::Vector2f SHADER_SPRITE_RATIO{ 1.4f / 320.0f, 1.4f / 320.0f };

    sf::Texture flashlightTexture;
    sf::Sprite* flashlightSprite = new sf::Sprite;
    
    Player* player = nullptr;

    std::vector<sf::Texture> textures = {};
public:
    Flashlight() : WidgetElement()
    {
        flashlightSprite->setOrigin(512.0f / 2.0f, 512.0f / 2.0f);
        flashlightSprite->setScale(SPRITE_SCALE);
        sf::Color color = flashlightSprite->getColor();
        color.a = 128;
        flashlightSprite->setColor(color);

        if (!flashlightShader.loadFromMemory(flashlightShaderCode, sf::Shader::Fragment))
        {
            throw std::runtime_error("Failed to load flashlight shader.");
        }

        for (int i = 52; i <= 60; i++)
        {
            sf::Texture newTexture;
            std::string texturePath = "../Content/Textures/512x512 textures (" + std::to_string(i) + ").png";
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

        sceneSprite.setTexture(sceneRenderTexture.getTexture());

        shapes = { &sceneSprite, flashlightSprite };

    }

    virtual void construct() override { return; }

    virtual void update(const float& deltaTime) override
    {
        WidgetElement::update(deltaTime);
        player = gameInstance.getPlayer();
        // Flashlight Movement and pic by pic
        static int steps = 0;
        if (++steps % 60 == 0) {
            int textureIndex = steps % textures.size();  // Safely index within the bounds of textures
            flashlightTexture = textures[textureIndex];  // Set the texture to the sprite
            flashlightSprite->setTexture(flashlightTexture);
        }
        sf::Vector2f newPos = player->getPos();
        float newRot = player->getRot();
        flashlightSprite->setPosition(newPos);
        flashlightSprite->setRotation(newRot);
        // Clear the render texture with the black color
        sceneRenderTexture.clear(sf::Color::Black);
        // Get mouse position and set shader uniforms
        flashlightShader.setUniform("lightPos", newPos);
        flashlightShader.setUniform("radius", radius);
        flashlightShader.setUniform("viewportHeight", float(windowSize.y)); // Pass the viewport height
    }

    sf::Vector2f getPos() const 
    {
        return flashlightSprite->getPosition();
    }

    void drawOtherScene(sf::Drawable* drawable)
    {
        // Draw your scene here (make sure to render the actual scene)
        sceneRenderTexture.draw(*drawable);

        // Use the shader to draw the flashlight effect
        sceneRenderTexture.draw(sceneSprite, &flashlightShader);

        sceneRenderTexture.display(); // Update the render texture
    }

    void setRadius(const float& newRadius)
    {
        radius = newRadius;
        float radiusRatio = (newRadius / SHADER_RADIUS);
        sf::Vector2f newScale = { (SPRITE_SCALE * radiusRatio) };
        flashlightSprite->setScale(newScale);
    }
    void resetRadius() { setRadius(SHADER_RADIUS); }
};
