#include "SkyBox.h"

SkyBox::SkyBox(const std::string &basePath) : shader("shaders/skybox.vs", "shaders/skybox.fs") {
    std::vector<std::string> faces{basePath + "/right.jpg",  basePath + "/left.jpg",  basePath + "/top.jpg",
                                   basePath + "/bottom.jpg", basePath + "/front.jpg", basePath + "/back.jpg"};
    cubeMap.loadTexture(faces);

    shader.use();
    shader.setInt("skybox", 0);
}

void SkyBox::draw(const glm::mat4 &viewMatrix, const glm::mat4 &projection) {
    glDepthFunc(GL_LEQUAL);
    shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(viewMatrix)); // remove translation from the view matrix
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    cubeMap.bind(0);
    cube.draw(shader);

    glDepthFunc(GL_LESS);
}
