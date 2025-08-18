#pragma once

class IRenderer {
  public:
    virtual ~IRenderer() = default;
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void onResize(int width, int height) = 0;
};