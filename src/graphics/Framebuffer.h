#pragma once

#include <glad/glad.h>

class Framebuffer {
  public:
    Framebuffer();
    ~Framebuffer();
    Framebuffer(const Framebuffer &) = delete;
    Framebuffer &operator=(const Framebuffer &) = delete;
    Framebuffer(Framebuffer &&) noexcept;
    Framebuffer &operator=(Framebuffer &&) = delete;

    void bind(int curX, int curY, int curWidth, int curHeight);
    void unbind();

    GLuint getTexture() const { return textureId; }
    GLuint getFBO() const { return fbo; }

    void resize(int newWidth, int newHeight);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

  private:
    int width;
    int height;
    GLint oldViewport[4] = {0};
    GLuint fbo;
    GLuint textureId;
    GLuint rbo;
};