#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
  public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char *vertexPath, const char *fragmentPath, const char* geometryPath = nullptr);
    ~Shader();
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;
    Shader(Shader &&) = delete;
    Shader &operator=(Shader &&) = delete;

    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const { setVec3(name, value.x, value.y, value.z); }
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    GLint getUniformLocation(const std::string &name) const;

    bool usesTexture() const { return usesTex; }
    void setUsesTexture(bool uses) { usesTex = uses; }

  private:
    void checkCompileErrors(const unsigned int &shader, const std::string &type);
    void checkLinkingErrors();
    std::string readFile(const char *filePath);

    bool usesTex = true;
};
