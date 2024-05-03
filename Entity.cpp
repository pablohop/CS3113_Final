#include "Entity.hpp"

Entity::Entity(float posX, float posY, float velX, float velY) : x(posX), y(posY), vx(velX), vy(velY), isActive(true) {}

void Entity::Update(float deltaTime) {
    // Simple movement logic
    x += vx * deltaTime;
    y += vy * deltaTime;
}

void Entity::Render(ShaderProgram* program) {
    if (!isActive) return;

    // Set up transformation matrix for the shader
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, 0.0f));
    program->set_model_matrix(modelMatrix);

    // Example rendering code (Assuming vertices and textures are set up)
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

bool Entity::CheckCollision(float mx, float my) {
    // Assuming entity size for simplicity
    float width = 1.0f; // Width of the entity
    float height = 1.0f; // Height of the entity
    return (mx >= x && mx <= x + width && my >= y && my <= y + height);
}
