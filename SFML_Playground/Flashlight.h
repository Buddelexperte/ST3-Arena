#pragma once
#include "BaseClasses.h"

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
    Flashlight(InputWidget* parent) : WidgetElement(parent)
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

    virtual void construct() override { return; }

    void update(const float& deltaTime) override
    {
        WidgetElement::update(deltaTime);

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
        sf::Vector2f newPos = gameInstance->getPlayer()->getPos();
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

    void setMaskMode(const bool& bCone = false)
    {
        bUseCone = bCone;
        shaderType = (bUseCone ? CONE : CIRCLE);
    }
    void toggleMaskMode()
    {
        bUseCone = !bUseCone;
        shaderType = (bUseCone ? CONE : CIRCLE);

    }

    sf::Vector2f getPos() const
    {
        return flashlightSprite.getPosition();
    }

    sf::Shader* getActiveShader()
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

    void drawOtherScene(sf::Drawable* drawable)
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

    void setRadius(const float& newRadius)
    {
        radius = newRadius;
        float radiusRatio = (newRadius / SHADER_RADIUS);
        sf::Vector2f newScale = { (SPRITE_SCALE * radiusRatio) };
        flashlightSprite.setScale(newScale);
    }
    void resetRadius() { setRadius(SHADER_RADIUS); }
};