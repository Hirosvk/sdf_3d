#include <iostream>

#include "shader.h"

using namespace OpenGL;

Shader::Shader(std::string vertexSource, std::string fragmentSource) {
  GLuint vertexShader = initShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = initShader(GL_FRAGMENT_SHADER, fragmentSource);

  id = glCreateProgram();
  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);

  glLinkProgram(id);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

GLuint Shader::getAttribLocation(const GLchar *attribute) {
  return glGetAttribLocation(id, attribute);
}

void Shader::setRGBA(std::string attribute, GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
  const GLchar* c_attr = attribute.c_str();

  int location = glGetUniformLocation(id, c_attr);
  glUniform4f(location, r, g, b, a);
}

void Shader::setMatrix4(std::string attribute, const glm::mat4 &transform) {
  const GLchar* c_attr = attribute.c_str();

  int location = glGetUniformLocation(id, c_attr);
  glUniformMatrix4fv(location, 1, GL_FALSE, &transform[0][0]);
}

void Shader::use() {
  glUseProgram(id);
}

void Shader::reset() {
  glUseProgram(0);
}

GLuint Shader::initShader (GLenum type, std::string &source)
{
  GLuint shaderId = glCreateShader(type);
  const GLchar* c_source = source.c_str();
  glShaderSource(shaderId, 1, &c_source, NULL);
  glCompileShader(shaderId);

  GLint status;
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {

    char infoLog[512];
    glGetShaderInfoLog(shaderId, 521, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

    throw std::runtime_error("shader compile failed");
  }
  return shaderId;
}
