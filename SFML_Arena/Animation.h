#include <SFML/Graphics.hpp>

class Animation
{
private:
    sf::RectangleShape& targetShape; // Das Ziel (z. B. dein Button oder Sprite)
    sf::Vector2i frameSize; // Die Größe eines Frames (z. B. 64x64 Pixel)
    int frameCount; // Anzahl der Frames in der Animation
    float frameTime; // Zeit pro Frame in Sekunden
    float elapsedTime; // Verstrichene Zeit
    int currentFrame; // Aktueller Frame

public:
    Animation(sf::RectangleShape& shape, const sf::Vector2i& frameSize, int frameCount, float frameTime)
        : targetShape(shape), frameSize(frameSize), frameCount(frameCount), frameTime(frameTime), elapsedTime(0), currentFrame(0) {}

    void update(float deltaTime)
    {
        elapsedTime += deltaTime;

        // Wechsel zum nächsten Frame, wenn genügend Zeit vergangen ist
        if (elapsedTime >= frameTime)
        {
            elapsedTime -= frameTime;
            currentFrame = (currentFrame + 1) % frameCount;

            // Berechne den Texturausschnitt (nur horizontal in diesem Beispiel)
            int left = currentFrame * frameSize.x;
            targetShape.setTextureRect(sf::IntRect(left, 0, frameSize.x, frameSize.y));
        }
    }
};
