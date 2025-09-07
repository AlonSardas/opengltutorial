#pragma once

#include "Player.h"
#include "graphics/Shader.h"
#include "models/Cube.h"
#include <glm/glm.hpp>

class PlayerModel {
  public:
    PlayerModel(const Player *player);

    void draw(Shader &shader);

  private:
    const Player *player;
    Cube body, head;
};