#include "debugUtils.h"
#include <iomanip>
#include <iostream>

void printMat4(const std::string &name, const glm::mat4 &mat) {
    std::cout << name << std::endl;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << std::setw(10) << mat[col][row] << " ";
        }
        std::cout << std::endl;
    }
}

void printVec4(const std::string &name, const glm::vec4 &v) {
    std::cout << name << ": ";
    for (int i = 0; i < 4; ++i) {
        std::cout << std::setw(10) << v[i] << " ";
    }
    std::cout << std::endl;
}

void printVec3(const std::string &name, const glm::vec3 &v) {
    std::cout << name << ": ";
    for (int i = 0; i < 3; ++i) {
        std::cout << std::setw(10) << v[i] << " ";
    }
    std::cout << std::endl;
}
