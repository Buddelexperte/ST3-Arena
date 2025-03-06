#pragma once

#include "BaseClasses.h"

class Player;

class Flashlight : public WidgetElement
{
private:
    enum FlashlightType {
        TEST = -1,
        CIRCLE = 0,
        CONE
    } shaderType = CIRCLE;

    const std::string circleShader_Code = R"(
    uniform vec2 lightPos; // light "Source" = Center of Mask
    uniform float radius; // Circle Radius
    uniform float viewportHeight; // Viewport Height
    uniform vec2 u_viewSize; // Size of the view

    void main() {
        // Convert gl_FragCoord to normalized coordinates (0 to 1)
        vec2 normalizedCoords = gl_FragCoord.xy / u_viewSize; 

        // Scale normalized coordinates back to world space
        vec2 worldCoords = normalizedCoords * u_viewSize;

        // Flip Y coordinate (SFML's Y axis is flipped for textures)
        worldCoords.y = viewportHeight - worldCoords.y;

        // Calculate distance from light position
        float dist = length(worldCoords - lightPos);

        // Start with a fully black color (opaque)
        vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

        // If within radius, calculate smooth alpha for soft edge
        if (dist < radius) {
            float alpha = smoothstep(radius, radius * 0.4, dist); // Smooth transition
            color = vec4(0.0, 0.0, 0.0, 1.0 - alpha); // Black with varying transparency
        }

        gl_FragColor = color; // Output final color for the fragment
    }
)";

    const std::string coneShader_Code = R"(
    uniform vec2 lightPos; // Light "Source" = Center of Mask
    uniform vec2 direction; // Direction of the cone from the center (normalized)
    uniform float radius; // Cone Radius
    uniform float angle; // Half-angle of the cone in radians
    uniform float viewportHeight; // Height of the viewport
    uniform vec2 u_viewSize; // Size of the view

    void main() {
        // Convert gl_FragCoord to normalized coordinates (0 to 1)
        vec2 normalizedCoords = gl_FragCoord.xy / u_viewSize;

        // Scale normalized coordinates back to world space
        vec2 worldCoords = normalizedCoords * u_viewSize;

        // Flip Y coordinate (SFML's Y axis is flipped for textures)
        worldCoords.y = viewportHeight - worldCoords.y;

        // Calculate the vector to the fragment and its distance
        vec2 toPixel = worldCoords - lightPos;
        float dist = length(toPixel);

        // Start with a fully black color (opaque)
        vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

        // Check if within radius
        if (dist < radius) {
            vec2 fragDir = normalize(toPixel); // Normalize the direction to this pixel
            float dotProduct = dot(fragDir, direction);

            // Check if within the cone angle
            if (dotProduct > cos(angle)) {
                // Smooth transition for distance from lightPos
                float distanceFactor = smoothstep(radius, radius * 0.5, dist);

                // Smooth transition for the cone's angle (sideways blending)
                float angleFactor = smoothstep(cos(angle), 0.999, dotProduct);

                // Combine both factors multiplicatively
                float alpha = distanceFactor * angleFactor;

                // Output color with appropriate alpha blending
                color = vec4(0.0, 0.0, 0.0, 1.0 - alpha);
            }

        }

        gl_FragColor = color; // Output final color for the fragment
    }
)";

    const std::string debugShader_Code = R"(
    uniform vec2 lightPos; // Light "Source" = Center of Mask
    uniform vec2 direction; // Direction of the cone from the center (normalized)
    uniform float radius; // Cone Radius
    uniform float angle; // Half-angle of the cone in radians
    uniform float viewportHeight; // Height of the viewport
    uniform vec2 u_viewSize; // Size of the view

    void main() {
        vec2 normalizedCoords = gl_FragCoord.xy / u_viewSize;
        gl_FragColor = vec4(normalizedCoords, 0.0, 1.0); // Output a gradient
    }
)";

    sf::Shader flashlightShader_Circle;
    sf::Shader flashlightShader_Cone;
    sf::Shader testShader;

    sf::Shader* currShader = nullptr;
    bool bUseCone = false;

    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;

    const float SHADER_RADIUS = 480;
    float radius = SHADER_RADIUS;
    const sf::Vector2f SPRITE_SCALE = { 2.1f, 2.1f };
    const sf::Vector2f SHADER_SPRITE_RATIO{ 2.1f / SHADER_RADIUS, 2.1f / SHADER_RADIUS };

    sf::Texture flashlightTexture;
    sf::Sprite flashlightSprite;

    Player* player = nullptr;

    std::vector<sf::Texture> textures = {};

public:
    Flashlight(InputWidget* parent);

    virtual void construct() override { return; }

    void tick(const float& deltaTime) override;

    void setMaskMode(const bool = false);

    void toggleMaskMode();

    sf::Vector2f getPosition() const
        { return flashlightSprite.getPosition(); }

    sf::Shader* getActiveShader();

    void drawOtherScene(sf::Drawable* drawable);

    void setRadius(const float& newRadius);

    void resetRadius() { setRadius(SHADER_RADIUS); }
};