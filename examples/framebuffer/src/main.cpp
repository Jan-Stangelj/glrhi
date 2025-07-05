#include "glrhi/glrhi.hpp"

#include <memory>

#include <iostream>

int main()
{
    glrhi::window window(1280, 720, "Framebuffer example");

    glrhi::shader shader("../examples/framebuffer/shaders/basic.vert", "../examples/framebuffer/shaders/basic.frag");
    shader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));

    glrhi::shader buffer("../examples/framebuffer/shaders/buffer.vert", "../examples/framebuffer/shaders/buffer.frag");

    float vertices[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2
    };
    
    glrhi::vao VAO;

    glrhi::vbo VBO(vertices, sizeof(vertices));
    glrhi::ebo EBO(indices, sizeof(indices));

    VAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
    VAO.init(VBO, EBO, 3*sizeof(float));

    float quad[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int quadIndices[] = {
        0, 1, 2,
        1, 2, 3
    };

    glrhi::vao quadVAO;

    glrhi::vbo quadVBO(quad, sizeof(quad));
    glrhi::ebo quadEBO(quadIndices, sizeof(quadIndices));

    quadVAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);
    quadVAO.addAttribute(2, GL_FLOAT, GL_FALSE, 3*sizeof(float));
    quadVAO.init(quadVBO, quadEBO, 5*sizeof(float));

    glrhi::texture2D colorTex = glrhi::texture2D(1280, 720, GL_RGBA8);
    glrhi::texture2D depthTex = glrhi::texture2D(1280, 720, GL_DEPTH_COMPONENT24);

    glrhi::fbo FBO;

    FBO.attachColorTexture(&colorTex);
    FBO.attachDepthTexture(&depthTex);

    FBO.init();

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose())
    {
        FBO.bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        VAO.bind();
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        FBO.unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        buffer.use();
        quadVAO.bind();
        colorTex.bind(0, buffer, "screen");

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}