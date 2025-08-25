#include "Cube.h"
#include "CubeData.h"

const std::array<float, 288> Cube::vertices = getCubeVerticesWithNormalsAndTexture();

int cubeCount = 0;

Cube::Cube() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::setTexture(const std::string &texturePath) {
    texture.emplace();
    texture->bind(0);
    texture->setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->loadImage(texturePath.c_str(), true);
}

void Cube::draw(Shader &shader) {
    if (texture) {
        // shader.setInt("texture_diffuse1", 0);
        texture->bind(0);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
