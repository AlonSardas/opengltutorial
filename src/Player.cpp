#include "Player.h"
#include "utils/debugUtils.h"
#include <glm/gtc/matrix_transform.hpp>

void Player::updateViewMatrix() {
    glm::vec3 cameraOffset = cameraBase + 1.5f * up - 4.0f * front;

    viewMatrix = glm::lookAt(position + cameraOffset, position + cameraOffset + front, worldUp);
}
