#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "geometry/signed_distance_function.h"
#include "geometry/square.h"
#include "geometry/mesher.h"


using namespace Geometry;
int main () {
  float unit = 10.0;
  float halfUnit = unit / 2.0;
  glm::vec3 origin(5.0, 0.0, 0.0);

  Square sdf = Square(origin, unit);
  SignedDistanceFunction *sdf_ptr = &sdf;

  Mesher mesher(
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    glm::vec2(-unit, unit * 2),
    halfUnit,
    *sdf_ptr
  );

  mesher.generateVoxels();
  mesher.generateVertices();
  mesher.generateCubes();

  int c = 0;
  for(auto &cube: mesher.cubes) {
    if (cube->isIsosurface && cube->polygons.size() > 0) {
      for(auto &polygon: cube->polygons) {
        std::cout
          << polygon->vertices[0]->point.x << ", "
          << polygon->vertices[0]->point.y << ", "
          << polygon->vertices[0]->point.z << ", "

          << polygon->vertices[1]->point.x << ", "
          << polygon->vertices[1]->point.y << ", "
          << polygon->vertices[1]->point.z << ", "

          << polygon->vertices[2]->point.x << ", "
          << polygon->vertices[2]->point.y << ", "
          << polygon->vertices[2]->point.z << ", "
          << std::endl;
      }
    }
  }

  return 0;
}
