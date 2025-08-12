#pragma once

#include "stb_image.h"
#include <glad/glad.h>
#include <stdexcept>

class Texture {
  public:
    Texture();
    ~Texture();
    void setWrap(GLint wrapX, GLint wrapY);
    void setMinMagFilters(GLint minFilter, GLint magFilter);
    void bind(int textureUnit);
    void loadImage(const char *filePath, bool flipVertically);
    inline GLuint getID() const { return id; }

  private:
    GLuint id;
    void setWrapX(GLint param);
    void setWrapY(GLint param);
};