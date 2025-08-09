#pragma once

#include <glad/glad.h>

class Texture {
  public:
    Texture();
    ~Texture();
    void setWrap(GLint wrapX, GLint wrapY);
    void setMinMagFilters(GLint minFilter, GLint magFilter);
    void bind(int textureUnit);
    void loadImage(const char *filePath, bool withAlpha, bool flipVertically);
    inline GLuint getID() const { return id; }

  private:
    GLuint id;
    void setWrapX(GLint param);
    void setWrapY(GLint param);
};