#pragma once

#include "stb/stb_image.h"
#include <glad/glad.h>
#include <stdexcept>

class Texture {
  public:
    Texture();
    ~Texture();
    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;
    Texture(Texture &&) = delete;
    Texture &operator=(Texture &&) = delete;

    void setWrap(GLint wrapX, GLint wrapY);
    void setMinMagFilters(GLint minFilter, GLint magFilter);
    void bind(int textureUnit) const;
    void loadImage(const char *filePath, bool flipVertically);
    inline GLuint getID() const { return id; }

  private:
    GLuint id;
    void setWrapX(GLint param);
    void setWrapY(GLint param);
};