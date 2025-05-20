#pragma once

#include "WidgetBase.h"
#include "ValueBar.h"

// TODO: Add enemy DeathLights

class Flashlight : public WidgetElement
{
public:
    enum class Type {
        TEST = -1,
        CIRCLE = 0,
        CONE
    };

private:
    Flashlight::Type flashlightShape = Flashlight::Type::CIRCLE;

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
    uniform vec2 lightPos;         // Light "Source" = Center of Mask
    uniform vec2 direction;        // Direction of the first cone (normalized)
    uniform float radius;          // Cone Radius
    uniform float angle;           // Half-angle of each cone in radians
    uniform bool useSecondCone;    // Toggle for second cone
    uniform float viewportHeight;  // Height of the viewport
    uniform vec2 u_viewSize;       // Size of the view

    void main() {
        // Convert gl_FragCoord to normalized coordinates (0 to 1)
        vec2 normalizedCoords = gl_FragCoord.xy / u_viewSize;

        // Scale back to world space & flip Y
        vec2 worldCoords = normalizedCoords * u_viewSize;
        worldCoords.y = viewportHeight - worldCoords.y;

        // Vector & distance from light to pixel
        vec2 toPixel = worldCoords - lightPos;
        float dist = length(toPixel);

        // Default: fully black (opaque mask)
        vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

        if (dist < radius)
        {
            vec2 fragDir = normalize(toPixel);
            float cosA = cos(angle);

            // FIRST CONE TEST
            float dp1 = dot(fragDir, direction);
            bool inFirst  = (dp1 > cosA);

            // SECOND CONE TEST (opposite direction)
            bool inSecond = false;
            float dp2 = 0.0;
            if (useSecondCone)
            {
                dp2 = dot(fragDir, -direction);
                inSecond = (dp2 > cosA);
            }

            // If pixel is in either cone, compute smooth alpha
            if (inFirst || inSecond)
            {
                // Distance fade
                float distanceFactor = smoothstep(radius, radius * 0.5, dist);

                // Angle fade: pick the larger dot-product for tighter smoothing
                float bestDp = inFirst ? dp1 : dp2;
                float angleFactor = smoothstep(cosA, 0.999, bestDp);

                float alpha = distanceFactor * angleFactor;
                color = vec4(0.0, 0.0, 0.0, 1.0 - alpha);
            }
        }

        gl_FragColor = color;
    }
    )";


    static const inline std::string testShader_Code = R"(
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

    sf::Shader circleShader;
    sf::Shader coneShader;
    sf::Shader testShader;

    sf::Shader* currShader = nullptr;
    bool bUseCone = false;
    bool bSecondCone = false;

    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;

    sf::Vector2f coneDir;
    static constexpr float SHADER_RADIUS = 480;
    float radius = SHADER_RADIUS;
    static const inline sf::Vector2f SPRITE_SCALE = { 2.1f, 2.1f };
    sf::Vector2f usedScale = SPRITE_SCALE;
    static const inline sf::Vector2f SHADER_SPRITE_RATIO { 2.1f / SHADER_RADIUS, 2.1f / SHADER_RADIUS };

    std::vector<sf::Texture> textures = {};
    static constexpr float ANIM_FRAME_WAIT = 0.075f;
    unsigned int animFrameCount = 0;
    ValueBar animWait;
    sf::Sprite flashlightSprite;

    void tick_idleAnim(const float&) override;
    void tick_shader(const float&);
    void tick_display(const float&);


public:
    Flashlight();
    ~Flashlight() = default;

    void tick(const float& deltaTime) override;

    void setMaskMode(const Flashlight::Type& = Flashlight::Type::CIRCLE);
    void setSecondCone(const bool& useSecond);

    void toggleMaskMode();

    sf::Vector2f getPosition() const
        { return flashlightSprite.getPosition(); }

    sf::Shader* getActiveShader();

    void drawOtherScene(sf::Drawable* drawable);
    void drawOtherScene(sf::Drawable& drawable);

    void setRadius(const float& newRadius);
    void resetRadius() { setRadius(SHADER_RADIUS); }
    float getRadius() const;

    void setPosition(const sf::Vector2f& newPos) override;
    void setRotation(const float& newRot) override;
};