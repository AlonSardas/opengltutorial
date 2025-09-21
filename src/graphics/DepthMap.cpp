#include "DepthMap.h"
#include "Framebuffer.h"
#include <stdexcept>

DepthMap::DepthMap() : width(0), height(0), fbo(0), textureId(0) { glGenFramebuffers(1, &fbo); }

DepthMap::~DepthMap() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &textureId);
}

void DepthMap::resize(int newWidth, int newHeight) {
    if (newWidth == width && newHeight == height)
        return;
    width = newWidth;
    height = newHeight;

    glDeleteTextures(1, &textureId);
    generateTexture();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    bindTexture();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Check framebuffer completeness
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("DepthMap::framebuffer is not complete: " +
                                 Framebuffer::framebufferStatusToString(status));
    }

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthMap::generateTexture() {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void DepthMap::bind(int curX, int curY, int curWidth, int curHeight) {
    if (width == 0 || height == 0) {
        throw std::runtime_error("DepthMap::framebuffer used before initialization (call resize() first)");
    }
    oldViewport[0] = curX;
    oldViewport[1] = curY;
    oldViewport[2] = curWidth;
    oldViewport[3] = curHeight;
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
}

void DepthMap::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
}