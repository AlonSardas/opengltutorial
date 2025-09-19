#include "ScreenQuad.h"

ScreenQuad::ScreenQuad(float x, float y, float width, float height) {
    float quadVertices[] = {x, y + height, 0.0f, 1.0f, x,         y, 0.0f, 0.0f, x + width, y,          1.0f, 0.0f,
                            x, y + height, 0.0f, 1.0f, x + width, y, 1.0f, 0.0f, x + width, y + height, 1.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

ScreenQuad::~ScreenQuad() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ScreenQuad::setBounds(float x, float y, float width, float height) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    float quadVertices[] = {x, y + height, 0.0f, 1.0f, x,         y, 0.0f, 0.0f, x + width, y,          1.0f, 0.0f,
                            x, y + height, 0.0f, 1.0f, x + width, y, 1.0f, 0.0f, x + width, y + height, 1.0f, 1.0f};

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quadVertices), quadVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScreenQuad::draw(Shader &shader) {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}