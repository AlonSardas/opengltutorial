#include "IDrawable.h"
#include "graphics/Texture.h"

class TexturedObject : public IDrawable {
  public:
    TexturedObject(const IDrawable *inner, const std::string &texturePath, const std::string &uniformName,
                   GLint wrapS = GL_CLAMP_TO_EDGE, GLint wrapT = GL_CLAMP_TO_EDGE);
    void draw(Shader &shader) const override;

  private:
    const IDrawable *drawable;
    Texture texture;
    std::string uniformName;
};