#include "LayeredMirror.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stdexcept>
#include <vector>

LayeredMirror::LayeredMirror(const glm::vec3 &pos, const glm::vec3 &norm, float width, float height, int textureWidth,
                             int textureHeight, int levels)
    : position(pos), normal(glm::normalize(norm)), upVector(0.0f, 1.0f, 0.0f), mirrorWidth(width), mirrorHeight(height),
      renderDistance(50.0f), active(true), shader("shaders/mirrorTransformer.vs", "shaders/mirrorTexture.fs"),
      currentLevel(0), maxLevels(levels) {
    init(textureWidth, textureHeight);
}

void LayeredMirror::init(int textureWidth, int textureHeight) {
    // Orient the mirror quad according to its normal
    glm::vec3 forward = -normal;
    glm::vec3 right = glm::normalize(glm::cross(upVector, forward));
    glm::vec3 up = glm::cross(forward, right);
    mirrorRotation = glm::mat3(right, up, forward);

    // Initialize all framebuffers
    framebuffers.reserve(maxLevels);
    for (int i = 0; i < maxLevels; ++i) {
        auto &framebuffer = framebuffers.emplace_back();
        framebuffer.resize(textureWidth, textureHeight);
    }

    std::cout << "LayeredMirror initialized with " << maxLevels << " levels at (" << position.x << ", " << position.y
              << ", " << position.z << ")" << ", normal=(" << normal.x << ", " << normal.y << ", " << normal.z << ")"
              << " with resolution " << textureWidth << "x" << textureHeight << std::endl;
}

void LayeredMirror::update(const glm::vec3 &cameraPos) {
    if (!active)
        return;
    updateMatrices(cameraPos);
}

void LayeredMirror::updateMatrices(const glm::vec3 &cameraPos) {
    // Calculate mirror camera position (reflected camera position)
    glm::vec3 cameraToMirror = position - cameraPos;
    float distanceToMirror = glm::dot(cameraToMirror, normal);
    reflectedCameraPos = cameraPos + 2.0f * distanceToMirror * normal;

    // Calculate mirror view matrix
    glm::vec3 mirrorForward = -normal;
    glm::vec3 mirrorRight = glm::normalize(glm::cross(mirrorForward, upVector));
    glm::vec3 mirrorUp = glm::cross(mirrorRight, mirrorForward);

    mirrorView = glm::lookAt(reflectedCameraPos, reflectedCameraPos + mirrorForward, mirrorUp);

    // Calculate frustum bounds
    glm::vec3 halfRight = 0.5f * mirrorWidth * mirrorRotation[0];
    glm::vec3 halfUp = 0.5f * mirrorHeight * mirrorRotation[1];

    glm::vec3 p0 = position - halfRight - halfUp;
    glm::vec3 p2 = position + halfRight + halfUp;

    glm::vec4 p0_view = mirrorView * glm::vec4(p0, 1.0f);
    glm::vec4 p2_view = mirrorView * glm::vec4(p2, 1.0f);

    float nearPlane = distanceToMirror;
    float farPlane = 1000.0f;
    float left = p0_view.x;
    float bottom = p0_view.y;
    float right = p2_view.x;
    float top = p2_view.y;

    mirrorProjection = glm::frustum(left, right, bottom, top, nearPlane, farPlane);
}

void LayeredMirror::beginMirrorRender(int level, int screenWidth, int screenHeight) {
    if (!active || level >= maxLevels)
        return;

    framebuffers[level].bind(0, 0, screenWidth, screenHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    currentLevel = level;
}

void LayeredMirror::endMirrorRender(int level) {
    if (!active || level >= maxLevels)
        return;
    framebuffers[level].unbind();
}

bool LayeredMirror::shouldRender(const glm::vec3 &cameraPos) const {
    if (!active)
        return false;

    float distance = glm::length(position - cameraPos);
    if (distance > renderDistance)
        return false;

    glm::vec3 cameraToMirror = glm::normalize(position - cameraPos);
    float dot = glm::dot(cameraToMirror, normal);

    return dot < 0.0f;
}

void LayeredMirror::draw(const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection, int level) {
    validateLevel("draw", level);

    shader.use();
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", mainProjection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4(mirrorRotation);
    model = glm::scale(model, glm::vec3(mirrorWidth, mirrorHeight, 1.0f));
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffers[level].getTexture());
    shader.setInt("mirrorTexture", 0);

    quad.draw(shader);
}

void LayeredMirror::drawLevel(int level, const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection) {
    draw(viewMatrix, mainProjection, level);
}

GLuint LayeredMirror::getTexture(int level) const {
    validateLevel("getTexture", level);
    return framebuffers[level].getTexture();
}

void LayeredMirror::setCurrentLevel(int level) {
    validateLevel("setCurrentLevel", level);
    currentLevel = level;
}

void LayeredMirror::copyLevel(int fromLevel, int toLevel) {
    validateLevel("copyLevel-fromLevel", fromLevel);
    validateLevel("copyLevel-toLevel", toLevel);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[fromLevel].getFBO());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[toLevel].getFBO());
    glBlitFramebuffer(0, 0, framebuffers[fromLevel].getWidth(), framebuffers[fromLevel].getHeight(), 0, 0,
                      framebuffers[toLevel].getWidth(), framebuffers[toLevel].getHeight(), GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LayeredMirror::swapLevels(int level1, int level2) {
    throw std::runtime_error("swap not implemented!");
    // validateLevel("swapLevels-level1", level1);
    // validateLevel("swapLevels-level2", level2);
    // std::swap(framebuffers[level1], framebuffers[level2]);
}

void LayeredMirror::clearLevel(int level) {
    validateLevel("clearLevel", level);

    // TODO: Consider getting these from somewhere else
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    framebuffers[level].bind(viewport[0], viewport[1], viewport[2], viewport[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    framebuffers[level].unbind();
}

void LayeredMirror::clearAllLevels() {
    for (int i = 0; i < maxLevels; ++i) {
        clearLevel(i);
    }
}

void LayeredMirror::validateLevel(const std::string &funcName, int level) const {
    if (level < 0 || level >= maxLevels) {
        throw std::runtime_error("LayeredMirror::" + funcName + " - Invalid level " + std::to_string(level) +
                                 ". Maximum allowed level is " + std::to_string(maxLevels - 1) + ".");
    }
}