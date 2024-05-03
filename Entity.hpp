#pragma once
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"


class Entity {
public:
    float x, y; // Position
    float vx, vy; // Velocity
    bool isActive; // Activity state

    Entity(float posX, float posY, float velX, float velY);
    virtual ~Entity() {}
    void Update(float deltaTime);
    void Render(ShaderProgram* program);
    bool CheckCollision(float mx, float my); // Check if a mouse click is within the entity's bounds
};
