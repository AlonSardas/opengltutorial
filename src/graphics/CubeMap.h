#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

class CubeMap {
  public:
    CubeMap();
    ~CubeMap();
    CubeMap(const CubeMap &) = delete;
    CubeMap &operator=(const CubeMap &) = delete;
    CubeMap(CubeMap &&) = delete;
    CubeMap &operator=(CubeMap &&) = delete;

    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    // -------------------------------------------------------
    void loadTexture(const std::vector<std::string> &faces);
    void bind(int textureUnit);
    GLuint getTexture() const { return textureId; }

  private:
    GLuint textureId;
};