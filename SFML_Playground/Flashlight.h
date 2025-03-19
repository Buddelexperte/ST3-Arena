#pragma once

#include "BaseClasses.h"

// TODO: Add enemy DeathLights

class Flashlight : public WidgetElement
{
private:
    enum FlashlightType {
        TEST = -1,
        CIRCLE = 0,
        CONE
    } shaderType = CIRCLE;

    static const inline std::string circleShader_Code = R"(
    uniform vec2 lightPos;
    uniform float radius;
    uniform float viewportHeight;
    uniform vec2 u_viewSize;

    // New uniforms for enemy death lights
    const int MAX_LIGHTS = 500;
    uniform int numDeathLights;
    uniform vec2 deathLightPos[MAX_LIGHTS];

    void main() {
        vec2 normalizedCoords = gl_FragCoord.xy / u_viewSize;
        vec2 worldCoords = normalizedCoords * u_viewSize;
        worldCoords.y = viewportHeight - worldCoords.y;

        float dist = length(worldCoords - lightPos);
        float alpha = smoothstep(radius, radius * 0.4, dist);
        
        // Start with a fully black color (opaque)

        // If within radius, calculate smooth alpha for soft edge
        if (dist < radius)
        {
            alpha = smoothstep(radius, radius * 0.4, dist); // Smooth transition
        }

        // Add contribution from enemy death lights
        for (int i = 0; i < numDeathLights; i++)
        {
            float distToDeathLight = length(worldCoords - deathLightPos[i]);
            float deathLightAlpha = smoothstep(radius * 0.3, radius * 0.1, distToDeathLight);
            alpha = max(alpha, deathLightAlpha); // Blend
        }

        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0 - alpha);
    }

)";

    static const inline std::string coneShader_Code = R"(
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
        if (dist < radius)
        {
            vec2 fragDir = normalize(toPixel); // Normalize the direction to this pixel
            float dotProduct = dot(fragDir, direction);

            // Check if within the cone angle
            if (dotProduct > cos(angle))
            {
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

    static const inline std::string debugShader_Code = R"(
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
    sf::Vector2f coneDir;

    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;

    static constexpr float SHADER_RADIUS = 480;
    float radius = SHADER_RADIUS;
    static const inline sf::Vector2f SPRITE_SCALE = { 2.1f, 2.1f };
    static const inline sf::Vector2f SHADER_SPRITE_RATIO { 2.1f / SHADER_RADIUS, 2.1f / SHADER_RADIUS };

    std::vector<sf::Texture> textures = {};
    sf::Sprite flashlightSprite;


    // Scrapped Light for dead enemies
    static constexpr bool bRenderDeathLights = false; // DESIGN CHOICE, DON'T TURN ON
    struct DeathLight
    {
        sf::Vector2f position;
        float timeRemaining;
    };
    std::vector<DeathLight> deathLights;


    void tick_animation(const float&);
    void tick_shader(const float&);
    void tick_deathLights(const float&);
    void tick_display(const float&);
public:
    Flashlight(InputWidget* parent);
    ~Flashlight() = default;

    virtual void construct() override { return; }

    void tick(const float& deltaTime) override;

    void setMaskMode(const bool& = false);

    void toggleMaskMode();

    sf::Vector2f getPosition() const
        { return flashlightSprite.getPosition(); }

    sf::Shader* getActiveShader();

    void drawOtherScene(sf::Drawable* drawable);

    void setRadius(const float& newRadius);

    void resetRadius() { setRadius(SHADER_RADIUS); }

    void setPosition(const sf::Vector2f& newPos) override;
    void setRotation(const float& newRot) override;

    // Additional Lights
    void addDeathLight(const sf::Vector2f&);

};