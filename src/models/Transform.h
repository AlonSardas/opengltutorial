#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
  public:
    Transform(const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &rot = glm::vec3(0.0f),
              const glm::vec3 &scale = glm::vec3(1.0f))
        : position(pos), rotation(rot), scale(scale) {
        updateModelMatrix();
    }

    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getRotation() const { return rotation; }
    const glm::vec3 &getScale() const { return scale; }
    const glm::mat4 &getModelMatrix() const { return modelMatrix; }

    void setPosition(const glm::vec3 &pos) {
        position = pos;
        updateModelMatrix();
    }

    void setRotation(const glm::vec3 &rot) {
        rotation = rot;
        updateModelMatrix();
    }

    void setScale(const glm::vec3 &s) {
        scale = s;
        updateModelMatrix();
    }

    void translate(const glm::vec3 &delta) {
        position += delta;
        updateModelMatrix();
    }

    void rotate(const glm::vec3 &delta) {
        rotation += delta;
        updateModelMatrix();
    }

  private:
    void updateModelMatrix() {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::rotate(modelMatrix, rotation.x, glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.y, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, rotation.z, glm::vec3(0, 0, 1));
        modelMatrix = glm::scale(modelMatrix, scale);
    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 modelMatrix;
};