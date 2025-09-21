#include "Sphere.h"
#include <cassert>
#include <iostream>

Sphere::Sphere(unsigned int latitudeBands, unsigned int longitudeBands, float radius) : VAO(0), VBO(0), EBO(0) {
    assert(latitudeBands >= 3 && longitudeBands >= 3 && "Sphere must have at least 3 latitude and longitude bands");
    assert(radius > 0.0f && "Sphere radius must be positive");

    generateVerticesAndIndices(latitudeBands, longitudeBands, radius);
    // std::cout << "total vertices " << vertices.size() << ", total indices " << indices.size() << std::endl;
    // std::cout << "indices[0] " << indices.data()[0] << ", " << indices.data()[1] << ", " << indices.data()[10]
    //           << " total size" << indices.size() * sizeof(unsigned int) << std::endl;
    // printVertexData();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Index Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Sphere::draw(Shader &shader) const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::generateVerticesAndIndices(unsigned int latitudeBands, unsigned int longitudeBands, float radius) {
    unsigned int numVertices = (latitudeBands + 1) * (longitudeBands + 1);
    unsigned int numIndices = latitudeBands * longitudeBands * 6;

    vertices.reserve(numVertices * 8); // 8 floats per vertex
    indices.reserve(numIndices);

    for (unsigned int lat = 0; lat <= latitudeBands; ++lat) {
        float theta = lat * M_PI / latitudeBands;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int lon = 0; lon <= longitudeBands; ++lon) {
            float phi = lon * 2.0f * M_PI / longitudeBands;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            float u = 1.0f - ((float)lon / longitudeBands);
            float v = 1.0f - ((float)lat / latitudeBands);

            // Position
            vertices.push_back(radius * x);
            vertices.push_back(radius * y);
            vertices.push_back(radius * z);

            // Normal
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // TexCoords
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (unsigned int lat = 0; lat < latitudeBands; ++lat) {
        for (unsigned int lon = 0; lon < longitudeBands; ++lon) {
            unsigned int first = (lat * (longitudeBands + 1)) + lon;
            unsigned int second = first + longitudeBands + 1;

            // Two triangles per quad
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void Sphere::printVertexData() const {
    std::cout << "\n=== SPHERE DEBUG INFO ===" << std::endl;
    std::cout << "Total vertices: " << vertices.size() / 8 << std::endl;
    std::cout << "Total triangles: " << indices.size() / 3 << std::endl;

    // Print all vertices (use with caution on large spheres!)
    for (size_t i = 0; i < vertices.size(); i += 8) {
        size_t vertexIndex = i / 8;
        std::cout << "V" << vertexIndex << ": " << "pos(" << vertices[i] << ", " << vertices[i + 1] << ", "
                  << vertices[i + 2] << ") " << "norm(" << vertices[i + 3] << ", " << vertices[i + 4] << ", "
                  << vertices[i + 5] << ") " << "uv(" << vertices[i + 6] << ", " << vertices[i + 7] << ")" << std::endl;
    }

    std::cout << "\nIndices:" << std::endl;
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::cout << "T" << i / 3 << ": " << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2]
                  << std::endl;
    }
}
