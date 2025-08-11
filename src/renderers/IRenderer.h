#pragma once

class IRenderer {
  public:
    virtual ~IRenderer() = default; // inline virtual destructor
    virtual void init() = 0;
    virtual void render() = 0;
    virtual void onResize(int width, int height) = 0;
};