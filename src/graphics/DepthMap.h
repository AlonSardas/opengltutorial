#pragma once

#include <glad/glad.h>

class DepthMap {
  public:
    DepthMap();
    ~DepthMap();
    DepthMap(const DepthMap &) = delete;
    DepthMap &operator=(const DepthMap &) = delete;
    DepthMap(DepthMap &&) = delete;
    DepthMap &operator=(DepthMap &&) = delete;

    void bind(int curX, int curY, int curWidth, int curHeight);
    void unbind();
    void clear() { glClear(GL_DEPTH_BUFFER_BIT); }

    GLuint getTexture() const { return textureId; }
    GLuint getFBO() const { return fbo; }

    void resize(int newWidth, int newHeight);
    int getWidth() const { return width; }
    int getHeight() const { return height; }

  protected:
    virtual void generateTexture();
    virtual void bindTexture() {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
    }

    int width;
    int height;
    GLint oldViewport[4] = {0};
    GLuint fbo;
    GLuint textureId;
};