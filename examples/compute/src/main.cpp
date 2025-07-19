#include "glrhi/glrhi.hpp"

int main()
{
    glrhi::window window(1280, 720, "Compute example");

    glrhi::shader buffer("../examples/compute/shaders/buffer.vert", "../examples/compute/shaders/buffer.frag");

    glrhi::compute rainbow("../examples/compute/shaders/compute.comp");

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

    glrhi::texture2D colorTex(1280, 720, GL_RGBA8);
    rainbow.setHandle64("imgOutput", colorTex.getImageHandle());

    while (!window.shouldClose())
    {

        rainbow.dispatch(1280, 720, 1);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        buffer.use();
        quadVAO.bind();
        buffer.setHandle64("screen", colorTex.getSamplerHandle());

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        window.swapBuffers();
    }

    return 0;
}