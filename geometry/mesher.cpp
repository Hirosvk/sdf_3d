#include "mesher.h"

using namespace Geometry;

Mesher::Mesher(
  glm::vec2 xRange_,
  glm::vec2 yRange_,
  glm::vec2 zRange_,
  float voxelUnit_,
  SignedDistanceFunction &sdf_
) :
  xRange(xRange_),
  yRange(yRange_),
  zRange(zRange_),
  voxelUnit(voxelUnit_),
  sdf(sdf_)
{}

Mesher::~Mesher() {
  for (auto &row: voxels) {
    for (auto &col: row) {
      for (auto &cell: col) {
        delete cell;
      }
    }
  }
}

void Mesher::generateVoxels() {
  glm::vec3 currentPos(0.0);
  int id = 0;

  currentPos.x = xRange[0];
  while(currentPos.x < xRange[1]) {
    voxels.push_back(VoxelRowCol());
    VoxelRowCol &rowCol = voxels.back();

    currentPos.y = yRange[0];
    while(currentPos.y < yRange[1]) {
      rowCol.push_back(VoxelRow());
      VoxelRow &row = rowCol.back();

      currentPos.z = zRange[0];
      while(currentPos.z < zRange[1]) {
        Voxel *voxel = new Voxel(
          id,
          currentPos,
          sdf(currentPos)
        );
        row.push_back(voxel);

        id++;
        currentPos.z += voxelUnit;
      }
      currentPos.y += voxelUnit;
    }
    currentPos.x += voxelUnit;
  }
}
