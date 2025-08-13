/*
https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*/

#include "Shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexCode = readFile(vertexPath);
        fragmentCode = readFile(fragmentPath);
    } catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        throw;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkLinkingErrors();

    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() { glUseProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
    GLint location = getUniformLocation(name);
    glUniform1i(location, (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const {
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const {
    GLint location = getUniformLocation(name);
    glUniform1f(location, value);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    GLint location = getUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

GLint Shader::getUniformLocation(const std::string &name) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        std::string errorMessage = "Uniform '" + name + "' not found or optimized out.";
        std::cout << errorMessage << std::endl;
        throw std::runtime_error(errorMessage);
    }
    return location;
}

void Shader::checkCompileErrors(const unsigned int &shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

    if (!success) {
        std::string errorMessage = "ERROR::SHADER::" + type + "::COMPILATION_FAILED\n";
        errorMessage += infoLog;
        throw std::runtime_error(errorMessage);
    } else if (strlen(infoLog) > 0) {
        std::cerr << "WARNING::SHADER::" << type << "::COMPILATION_LOG\n"
                  << infoLog << "\n"
                  << " -- --------------------------------------------------- -- " << std::endl;
    }
}

void Shader::checkLinkingErrors() {
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        std::string errorMessage;
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        errorMessage = std::string("ERROR::PROGRAM_LINKING_ERROR\n") + infoLog;
        std::cout << errorMessage << std::endl;
        throw std::runtime_error(errorMessage);
    }
}

std::string Shader::readFile(const char *filePath) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(filePath);

    std::ostringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}