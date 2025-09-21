#include "IDrawable.h"
#include "graphics/Texture.h"

class TexturedObject : public IDrawable {
  public:
    TexturedObject(const IDrawable *inner, const std::string &uniformName, unsigned char r, unsigned char g,
                   unsigned char b, unsigned char a);
    TexturedObject(const IDrawable *inner, const std::string &uniformName, const std::string &texturePath,
                   GLint wrapS = GL_CLAMP_TO_EDGE, GLint wrapT = GL_CLAMP_TO_EDGE);
    void draw(Shader &shader) const override;

  private:
    const IDrawable *drawable;
    Texture texture;
    std::string uniformName;
};