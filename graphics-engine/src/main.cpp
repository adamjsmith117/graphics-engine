#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

GLFWwindow* window;
unsigned int shaderProgram;

const char* vertShaderSource = "#version 460 core\n"
  "layout(location = 0) in vec3 aPos;\n"
  "void main() {\n"
  "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

const char* fragShaderSource = "#version 460 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

/** called by GLFW on window resize to dynamically update our glViewport */
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void compileShader(unsigned int* shader) {
  glCompileShader(*shader);

  int  success;
  char infoLog[512];
  glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(*shader, 512, NULL, infoLog);
    std::cout << "ERROR: shader compilation failed\n" << infoLog << std::endl;
  }
}

void initializeShaderProgram() {
  // create and compile vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertShaderSource, NULL);
  compileShader(&vertexShader);

  // create and compile fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
  compileShader(&fragmentShader);

  // initialize shaderProgram and register + link shaders
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  int  success;
  char infoLog[512];
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR: shaderProgram linking failed." << std::endl;
  }

  // use this shaderProgram for this render
  glUseProgram(shaderProgram);

  // cleanup
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void drawShape() {
  

  
}

int initializeGLFW() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "graphics-engine", NULL, NULL);
  if (window == NULL) {
    std::cout << "ERROR: Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  return 0;
}

int initializeGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
    return -1;
  }
  return 0;
}

int main() {
  if (initializeGLFW() == -1) {
    return -1;
  }
  if (initializeGLAD() == -1) {
    return -1;
  }

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // set openGL viewport dimensions to match GLFW initial window dimensions
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // register callback fn to dynamically resize viewport when window changes size

  initializeShaderProgram();

  // render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // *******************************************************************************************************************************
    // *** VAO initialization ***
    float vertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };

    unsigned int VBO; // vertexBufferObject for sending large chunks of vertex data to the GPU
    glGenBuffers(1, &VBO); // assign our VBO an ID of 1

    unsigned int VAO; // you can think of VAOs as object wrappers for VBOs that encapsulate all relevant data + attribute data to make swapping stuff to be rendered easier
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // binding our VBO as the GL_ARRAY_BUFFER
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies vertices to the GL_ARRAY_BUFFER (which is currently bound to our VBO)

    // set dimensions of vert data being passed to shader so it knows how to interpret the buffer (and enable the attribArray)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // *** IN RENDER LOOP ***
    
    glUseProgram(shaderProgram); // use shaderProgram for object being rendered
    glBindVertexArray(VAO); // bind that objects VAO
    glDrawArrays(GL_TRIANGLES, 0, 3); // (shape type, vertex array starting index, number of shapes to draw)
    // *******************************************************************************************************************************

    glfwPollEvents(); // polls for events like user input
    glfwSwapBuffers(window); // sets the buffer to be rendered to window
  }

  glfwTerminate();
  return 0;
}
