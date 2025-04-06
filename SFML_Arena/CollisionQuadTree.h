#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <memory>

#include "Collision.h"

class CollisionQuadTree {
private:
    sf::FloatRect boundary;
    std::vector<CollisionBox*> objects;
    bool divided;
    std::unique_ptr<CollisionQuadTree> children[4]; // Verwendung von smart pointers

    // Unterteile diesen Knoten in vier Kinder.
    void subdivide() {
        float x = boundary.left;
        float y = boundary.top;
        float w = boundary.width / 2.0f;
        float h = boundary.height / 2.0f;

        children[0] = std::make_unique<CollisionQuadTree>(sf::FloatRect(x, y, w, h));        // Top-left
        children[1] = std::make_unique<CollisionQuadTree>(sf::FloatRect(x + w, y, w, h));    // Top-right
        children[2] = std::make_unique<CollisionQuadTree>(sf::FloatRect(x, y + h, w, h));    // Bottom-left
        children[3] = std::make_unique<CollisionQuadTree>(sf::FloatRect(x + w, y + h, w, h)); // Bottom-right

        divided = true;
    }

public:
    // Maximale Anzahl an Objekten bevor eine Unterteilung erfolgt
    static constexpr int DIV_THRESHOLD = 4;

    CollisionQuadTree(const sf::FloatRect& boundary)
        : boundary(boundary), divided(false)
    {
    }

    // Leert den QuadTree rekursiv
    void clear() {
        objects.clear();
        if (divided) {
            for (int i = 0; i < 4; ++i) {
                children[i]->clear();
                children[i].reset();
            }
            divided = false;
        }
    }

    // Fügt ein Objekt in den QuadTree ein.
    bool insert(CollisionBox* object) {
        if (!boundary.intersects(object->getBounds()))
            return false; // Das Objekt gehört nicht in diesen Bereich

        if (objects.size() < DIV_THRESHOLD) {
            objects.push_back(object);
            return true;
        }

        if (!divided)
            subdivide();

        // Versuche, das Objekt in einen der Kinder einzufügen.
        for (int i = 0; i < 4; ++i) {
            if (children[i]->insert(object))
                return true;
        }

        // Falls das Objekt mehrere Bereiche überschneidet, speichern wir es im aktuellen Knoten.
        objects.push_back(object);
        return true;
    }

    // Sucht alle Objekte, die mit dem gegebenen Bereich kollidieren könnten.
    void query(const sf::FloatRect& range, std::vector<CollisionBox*>& found) {
        if (!boundary.intersects(range))
            return; // Kein Schnitt, also nichts gefunden

        // Überprüfe Objekte in diesem Knoten
        for (auto object : objects) {
            if (range.intersects(object->getBounds()))
                found.push_back(object);
        }

        // Rekursiv in den Kindern suchen
        if (divided) {
            for (int i = 0; i < 4; ++i) {
                children[i]->query(range, found);
            }
        }
    }
};
