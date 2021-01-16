#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "geometry/signed_distance_function.h"
#include "geometry/sphere.h"
#include "geometry/mesher.h"

int main () {
  //  Geometry::VoxelGrid grid;
  //  Geometry::VoxelRowCol &plane = grid[0];
  //  std::cout << plane.size() << std::endl;
  //  return 0;

  Geometry::Sphere sphere(glm::vec3(0.0, 0.0, 0.0), 4.0f);
  glm::vec2 xBounds(-5.0, 5.0);
  glm::vec2 yBounds(-5.0, 5.0);
  glm::vec2 zBounds(-5.0, 5.0);
  float unit = 1.0;

  Geometry::Mesher mesher(
    xBounds,
    yBounds,
    zBounds,
    unit,
    sphere
  );

  mesher.generateVoxels();
  std::cout << mesher.voxels.size() << std::endl;

  for (auto &row: mesher.voxels) {
    for (auto &col: row) {
      for (auto &cell: col) {
        if(cell->signedDistance < 0.1) {
          std::cout
            << cell->point.x << ","
            << cell->point.y << ","
            << cell->point.z << ","
            << std::endl;
        }
      }
    }
  }

  //  std::vector<std::reference_wrapper<int>> vec;

  //  if (true) {
    //  int i = 5;
    //  vec.push_back(std::ref(i));
  //  }

  //  std::cout << vec[0] << std::endl;

  return 0;
}
