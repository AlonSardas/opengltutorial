#include "TexturedObject.h"

TexturedObject::TexturedObject(const IDrawable *inner, const std::string &uniformName, unsigned char r, unsigned char g,
                               unsigned char b, unsigned char a)
    : drawable(inner), uniformName(uniformName) {
    texture.bind(0);
    texture.loadSolidColor(r, g, b, a);
}

TexturedObject::TexturedObject(const IDrawable *inner, const std::string &uniformName, const std::string &texturePath,
                               GLint wrapS, GLint wrapT)
    : drawable(inner), uniformName(uniformName) {
    texture.bind(0);
    texture.setWrap(wrapS, wrapT);
    texture.setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture.loadImage(texturePath.c_str(), true);
}

void TexturedObject::draw(Shader &shader) const {
    if (shader.usesTexture()) {
        texture.bind(0);
        shader.setInt(uniformName, 0);
    }
    drawable->draw(shader);
}
