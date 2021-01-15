#include <iostream>

#include "open_gl/window.h"
#include "open_gl/camera.h"
#include "open_gl/mesh.h"

int main () {
  int width = 1200;
  int height = 800;

  OpenGL::Window window(width, height);

  float vertices_[] = {
    5.035548844834139f,
    4.755530699947883f,
    80.69576856941576f,

    2.702701279245076f,
    -78.9807674850222f,
    0.0f,

    54.572561181482875f,
    4.755530699947869f,
    0.0f
  };
  std::vector<float> vertices(vertices_, vertices_ + 9);

  unsigned int indices_[] = {
    0, 1, 2
  };
  std::vector<unsigned int> indices(indices_, indices_ + 3);

  OpenGL::Mesh mesh(vertices, indices);
  OpenGL::Camera camera(width, height);

  glm::vec3 eye(
    183.77261603058471f,
    -277.6766183259117f,
    86.69292961708459f
  );

  glm::vec3 target(
    -342.3184168875409f,
    511.39640861489653f,
    -97.62788522248522f
  );

  camera.set(eye, target);

  while(!window.shouldClose()){
    window.clear();
    mesh.draw(camera);
    window.swapBuffers();
    window.pollAndProcessEvents();
  }
  window.terminate();

  return 0;
}
