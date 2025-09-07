#pragma once

#include "POVPlayer.h"

class Player : public POVPlayer {
  protected:
    void updateViewMatrix() override;
};