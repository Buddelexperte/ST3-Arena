#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class QuadTreeCollisionElement
{
public:
	virtual sf::FloatRect getBounds() const = 0;
};

class QuadTree
{
private:
    sf::FloatRect boundary;
    std::vector<QuadTreeCollisionElement*> objects;
    bool divided;
    QuadTree* children[4] = { nullptr, nullptr, nullptr, nullptr };

    // Subdivide this node into four children.
    void subdivide() {
        float x = boundary.left;
        float y = boundary.top;
        float w = boundary.width / 2.0f;
        float h = boundary.height / 2.0f;

        children[0] = new QuadTree(sf::FloatRect(x, y, w, h));               // Top-left
        children[1] = new QuadTree(sf::FloatRect(x + w, y, w, h));           // Top-right
        children[2] = new QuadTree(sf::FloatRect(x, y + h, w, h));           // Bottom-left
        children[3] = new QuadTree(sf::FloatRect(x + w, y + h, w, h));       // Bottom-right

        divided = true;
    }

public:
    // Maximum number of objects before subdividing
    static constexpr int DIV_THRESHOLD = 4;

    QuadTree(const sf::FloatRect& boundary)
        : boundary(boundary), divided(false) {
    }

    ~QuadTree() {
        // Clean up children if they were created
        for (auto child : children) {
            delete child;
        }
    }

    // Insert an object into the quad tree.
    bool insert(QuadTreeCollisionElement* object) {
        if (!boundary.intersects(object->getBounds()))
            return false; // The object doesn't belong in this node

        if (objects.size() < DIV_THRESHOLD) {
            objects.push_back(object);
            return true;
        }

        if (!divided)
            subdivide();

        // Try to insert into one of the children.
        for (auto child : children) {
            if (child->insert(object))
                return true;
        }

        // In case the object spans multiple regions, you might store it in the current node.
        objects.push_back(object);
        return true;
    }

    // Retrieve all objects that could collide with a given area.
    void query(const sf::FloatRect& range, std::vector<QuadTreeCollisionElement*>& found) {
        if (!boundary.intersects(range))
            return; // No intersection, nothing to find

        // Check objects in this node
        for (auto object : objects) {
            if (range.intersects(object->getBounds()))
                found.push_back(object);
        }

        // Recursively check children if they exist
        if (divided) {
            for (auto child : children) {
                child->query(range, found);
            }
        }
    }
};

// TODO: FINISH IMPLEMENTATION