#ifndef LOAD_SHADER
#define LOAD_SHADER

#include <stdexcept>
#include <string>

#define GLEW_STATIC
#include <GL/glew.h> // GLEW vs GLAD
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OpenGL {
  class Shader {
  public:
    GLuint id;

    Shader(std::string vertexSource, std::string fragmentSource);

    GLuint getAttribLocation(const GLchar *attribute);

    void setRGBA(std::string attribute, GLfloat r, GLfloat g, GLfloat b, GLfloat a);
    void setMatrix4(std::string attribute, const glm::mat4 &transform);

    void use();
    void reset();

  private:
    GLuint initShader (GLenum type, std::string &source);
  };
}
#endif
