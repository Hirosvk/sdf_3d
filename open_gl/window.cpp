#include "window.h"

using namespace OpenGL;

Window::Window(int screenWidth_, int screenHeight_, InputHandler &inputHandler_) :
  screenWidth(screenWidth_),
  screenHeight(screenHeight_),
  inputHandler(inputHandler_)
{
  initGLFW();
  initGL();

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Whenever glClear with COLOR_BUFFER is called
  // it uses the color set by glClearColor.
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

bool Window::shouldClose() {
  return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
  glfwSwapBuffers(window);
}

void Window::pollAndProcessEvents() {
  glfwPollEvents();
  processInput();
}

void Window::terminate() {
  glfwTerminate();
}

void Window::processInput() {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);

  } else {
    for(auto key: inputHandler.keysToWatch) {
      if(glfwGetKey(window, key) == GLFW_PRESS) {
        inputHandler.handle(key);
      }
    }
  }
}

void Window::clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window::initGLFW() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", nullptr, nullptr);

  glfwMakeContextCurrent(window);
}

void Window::initGL() {
  glewExperimental = GL_TRUE;
  glewInit();

  glEnable(GL_DEPTH_TEST);

  // for texts
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, screenWidth, screenHeight); // it worked without this, but the tutorial says to use it...
}
