#pragma once
#include "Entity.hpp"
#include <vector>

class Map {
public:
    std::vector<Entity*> entities;

    Map();
    ~Map();
    void Update(float deltaTime);
    void Render(ShaderProgram* program);
    void HandleClick(float mx, float my);
};
