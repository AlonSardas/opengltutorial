#include "Texture.h"

Texture::Texture() { glGenTextures(1, &id); }

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind(int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setWrap(GLint wrapX, GLint wrapY) {
    setWrapX(wrapX);
    setWrapY(wrapY);
}

void Texture::setWrapX(GLint param) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param); }

void Texture::setWrapY(GLint param) { glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param); }

void Texture::setMinMagFilters(GLint minFilter, GLint magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::loadImage(const char *filePath, bool flipVertically) {
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (data == nullptr) {
        throw std::runtime_error("Could not load image " + std::string(filePath));
    }
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    else {
        throw std::runtime_error("Unsupported number of texture components: " + std::to_string(nrComponents));
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}

void Texture::loadSolidColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    unsigned char data[] = {r, g, b, a};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
