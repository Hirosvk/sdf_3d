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

  for(auto &cube: cubes) {
    delete cube;
  }
}

void Mesher::generateCubes() {
  for(int x = 0; x < (voxels.size() - 1); x++) {
    for(int y = 0; y < (voxels[x].size() - 1); y++) {
      for(int z = 0; z < (voxels[y].size() - 1); z++) {
        Voxels cubeVoxels;

        cubeVoxels.push_back(voxels[x][y][z]);
        cubeVoxels.push_back(voxels[x+1][y][z]);
        cubeVoxels.push_back(voxels[x+1][y+1][z]);
        cubeVoxels.push_back(voxels[x][y+1][z]);

        cubeVoxels.push_back(voxels[x][y][z+1]);
        cubeVoxels.push_back(voxels[x+1][y][z+1]);
        cubeVoxels.push_back(voxels[x+1][y+1][z+1]);
        cubeVoxels.push_back(voxels[x][y+1][z+1]);
        Cube *newCube = new Cube(cubeVoxels, vertices);

        newCube->generatePolygons();
        cubes.push_back(newCube);
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
  for(int x = 0; x < voxels.size(); x++) {
    for(int y = 0; y < voxels[x].size(); y++) {
      for(int z = 0; z < voxels[y].size(); z++) {
        Voxel &voxel = *voxels[x][y][z];

        if (x + 1 < voxels.size()) vertices.addVertex(voxel, *voxels[x+1][y][z]);
        if (y + 1 < voxels[x].size()) vertices.addVertex(voxel, *voxels[x][y+1][z]);
        if (z + 1 < voxels[x][y].size()) vertices.addVertex(voxel, *voxels[x][y][z+1]);
      }
    }
  }
}
