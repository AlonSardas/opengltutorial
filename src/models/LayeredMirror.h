#pragma once

#include "graphics/Framebuffer.h"
#include "primitives/QuadNoNormals.h"
#include <glm/glm.hpp>
#include <vector>

class LayeredMirror {
  private:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 upVector;
    float mirrorWidth;
    float mirrorHeight;
    float renderDistance;
    bool active;

    glm::mat3 mirrorRotation;
    glm::vec3 reflectedCameraPos;
    glm::mat4 mirrorView;
    glm::mat4 mirrorProjection;

    Shader shader;
    QuadNoNormals quad;

    // Multiple framebuffers for different recursion levels
    std::vector<Framebuffer> framebuffers;
    int currentLevel;
    int maxLevels;

    void init(int textureWidth, int textureHeight);
    void updateMatrices(const glm::vec3 &cameraPos);
    void validateLevel(const std::string &funcName, int level) const;

  public:
    LayeredMirror(const glm::vec3 &pos, const glm::vec3 &norm, float width, float height, int textureWidth,
                  int textureHeight, int levels = 3);

    // Core functionality
    void update(const glm::vec3 &cameraPos);
    bool shouldRender(const glm::vec3 &cameraPos) const;

    // Level-specific rendering
    void beginMirrorRender(int level, int screenWidth, int screenHeight);
    void endMirrorRender(int level);
    void beginMirrorRender(int screenWidth, int screenHeight) { beginMirrorRender(0, screenWidth, screenHeight); }

    // Drawing with specific level
    void draw(const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection, int level = 0);
    void drawLevel(int level, const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection);

    // Getters
    const glm::mat4 &getViewMatrix() const { return mirrorView; }
    const glm::mat4 &getProjectionMatrix() const { return mirrorProjection; }
    const glm::vec3 &getReflectedCameraPos() const { return reflectedCameraPos; }
    const glm::vec3 &getPosition() const { return position; }
    const glm::vec3 &getNormal() const { return normal; }

    GLuint getTexture(int level = 0) const;
    int getMaxLevels() const { return maxLevels; }

    // Level management
    void setCurrentLevel(int level);
    int getCurrentLevel() const { return currentLevel; }

    // Utility
    void copyLevel(int fromLevel, int toLevel);
    void swapLevels(int level1, int level2);
    void clearLevel(int level);
    void clearAllLevels();
};
