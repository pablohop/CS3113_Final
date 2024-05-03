#include "Map.hpp"

Map::Map() {
    // Initialize some entities for demonstration
    entities.push_back(new Entity(100, 100, 0.1f, 0.0f)); // Entity moving horizontally
    entities.push_back(new Entity(150, 150, 0.0f, 0.1f)); // Entity moving vertically
}

Map::~Map() {
    for (Entity* e : entities) {
        delete e;
    }
}

void Map::Update(float deltaTime) {
    for (Entity* e : entities) {
        e->Update(deltaTime);
    }
}

void Map::Render(ShaderProgram* program) {
    for (Entity* e : entities) {
        e->Render(program);
    }
}

void Map::HandleClick(float mx, float my) {
    for (Entity* e : entities) {
        if (e->CheckCollision(mx, my)) {
            e->isActive = false; // For example, deactivate the entity
        }
    }
}
