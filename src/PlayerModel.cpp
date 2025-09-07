#include "PlayerModel.h"
#include <glm/gtc/matrix_transform.hpp>

PlayerModel::PlayerModel(const Player *player) : player(player) {
    body.setTexture("resources/container.jpg");
    head.setTexture("resources/awesomeface.png");
}

void PlayerModel::draw(Shader &shader) {
    glm::mat4 model = glm::translate(glm::mat4(1.0f), player->getPosition());
    model = glm::scale(model, glm::vec3(0.9f, 2.0f, 0.5f));
    shader.setMat4("model", model);
    body.draw(shader);

    glm::mat4 headModel = glm::translate(glm::mat4(1.0f), player->getPosition());
    headModel = glm::translate(headModel, glm::vec3(0.0f, 1.5f, 0.0f));
    const glm::vec3 &front = player->getFront();
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 up = glm::normalize(glm::cross(right, front));
    glm::mat4 rotation(1.0f);
    rotation[0] = glm::vec4(right, 0.0f);
    rotation[1] = glm::vec4(up, 0.0f);
    rotation[2] = glm::vec4(front, 0.0f);
    headModel = headModel * rotation;
    shader.setMat4("model", headModel);
    head.draw(shader);
}
