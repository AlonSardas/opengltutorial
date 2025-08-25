#include "Quad.h"
#include <GLFW/glfw3.h>

const std::array<float, 30> Quad::vertices = {
    // First triangle
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // Top-left
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom-left
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Bottom-right

    // Second triangle
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top-left
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f   // Top-right
};

Quad::Quad() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Quad::~Quad() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Quad::draw(Shader &shader) const {
    shader.use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}