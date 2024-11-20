#pragma once
#include "SFML_Arena.h"

class Flashlight : public WidgetElement
{
private:
    const std::string circleMaskShader = R"(
        uniform vec2 lightPos; // light "Source" = Center of Mask
        uniform float radius; // Circle Radius
        uniform float viewportHeight; // Viewport Height (probably sf::RenderWindow)

        void main() {
            vec2 pos = gl_FragCoord.xy; // Fragment coords
            float adjustedY = viewportHeight - pos.y; // Flip Y coordinate because SFML
            vec2 adjustedPos = vec2(pos.x, adjustedY); // Rebuild Frag pos
            float dist = length(adjustedPos - lightPos); // Pos distance from mask Center

            vec4 color = vec4(0.0, 0.0, 0.0, 1.0); // Fully black (opaque) for later masking

            // If distance from source is smaller than max radius of circle
            if (dist < radius) {
                // Dist is in range of radius
                float alpha = smoothstep(radius, radius * 0.7, dist); // Creates a soft edge
                color = vec4(0.0, 0.0, 0.0, 1.0 - alpha); // Black with varying transparency (revealing the scene)
            }

            gl_FragColor = color; // Output the final color for Fragment
        }
    )";

    const std::string coneMaskShader = R"(
        uniform vec2 lightPos; // light "Source" = Center of Mask
        uniform vec2 direction; // Direction of the cone from the center(normalized)
        uniform float radius; // Cone Radius
        uniform float angle; // Half-angle of the cone in radians
        uniform float viewportHeight; // Height of the viewport for Y-axis flipping

        void main() {
            vec2 pos = gl_FragCoord.xy; // UV position of Fragment
            float adjustedY = viewportHeight - pos.y; // Flip Y coordinate
            vec2 adjustedPos = vec2(pos.x, adjustedY); // Rebuild position

            vec2 toPixel = adjustedPos - lightPos; 
            float dist = length(toPixel); // Get Frag distance from source

            vec4 color = vec4(0.0, 0.0, 0.0, 1.0); // Black overlay to mask out

            // Check if within cone and radius
            if (dist < radius) {
                vec2 fragDir = normalize(toPixel); // Normalize the direction to this pixel
                float dotProduct = dot(fragDir, direction);

                // Check if direction is within the cone angle
                if (dotProduct > cos(angle)) {
                    // dist is in range of radius
                    float alpha = smoothstep(radius, radius * 0.5, dist); // Creates a soft edge
                    color = vec4(0.0, 0.0, 0.0, 1.0 - alpha); // Black with transparency to reveal the scene
                }
            }

            gl_FragColor = color; // Output the final color for fragment
        }

    )";

    sf::Shader flashlightShader_Circle;
    sf::Shader flashlightShader_Cone;
    sf::RenderTexture sceneRenderTexture;
    sf::Sprite sceneSprite;

    const float SHADER_RADIUS = 320.0f;
    float radius = SHADER_RADIUS;
    const sf::Vector2f SPRITE_SCALE = { 1.4f, 1.4f };
    const sf::Vector2f SHADER_SPRITE_RATIO{ 1.4f / 320.0f, 1.4f / 320.0f };

    sf::Texture flashlightTexture;
    sf::Sprite flashlightSprite;
    
    Player* player = nullptr;

    std::vector<sf::Texture> textures = {};

    bool bUseCone = false;
public:
    Flashlight(WidgetElement* parent) : WidgetElement(parent)
    {
        flashlightSprite.setOrigin(512.0f / 2.0f, 512.0f / 2.0f);
        sf::Color color = flashlightSprite.getColor();
        color.a = 50;
        flashlightSprite.setColor(color);

        if (!flashlightShader_Circle.loadFromMemory(circleMaskShader, sf::Shader::Fragment))
        {
            throw std::runtime_error("Failed to load flashlight Circle shader.");
        }
        if (!flashlightShader_Cone.loadFromMemory(coneMaskShader, sf::Shader::Fragment))
        {
            throw std::runtime_error("Failed to load flashlight Cone shader.");
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

        sceneRenderTexture.setSmooth(false);

        sceneSprite.setTexture(sceneRenderTexture.getTexture());

        shapes = { &sceneSprite };
    }

    virtual void construct() override { return; }

    void update(const float& deltaTime) override
    {
        WidgetElement::update(deltaTime);
        player = gameInstance.getPlayer();

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
        sf::Vector2f newPos = player->getPos();
        if (newPos != lastPos) {
            flashlightSprite.setPosition(newPos);
            lastPos = newPos;
        }

        if (!(gameInstance.getIsPaused() || bUseCone))
        {
            float newRot = getLookAtRot(newPos, gameInstance.getMousePos());
            if (newRot != lastRot) {
                flashlightSprite.setRotation(newRot);
                lastRot = newRot;
            }
        }

        // Transform player's world position to view-space coordinates for shader
        sf::Vector2f viewOffset = view->getCenter() - (view->getSize() / 2.0f);
        sf::Vector2f lightPos = newPos - viewOffset;

        static float lastMouseDir[2] = { 0.0f, 0.0f };
        sf::Vector2f mouseDir = { lastMouseDir[0], lastMouseDir[1] };
        if (!gameInstance.getIsPaused() && bUseCone)
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
            lastMouseDir[0] = mouseDir.x; lastMouseDir[1] = mouseDir.y;
        }
        

        if (bUseCone)
        {
            flashlightSprite.setScale(SPRITE_SCALE * 2.0f);
            // Pass uniforms to the shader
            flashlightShader_Cone.setUniform("lightPos", lightPos);
            flashlightShader_Cone.setUniform("direction", mouseDir);
            flashlightShader_Cone.setUniform("radius", radius * 2.0f);
            flashlightShader_Cone.setUniform("angle", degreesToRadians(25.0f)); // 40° cone (20° half-angle)
            flashlightShader_Cone.setUniform("viewportHeight", view->getSize().y);
        }
        else
        {
            flashlightSprite.setScale(SPRITE_SCALE);
            // Pass transformed light position and viewport size to the shader
            flashlightShader_Circle.setUniform("lightPos", lightPos);
            flashlightShader_Circle.setUniform("radius", radius);
            flashlightShader_Circle.setUniform("viewportHeight", view->getSize().y);
        }

        // Set the render texture's view to match the player's view
        sceneRenderTexture.setView(*view);

        // Render flashlight effect
        sceneRenderTexture.clear(sf::Color::Black);
        sceneRenderTexture.display();

        // Ensure the render texture sprite is positioned properly
        sceneSprite.setPosition(view->getCenter() - (view->getSize() / 2.0f));
        return;
    }

    void setMaskMode(const bool& bCone = false)
    {
        bUseCone = bCone;
    }
    void toggleMaskMode()
    {
        bUseCone = !bUseCone;
    }

    sf::Vector2f getPos() const 
    {
        return flashlightSprite.getPosition();
    }

    void drawOtherScene(sf::Drawable* drawable)
    {
        // Draw your scene here (make sure to render the actual scene)
        sceneRenderTexture.draw(*drawable);
        sceneRenderTexture.draw(flashlightSprite);

        // Use the shader to draw the flashlight effect
        sf::Shader* currShader = (bUseCone ? &flashlightShader_Cone : &flashlightShader_Circle);
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
