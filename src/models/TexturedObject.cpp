#include "TexturedObject.h"

TexturedObject::TexturedObject(const IDrawable *inner, const std::string &texturePath, const std::string &uniformName)
    : drawable(inner), uniformName(uniformName) {
    texture.bind(0);
    texture.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
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
