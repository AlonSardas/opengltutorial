#include "Mirror.h"
#include <iostream>

Mirror::Mirror()
    : position(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 1.0f), upVector(0.0f, 1.0f, 0.0f), mirrorWidth(1.0f),
      mirrorHeight(1.0f), renderDistance(50.0f), active(true),
      shader("shaders/mirrorTransformer.vs", "shaders/mirrorTexture.fs") {}

void Mirror::init(const glm::vec3 &pos, const glm::vec3 &norm, float width, float height, int textureWidth,
                  int textureHeight) {
    position = pos;
    normal = glm::normalize(norm);
    mirrorWidth = width;
    mirrorHeight = height;

    // Orient the mirror quad according to its normal
    // Create rotation matrix to align quad with mirror normal
    glm::vec3 forward = -normal; // Mirror faces opposite to normal
    glm::vec3 right = glm::normalize(glm::cross(upVector, forward));
    glm::vec3 up = glm::cross(forward, right);
    mirrorRotation = glm::mat3(right, up, forward);

    framebuffer.resize(textureWidth, textureHeight);

    std::cout << "Mirror initialized at (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << ", normal=(" << normal.x
              << ", " << normal.y << ", " << normal.z << ")" << "with resolution " << textureWidth << "x"
              << textureHeight << std::endl;
}

void Mirror::update(const glm::vec3 &cameraPos, const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection) {
    if (!active) {
        return;
    }

    updateMatrices(cameraPos, viewMatrix, mainProjection);
}

void Mirror::updateMatrices(const glm::vec3 &cameraPos, const glm::mat4 &viewMatrix, const glm::mat4 &mainProjection) {
    shader.use();
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", mainProjection);

    // Calculate mirror camera position (reflected camera position)
    glm::vec3 cameraToMirror = position - cameraPos;
    float distanceToMirror = glm::dot(cameraToMirror, normal);
    glm::vec3 reflectedCameraPos = cameraPos + 2.0f * distanceToMirror * normal;

    std::cout << "reflected pos (" << reflectedCameraPos.x << ", " << reflectedCameraPos.y << ", "
              << reflectedCameraPos.z << ")" << std::endl;

    // Calculate mirror view matrix (looking in opposite direction of normal)
    glm::vec3 mirrorForward = -normal;
    glm::vec3 mirrorRight = glm::normalize(glm::cross(mirrorForward, upVector));
    glm::vec3 mirrorUp = glm::cross(mirrorRight, mirrorForward);
    // std::cout << "Mirror up (" << mirrorUp.x << ", " << mirrorUp.y << ", " << mirrorUp.z << ")" << std::endl;

    // mirrorView = glm::lookAt(reflectedCameraPos, position, mirrorUp);
    mirrorView = glm::lookAt(reflectedCameraPos, reflectedCameraPos + mirrorForward, mirrorUp);

    glm::vec3 halfRight = 0.5f * mirrorWidth * mirrorRotation[0]; // right vector
    glm::vec3 halfUp = 0.5f * mirrorHeight * mirrorRotation[1];   // up vector
    // 4 corners in world space (clockwise)
    glm::vec3 p0 = position - halfRight - halfUp;
    glm::vec3 p1 = position + halfRight - halfUp;
    glm::vec3 p2 = position + halfRight + halfUp;
    glm::vec3 p3 = position - halfRight + halfUp;

    glm::vec4 p0_view = mirrorView * glm::vec4(p0, 1.0f);
    glm::vec4 p1_view = mirrorView * glm::vec4(p1, 1.0f);
    glm::vec4 p2_view = mirrorView * glm::vec4(p2, 1.0f);
    glm::vec4 p3_view = mirrorView * glm::vec4(p3, 1.0f);

    float nearPlane = distanceToMirror;
    float farPlane = 1000.0f;
    float left = p0_view.x;
    float bottom = p0_view.y;
    float right = p2_view.x;
    float top = p2_view.y;

    std::cout << "near=" << nearPlane << ". left=" << left << ", right=" << right << ", bottom=" << bottom
              << ", top=" << top << std::endl;

    mirrorProjection = glm::frustum(left, right, bottom, top, nearPlane, farPlane);
    // mirrorProjection = glm::perspective(fov, mirrorAspect, 0.1f, 1000.0f);
    // float fov = 2.0f * atan(mirrorHeight * 0.5f / distanceToMirror);
    glm::mat4 horizontalFlip = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));
    // mirrorProjection = horizontalFlip * glm::perspective(fov, mirrorAspect, distanceToMirror, 1000.0f);
}

void Mirror::beginMirrorRender(int screenWidth, int screenHeight) {
    if (!active)
        return;

    // Bind framebuffer and set viewport
    framebuffer.bind(0, 0, screenWidth, screenHeight);

    // Clear mirror framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable face culling flip for mirror reflection
    // glFrontFace(GL_CW); // Flip winding order for reflection
}

void Mirror::endMirrorRender() {
    if (!active)
        return;

    // Restore normal face culling
    // glFrontFace(GL_CCW);

    // Unbind framebuffer
    framebuffer.unbind();
}

bool Mirror::shouldRender(const glm::vec3 &cameraPos) const {
    if (!active)
        return false;

    // Distance culling
    float distance = glm::length(position - cameraPos);
    if (distance > renderDistance)
        return false;

    // Frustum culling - check if camera is facing the mirror
    glm::vec3 cameraToMirror = glm::normalize(position - cameraPos);
    float dot = glm::dot(cameraToMirror, normal);

    // Only render if camera is in front of mirror (not behind it)
    return dot < 0.0f; // Mirror normal points towards viewer
}

void Mirror::draw() {
    if (!active)
        return;

    shader.use();

    // Set model matrix to position and scale the quad
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = model * glm::mat4(mirrorRotation);
    model = glm::scale(model, glm::vec3(mirrorWidth, mirrorHeight, 1.0f));
    shader.setMat4("model", model);

    // Bind mirror texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer.getTexture());
    shader.setInt("mirrorTexture", 0);

    quad.draw(shader);
}
