#include "Portal.h"
#include "utils/debugUtils.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Portal::Portal(const std::string &name, const glm::vec3 &pos, const glm::vec3 &n, float angle, float w, float h)
    : name(name), destination(0), position(pos), normal(glm::normalize(n)), width(w), height(h), angle(angle),
      blankShader("shaders/mirrorTransformer.vs", "shaders/innerOnly.fs"),
      frameShader("shaders/mirrorTransformer.vs", "shaders/frameOnly.fs") {

    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::normalize(glm::cross(upVector, normal));
    up = glm::cross(normal, right);
    glm::mat3 rotation = glm::mat3(right, up, normal);

    modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, position);
    modelMat = modelMat * glm::mat4(rotation);
    modelMat = glm::scale(modelMat, glm::vec3(w, h, 1.0f));

    // std::cout << "model mat:" << std::endl;
    // printMat4(model);
}

void Portal::draw(const glm::mat4 &viewMat, const glm::mat4 &projMat) {
    frameShader.use();
    blankShader.setMat4("view", viewMat);
    blankShader.setMat4("projection", projMat);
    blankShader.setMat4("model", modelMat);

    quad.draw(frameShader);
}

void Portal::drawBlank(const glm::mat4 &viewMat, const glm::mat4 &projMat) {
    blankShader.use();
    blankShader.setMat4("view", viewMat);
    blankShader.setMat4("projection", projMat);
    blankShader.setMat4("model", modelMat);
    // blankShader.setMat4("model", glm::mat4(1.0f));

    quad.draw(blankShader);
}

std::optional<glm::mat4> Portal::getViewMat(const glm::mat4 &viewMat) const {
    glm::vec3 camForward = glm::normalize(glm::vec3(viewMat[2]));
    float facing = glm::dot(camForward, normal);
    if (facing > 0.5f) {
        // This makes problem
        // std::cout << "Camera not facing " << name << std::endl;
        // return std::nullopt;
    }

    glm::mat4 destinationView = viewMat * this->modelMat *
                                glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                glm::inverse(this->destination->modelMat);
    return destinationView;
}

glm::vec3 Portal::teleportPosition(const glm::vec3 &pos) const {
    glm::vec4 posw(pos, 1.0f);
    glm::mat4 transMatrix = this->destination->modelMat *
                            glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                            glm::inverse(modelMat);
    return transMatrix * posw;
}

glm::vec4 Portal::teleportVec(const glm::vec4 &vec) const {
    glm::mat4 transMatrix = this->destination->modelMat *
                            glm::rotate(glm::mat4(1.0f), glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f)) *
                            glm::inverse(modelMat);
    return transMatrix * vec;
}

std::optional<glm::mat4> Portal::getProjMat(const glm::mat4 &destView, const glm::mat4 &projMat) const {
    // glm::mat4 const Portal::getProjMat(const glm::mat4 &destView, const glm::mat4 &projMat) const {
    // return projMat;

    // following this:
    // https://www.terathon.com/lengyel/Lengyel-Oblique.pdf

    // Create clip plane in world space (ax + by + cz + d = 0 form)
    glm::vec4 clipPlane(normal, -glm::dot(normal, position));
    // glm::vec4 clipPlane(destination->normal, glm::dot(destination->normal, destination->position));
    // Transform to camera space
    clipPlane = glm::transpose(glm::inverse(destView)) * clipPlane;
    // clipPlane = viewMat * clipPlane;

    // printVec4("clipPlane", clipPlane);

    // If plane faces away from camera, don't clip
    if (clipPlane.w > 0.0f) {
        // std::cout << "Clip plane away from camera for " << name << std::endl;
        // return projMat;
        // return glm::mat4(0.0f);
        return std::nullopt;
    }
    // else {
    //     std::cout << "Fixing clip plane" << std::endl;
    // }
    // Eq 7
    glm::vec4 q = glm::inverse(projMat) * glm::vec4(glm::sign(clipPlane.x), glm::sign(clipPlane.y), 1.0f, 1.0f);

    // Eq 9
    // Note: the factor glm::dot(glm::row(projMat, 3), q) is always 1
    // float a = 2.0f * glm::dot(glm::row(projMat, 3), q) / glm::dot(clipPlane, q);
    float a = 2.0f / glm::dot(clipPlane, q);
    // float a = 2.0f / glm::dot(clipPlane, q);
    glm::vec4 c = clipPlane * a;

    glm::mat4 newProjMat = projMat;
    // printMat4("before", newProjMat);
    // Eq 10
    newProjMat = glm::row(newProjMat, 2, c - glm::row(newProjMat, 3));
    // printMat4("after", newProjMat);

    // printVec4("new clip plane", glm::row(newProjMat, 2) + glm::row(newProjMat, 3));

    return newProjMat;
}

bool Portal::isInside(const glm::vec3 &playerPos) const {
    glm::vec3 toPlayer = playerPos - position;

    // Project onto portal's local axes
    float x = glm::dot(toPlayer, right);
    float y = glm::dot(toPlayer, up);

    if (std::abs(x) <= width * 0.5f && std::abs(y) <= height * 0.5f) {
        return true;
    }
    return false;
}
