#include "Renderer.h"

Renderer::Renderer(const QuatCamera &c)
    : shader("shaders/vertexTransformer.vs", "shaders/textureMergeFragment.fs"),
      camera(c) {}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);
}

void Renderer::onResize(int width, int height) {
    // std::cout << "resized!" << width << "," << height << std::endl;
    glViewport(0, 0, width, height);
    aspectRatio = float(width) / height;
    updateProjectionMatrix();
}

void Renderer::adjustFov(float deltaDegrees) {
    fov += deltaDegrees;
    if (fov < MIN_FOV)
        fov = MIN_FOV;
    if (fov > MAX_FOV)
        fov = MAX_FOV;
    updateProjectionMatrix();
}

void Renderer::updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    // projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, -10.0f, 100.0f);
    shader.setMat4("projection", projection);
}

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);
    model =
        glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where
    // we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    // TODO: Fix so aspect is preserved when changing size
    // TODO: Fix the resize issue
    // TODO: Check what happen without this
    // projection =
    // glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    // projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 300.0f);

    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);

    /*
    float vertices[] = {
        // positions          // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // top left
    };
    */
    const auto vertices = Renderer::getCubeVertices();
    // std::cout << sizeof(vertices) << vertices.data()[0] << std::endl;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                 GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    //              GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture
    // -------------------------
    texture1.bind(0);
    texture1.setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture1.setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture1.loadImage("resources/container.jpg", false, false);
    texture2.bind(1);
    texture2.setWrap(GL_REPEAT, GL_REPEAT);
    texture2.setMinMagFilters(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    // texture2.loadImage("resources/awesomeface.png", true, true);
    texture2.loadImage("resources/Pitz1.jpeg", false, true);

    shader.setFloat("mixRatio", 1.0f);
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
}

void Renderer::render() {
    glClearColor(1.0f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.setMat4("view", camera.getViewMatrix());

    glBindVertexArray(VAO);
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    auto cubePositions = getCubePositions();
    for (unsigned int i = 0; i < cubePositions.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle),
                            glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

const std::array<glm::vec3, 10> &Renderer::getCubePositions() {
    static const std::array<glm::vec3, 10> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    return cubePositions;
}

const std::array<float, 180> &Renderer::getCubeVertices() {
    static const std::array<float, 180> vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};
    return vertices;
}