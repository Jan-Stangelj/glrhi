#include "glrhi/glrhi.hpp"

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

    
	GLuint FBO;
	glCreateFramebuffers(1, &FBO);

	glrhi::texture2D fbTex(false);
    fbTex.loadEmpty(1280, 720, GL_RGB8);
	glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, fbTex.getID(), 0);

    GLuint rbo;
    glCreateRenderbuffers(1, &rbo);
    glNamedRenderbufferStorage(rbo, GL_DEPTH_COMPONENT24, 1280, 720);
    glNamedFramebufferRenderbuffer(FBO, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    GLenum drawBuf[] = { GL_COLOR_ATTACHMENT0 };
    glNamedFramebufferDrawBuffers(FBO, 1, drawBuf);

	auto fboStatus = glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glEnable(GL_DEPTH_TEST);

    while (!window.shouldClose())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        VAO.bind();
        
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        buffer.use();
        quadVAO.bind();
        fbTex.bind(0, buffer, "screen");

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}