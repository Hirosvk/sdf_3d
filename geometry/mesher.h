#include <iostream>
#include <vector>

#include "signed_distance_function.h"
#include "voxel.h"

namespace Geometry {
  class Mesher {
    glm::vec2 xRange, yRange, zRange;
    float voxelUnit;
    SignedDistanceFunction &sdf;

  public:
    VoxelGrid voxels;

  public:
    Mesher(
      glm::vec2 xRange_,
      glm::vec2 yRange_,
      glm::vec2 zRange_,
      float voxelUnit_,
      SignedDistanceFunction &sdf_
    );
    ~Mesher();

    void generateVoxels();
  };
}
