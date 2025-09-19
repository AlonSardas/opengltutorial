#pragma once

#include "graphics/CubeMap.h"
#include "graphics/Shader.h"
#include "models/primitives/Cube.h"
#include <string>

class SkyBox {
  public:
    SkyBox(const std::string &basePath);
    ~SkyBox() = default;
    SkyBox(const SkyBox &) = delete;
    SkyBox &operator=(const SkyBox &) = delete;
    SkyBox(SkyBox &&) = delete;
    SkyBox &operator=(SkyBox &&) = delete;

    void draw(const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection);

  private:
    Shader shader;
    CubeMap cubeMap;
    Cube cube;
};