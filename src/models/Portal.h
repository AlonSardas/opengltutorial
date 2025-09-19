/*
Following this:
https://medium.com/@alikomurcu/portals-with-opengl-d74da6241dd4
*/

#pragma once

#include "graphics/Shader.h"
#include "primitives/QuadNoNormals.h"
#include <glm/glm.hpp>
#include <optional>

class Portal {
  public:
    Portal(const std::string &name, const glm::vec3 &pos, const glm::vec3 &n, float angle, float width, float height);
    ~Portal() = default;
    Portal(const Portal &) = delete;
    Portal &operator=(const Portal &) = delete;
    Portal(Portal &&) = delete;
    Portal &operator=(Portal &&) = delete;

    std::optional<glm::mat4> getProjMat(glm::mat4 const &destView, glm::mat4 const &projMat) const;
    // glm::mat4 const getProjMat(glm::mat4 const &destView, glm::mat4 const &projMat) const;
    std::optional<glm::mat4> getViewMat(const glm::mat4 &viewMat) const;
    glm::vec3 teleportPosition(const glm::vec3 &pos) const;
    glm::vec4 teleportVec(const glm::vec4 &vec) const;
    void setDestination(Portal *dest) { destination = dest; };
    Portal *getDestination() const { return destination; };
    void draw(const glm::mat4 &viewMat, const glm::mat4 &projMat);
    void drawBlank(const glm::mat4 &viewMat, const glm::mat4 &projMat);
    bool isInside(const glm::vec3 &playerPos) const;

    const std::string &getName() const { return name; }
    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getNormal() const { return normal; }

  private:
    std::string name;
    Portal *destination;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 right;
    glm::vec3 up;
    float width, height;
    float angle;
    glm::mat4 modelMat;
    QuadNoNormals quad;
    Shader blankShader, frameShader;
};