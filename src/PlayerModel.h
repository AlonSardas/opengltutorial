#pragma once

#include "Player.h"
#include "graphics/Shader.h"
#include "models/IDrawable.h"
#include "models/primitives/Cube.h"
#include <glm/glm.hpp>

class PlayerModel : public IDrawable {
  public:
    PlayerModel(const Player *player);

    void draw(Shader &shader) const override;

  private:
    const Player *player;
    Cube body, head;
};