#include "mesh.h"

using namespace OpenGL;

Mesh::Mesh(
  std::vector<float> &vertices,
  std::vector<unsigned int> &indices
) :
  shader(Shader(vertexSource, fragmentSource))
{
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(float) * vertices.size(),
    &vertices[0],
    GL_STATIC_DRAW
  );

  int posAttrib = shader.getAttribLocation("position");
  glVertexAttribPointer(
    posAttrib,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*) 0
  );
  glEnableVertexAttribArray(posAttrib);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  indexSize = indices.size();
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    sizeof(unsigned int) * indexSize,
    &indices[0],
    GL_STATIC_DRAW
  );

  shader.use();
  shader.setRGBA("currentColor", 1.0f, 0.0f, 0.0f, 1.0f);

  glBindVertexArray(0);
}

void Mesh::draw(Camera &camera) {
  shader.use();

  shader.setMatrix4("view", camera.view());
  shader.setMatrix4("projection", camera.projection());

  glBindVertexArray(VAO);

  glDrawElements(
    GL_TRIANGLES,
    indexSize,
    GL_UNSIGNED_INT,
    0 // offset
  );

  glBindVertexArray(0);
  shader.reset();
}
