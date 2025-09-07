#pragma once

#include "IControllable.h"
#include "models/Portal.h"
#include <unordered_map>

class PortalTraversable {
  public:
    PortalTraversable(IControllable *player) : player(player) {}

    bool checkPortals();
    void addPortal(const Portal *portal) { portalsNormals[portal] = -1.0f; }

  protected:
    IControllable *const player;
    std::unordered_map<const Portal *, float> portalsNormals;
};