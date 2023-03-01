#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Screen width and height variables
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

// Functions declarations

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// A callback function on the window that gets called each time the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// The function takes the window as input together with a key and returns whether this key is currently being pressed
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Shaders

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.9f, 0.6f, 0.4f, 1.0f);\n"
"}\n\0";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check if there is any error occurred while compiling the vertex shader
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader failed" << std::endl << infoLog << std::endl;
    }

    // Build and compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check if there is any error occurred while compiling the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader failed" << std::endl << infoLog << std::endl;
    }

    // Build a shader program and link the vertex shader and the fragment shader to it
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check if there is any error occurred while compiling the shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed" << std::endl << infoLog << std::endl;
    }

    // Delete the shaders as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set the vertices coordinates. Each line represents a vertex (x, y and z)
    float vertices[] =
    {
        // First rectangle
        -0.25f,  0.5f, 0.0f, // top right
        -0.25f, -0.5f, 0.0f, // bottom right
        -0.75f, -0.5f, 0.0f, // bottom left
        -0.75f,  0.5f, 0.0f, // top left

        // Second rectangle
        0.25f,  0.25f, 0.0f, // top right
        0.25f, -0.25f, 0.0f, // bottom right
        -0.25f, -0.25f, 0.0f, // bottom left
        -0.25f,  0.25f, 0.0f, // top left

        // Third rectangle
        0.75f,  0.5f, 0.0f, // top right
        0.75f, -0.5f, 0.0f, // bottom right
        0.25f, -0.5f, 0.0f, // bottom left
        0.25f,  0.5f, 0.0f, // top left
    };
    // Set the indices that according to them the vertices will be scanned
    unsigned int indices[] =
    {
        0, 1, 3, // First triangle
        1, 2, 3, // Second triangle

        4, 5, 7, // Third triangle
        5, 6, 7, // Fourth triangle

        8, 9, 11, // Fifth triangle
        9, 10, 11 // Sixth triangle
    };
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the VAO
    glBindVertexArray(VAO);
    // Bind the VBO and configure vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind the EBO and configure the index attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // In order to draw the rectangles (triangles) as lines (not filled) uncomment the below line
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Start while loop so the window doesn't shut down immediately until it recieve explicit order
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Set the color of the screen
        glClearColor(0.0f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Give order to use the shader program so we start drawing to the screen
        glUseProgram(shaderProgram);
        // Bind the VAO
        glBindVertexArray(VAO);
        // Draw the rectangles using the EBO. Notice there is 3 rectangles which represented by 18 triangles
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete everything to prevent memory leaking
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Terminate the GLFW to prevent memory leaking
    glfwTerminate();
    return 0;
}