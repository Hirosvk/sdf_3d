#include <iostream>

#include "open_gl/window.h"
#include "open_gl/camera.h"
#include "open_gl/mesh.h"
#include "open_gl/input_handler.h"

#include "geometry/signed_distance_function.h"
#include "geometry/square.h"
#include "geometry/sphere.h"
#include "geometry/mesher.h"

using namespace Geometry;

int main () {
  int width = 1200;
  int height = 900;

  OpenGL::Camera camera(width, height);
  glm::vec3 eye(
    183.77261603058471f,
    -277.6766183259117f,
    86.69292961708459f
  );
  glm::vec3 target(0.0f, 0.0f, 0.0f);
  camera.set(eye, target);

  OpenGL::InputHandler inputHandler(camera);
  OpenGL::Window window(width, height, inputHandler);

  float unit = 10.0;
  float halfUnit = unit / 2.0;
  float radius = 5.0;
  glm::vec3 origin(0.0, 0.0, 0.0);

  Sphere sdf = Sphere(origin, radius);

  SignedDistanceFunction *sdf_ptr = &sdf;

  Mesher mesher(
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    unit / 20.0,
    *sdf_ptr
  );

  mesher.generateVoxels();
  mesher.generateVertices();
  mesher.generateCubes();

  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  mesher.generatePolygons(vertices, indices);

  OpenGL::Mesh mesh(vertices, indices);

  while(!window.shouldClose()){
    window.clear();
    mesh.draw(camera);
    window.swapBuffers();
    window.pollAndProcessEvents();
  }
  window.terminate();

  return 0;
}
