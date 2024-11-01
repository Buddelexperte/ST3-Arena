#pragma once
#include "SFML_Arena.h"

class Flashlight : public WidgetMenu
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
                float alpha = smoothstep(radius, radius * 0.9, dist); // Creates a soft edge
                color = vec4(0.0, 0.0, 0.0, 1.0 - alpha); // Black with varying transparency (revealing the scene)
            }

            gl_FragColor = color; // Output the color
        }
    )";

    sf::Shader flashlightShader;
    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;
    const float flashlightRadius = 300.0f;

public:
    Flashlight() : WidgetMenu()
    {
        if (!flashlightShader.loadFromMemory(flashlightShaderCode, sf::Shader::Fragment))
        {
            throw std::runtime_error("Failed to load flashlight shader.");
        }

        // Create the render texture with the window size
        if (!sceneRenderTexture.create(windowSize.x, windowSize.y))
        {
            throw std::runtime_error("Failed to create scene render texture.");
        }

        sceneSprite.setTexture(sceneRenderTexture.getTexture());
    }

    virtual void init() override { return; }

    virtual void update(const float& deltaTime) override
    {
        windowUpdate();
        // Clear the render texture with the black color
        sceneRenderTexture.clear(sf::Color::Black);
    }

    void drawOtherScene(sf::Drawable* drawable)
    {
        
        // Draw your scene here (make sure to render the actual scene)
        sceneRenderTexture.draw(*drawable);

        // Get mouse position and set shader uniforms
        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
        flashlightShader.setUniform("lightPos", mousePos);
        flashlightShader.setUniform("radius", flashlightRadius);
        flashlightShader.setUniform("viewportHeight", float(windowSize.y)); // Pass the viewport height

        // Use the shader to draw the flashlight effect
        sceneRenderTexture.draw(sceneSprite, &flashlightShader);

        sceneRenderTexture.display(); // Update the render texture
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(sceneSprite); // Draw the flashlight scene
    }
};
