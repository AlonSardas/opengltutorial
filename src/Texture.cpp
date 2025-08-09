#include "Texture.h"
#include "stb_image.h"
#include <stdexcept>

Texture::Texture() { glGenTextures(1, &id); }

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind(int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::setWrap(GLint wrapX, GLint wrapY) {
    setWrapX(wrapX);
    setWrapY(wrapY);
}

void Texture::setWrapX(GLint param) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
}

void Texture::setWrapY(GLint param) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
}

void Texture::setMinMagFilters(GLint minFilter, GLint magFilter) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::loadImage(const char *filePath, bool withAlpha,
                        bool flipVertically) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if (data == nullptr) {
        throw std::runtime_error("Could not load image " +
                                 std::string(filePath));
    }
    GLenum rgba = withAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgba,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
}
