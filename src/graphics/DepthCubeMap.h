// Useful for point light shadow maps
#pragma once

#include "DepthMap.h"

class DepthCubeMap : public DepthMap {
protected:
    void generateTexture() override;
    void bindTexture() override;
};