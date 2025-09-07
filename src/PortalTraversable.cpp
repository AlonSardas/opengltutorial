#include "PortalTraversable.h"
#include "utils/debugUtils.h"

bool PortalTraversable::checkPortals() {
    for (auto &[portal, lastNormal] : portalsNormals) {
        const glm::vec3 &entityPosition = player->getPosition();
        glm::vec3 toPortal = portal->getPosition() - entityPosition;
        float currentNormal = glm::dot(toPortal, portal->getNormal());

        if (lastNormal <= 0 && currentNormal > 0) {
            if (portal->isInside(entityPosition)) {

                // Teleport through the portal
                if (portal->getDestination()) {
                    std::cout << "Crossed portal " << portal->getName() << std::endl;
                    std::cout << "teleporting from ";
                    printVec3("pos1", entityPosition);
                    player->setPosition(portal->teleportPosition(entityPosition));
                    std::cout << "to ";
                    printVec3("pos2", player->getPosition());

                    const glm::vec3 &entityFront = player->getFront();
                    glm::vec4 forward4(entityFront, 0.0f);
                    glm::vec3 newForward = portal->teleportVec(forward4);

                    // front = newForward;
                    // right = glm::normalize(glm::cross(front, worldUp));
                    // up = glm::normalize(glm::cross(right, front));
                    float yaw = glm::degrees(atan2(newForward.z, newForward.x));
                    float pitch = glm::degrees(asin(newForward.y));
                    player->setRotation(yaw, pitch, 0.0f);

                    return true;

                    // front = glm::normalize(newForward);
                    // right = glm::normalize(glm::cross(front, worldUp));
                    // up = glm::normalize(glm::cross(right, front));
                    // Recalculate yaw and pitch from new front vector

                    //     const Portal *dest = portal->destination;
                    //     glm::vec3 localPos = getPosition() - portal->position;

                    //     // Rotate 180 degrees around the up axis of the portal
                    //     glm::vec3 up(0.0f, 1.0f, 0.0f);
                    //     glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), up);
                    //     glm::vec3 rotatedPos = glm::vec3(rotation * glm::vec4(localPos, 1.0f));

                    //     // New position relative to destination portal
                    //     glm::vec3 newPos = dest->position + rotatedPos;

                    //     // Update position
                    //     setPosition(newPos);

                    //     // Adjust orientation to match destination portal
                    //     glm::vec3 forward = getFront();
                    //     glm::vec3 right = glm::normalize(glm::cross(forward, up));
                    //     glm::mat4 orientRotation = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), up);
                    //     glm::vec3 newForward = glm::vec3(orientRotation * glm::vec4(forward, 0.0f));
                    //     float yaw = atan2(newForward.z, newForward.x) * 180.0f / 3.14159265f - 90.0f;
                    //     float pitch = asin(newForward.y) * 180.0f / 3.14159265f;
                    //     setRotation(yaw, pitch, 0.0f);
                }
            }
        }
        lastNormal = currentNormal;
    }

    return false;
}