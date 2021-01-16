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
  sdf(sdf_),
  vertices(Vertices())
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
  unsigned int id = 0;

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
        float distance = sdf(currentPos);

        Voxel *voxel = new Voxel(
          id,
          currentPos,
          distance,
          distance > 0
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
void Mesher::generateVertices() {
  for(int x = 0; x < (voxels.size() - 1); x++) {
    for(int y = 0; y < (voxels[x].size() - 1); y++) {
      for(int z = 0; z < (voxels[y].size() - 1); z++) {
        Voxel &voxel = *voxels[x][y][z];

        vertices.addVertex(voxel, *voxels[x+1][y][z]);
        vertices.addVertex(voxel, *voxels[x][y+1][z]);
        vertices.addVertex(voxel, *voxels[x][y][z+1]);
      }
    }
  }
}
