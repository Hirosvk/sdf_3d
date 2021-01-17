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

  // Setting up Camera
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


  // Setting up Sphere
  float bound = 20.0;

  float radius = 5.0;
  glm::vec3 center(-5.0, -5.0, -5.0);

  Sphere sdf = Sphere(center, radius);
  SignedDistanceFunction *sdf_ptr = &sdf;

  Mesher mesher(
    glm::vec2(-bound, bound * 2),
    glm::vec2(-bound, bound * 2),
    glm::vec2(-bound, bound * 2),
    bound / 20.0,
    *sdf_ptr
  );

  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  mesher.generatePolygons(vertices, indices);

  glm::vec4 color(1.0f, 0.0f, 0.0f, 1.0f);
  OpenGL::Mesh mesh(vertices, indices, color);


  // Setting up Square
  glm::vec3 origin(0.0, 0.0, 0.0);
  float dimension = 20.0;
  Square sq = Square(origin, dimension);
  SignedDistanceFunction *sdf_ptr2 = &sq;

  Mesher mesher2(
    glm::vec2(-bound, bound * 2),
    glm::vec2(-bound, bound * 2),
    glm::vec2(-bound, bound * 2),
    bound / 20.0,
    *sdf_ptr2
  );

  std::vector<float> vertices2;
  std::vector<unsigned int> indices2;
  mesher2.generatePolygons(vertices2, indices2);

  glm::vec4 color2(0.0f, 1.0f, 0.0f, 1.0f);
  OpenGL::Mesh mesh2(vertices2, indices2, color2);


  // Render loop
  while(!window.shouldClose()){
    window.clear();
    mesh.draw(camera);
    mesh2.draw(camera);
    window.swapBuffers();
    window.pollAndProcessEvents();
  }
  window.terminate();

  return 0;
}
