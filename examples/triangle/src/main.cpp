#include <glrhi/glrhi.hpp>

int main()
{
    // Create the window
    glrhi::window window(1280, 720, "Hello Triangle");

    // Create and compile the shader
    glrhi::shader shader("../examples/triangle/shaders/basic.vert", "../examples/triangle/shaders/basic.frag");

    // Set the shader uniform for the triangle color
    shader.setVec3("color", glm::vec3(1.0f, 0.0f, 1.0f));

    // Triangle vertices
    GLfloat vertices[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    // Triangle indices
    GLuint indices[] = {
        0, 1, 2
    };

    // We load vertices and indices into their respective buffers
    glrhi::vbo VBO(vertices, sizeof(vertices));
    glrhi::ebo EBO(indices, sizeof(indices));

    // Create a vao and set the attribute settings
    glrhi::vao VAO;
    VAO.addAttribute(3, GL_FLOAT, GL_FALSE, 0);

    // Init the VAO and link the VBO and EBO to it
    VAO.init(VBO, EBO, 3*sizeof(GLfloat));

    while (!window.shouldClose())
    {
        // Pool the events from the window
        window.poolEvents();

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // In this example we dont really need to rebind every time, but in more complex systems you have to
        shader.use();
        VAO.bind();

        // Issue a draw call for the triangle
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
 
        // Swap the back buffer with the front buffer
        window.swapBuffers();
    }

    // No need for manually deallocating resources (everything is in the destructors)

    return 0;
}