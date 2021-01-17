#define GLEW_STATIC
#include <GL/glew.h> // GLEW vs GLAD
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

namespace OpenGL {
  class Mesh
  {

  const std::string vertexSource = "\
    #version 150 core\
    in vec3 position;\
    uniform mat4 projection;\
    uniform mat4 view;\
    void main()\
    {\
      gl_Position = projection * view * vec4(position.x, position.y, position.z, 1.0);\
    }\
  ";

  const std::string fragmentSource = "\
    #version 150 core\
    out vec4 outColor;\
    uniform vec4 currentColor;\
    void main()\
    {\
      outColor = currentColor;\
    }\
  ";

  public:
    Shader shader;
    glm::vec4 color;
    unsigned int VAO, VBO, EBO;
    unsigned int indexSize;

    Mesh(std::vector<float> &vertices, std::vector<unsigned int> &indices, glm::vec4 color);
    void draw(Camera &camera);
  };
}
