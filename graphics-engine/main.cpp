#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// called by GLFW on window resize to dynamically update our glViewport
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

// method for processing user input
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  // init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "graphics-engine", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // validate that GLAD has initialized
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // set the OpenGL Viewport & register dynamic window resizing callback
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    // rendering commands go here...
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents(); // polls for events like user input
    glfwSwapBuffers(window); // sets the buffer to be rendered to window
  }

  // cleanup and terminate process
  glfwTerminate();
  return 0;
}
