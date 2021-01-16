#include <iostream>

#include "cube.h"

using namespace Geometry;

Cube::Cube(Voxels &voxels_, Vertices &vertices_) :
  voxels(Voxels(voxels_)),
  vertices(vertices_)
{
  Ids negSignedVoxIds;
  for(int i = 0; i < 8; i++) {
    if (voxels[i]->sign == false) negSignedVoxIds.insert(i);
  }

  unsigned int negSignedVoxCount = negSignedVoxIds.size();
  if (negSignedVoxCount == 0 || negSignedVoxCount == 8) {
    isIsosurface = false;
  } else {
    isIsosurface = true;
    groupVoxels(negSignedVoxIds);
  }
}

void Cube::groupVoxels(Ids &negSignedVoxIds) {
  while(!negSignedVoxIds.empty()) {
    auto it = negSignedVoxIds.begin();
    unsigned int id = *it;
    negSignedVoxIds.erase(it);

    bool inserted = false;
    for (auto &group: groupedVoxelIds) {
      if (inserted) break;
      for (auto nextId: adjacencyLookup[id]) {
        if (inserted) break;

        if (group.find(nextId) != group.end()) {
          group.insert(id);
          inserted = true;
        }
      }
    }

    if (!inserted) {
      Ids newGroup;
      newGroup.insert(id);
      groupedVoxelIds.push_back(newGroup);
    }
  }
}

void Cube::generatePolygons() {
  for(auto &group: groupedVoxelIds) {
    generatePolygonsFromGroup(group);
  }
}

void Cube::generatePolygon1Corner(Ids &group) {
  unsigned int id = *group.begin();
  Voxels voxelsToConn;
  for (auto nextId: adjacencyLookup[id]) {
    voxelsToConn.push_back(voxels[nextId]);
  }
  Polygon *newPolygon = new Polygon(
    vertices.findVertex(*voxels[id], *voxelsToConn[0]),
    vertices.findVertex(*voxels[id], *voxelsToConn[1]),
    vertices.findVertex(*voxels[id], *voxelsToConn[2])
  );
  polygons.push_back(newPolygon);
}

void Cube::generatePolygon1Side(Ids &group) {
  auto it = group.begin();
  unsigned int voxId1 = *it;
  it++;
  unsigned int voxId2 = *it;

  VertexPair vertices1 = getVertices(voxId1, voxId2);
  VertexPair vertices2 = getVertices(voxId2, voxId1);

  Polygon *newPolygon1 = new Polygon(
    vertices1.first, vertices1.second, vertices2.first
  );
  polygons.push_back(newPolygon1);

  Polygon *newPolygon2 = new Polygon(
    vertices2.first, vertices2.second, vertices1.first
  );
  polygons.push_back(newPolygon2);
}

VertexPair Cube::getVertices(unsigned int voxId, unsigned int otherVoxId) {
  const unsigned int *ajacentVoxIds = &(adjacencyLookup[voxId][0]);
  int voxId1, voxId2;

  for (int i = 0; i < 3; i++) {
    if (ajacentVoxIds[i] == otherVoxId) {
      voxId1 = ajacentVoxIds[(i + 1) % 3];
      voxId2 = ajacentVoxIds[(i + 2) % 3];
      break;
    }
  }

  //  std::cout
    //  << voxId << ", "
    //  << voxId1 << ", "
    //  << voxId2 << ", "
    //  << std::endl;
  return VertexPair(
    vertices.findVertex(*voxels[voxId], *voxels[voxId1]),
    vertices.findVertex(*voxels[voxId], *voxels[voxId2])
  );
}

void Cube::generatePolygonsFromGroup(Ids &group) {
  switch(group.size()) {
    case 1:
      generatePolygon1Corner(group);
      break;
    case 2:
      generatePolygon1Side(group);
      break;
    //  case 3:
      //  break;

    //  case 4:
      //  break;

    //  case 5:
    //  case 6:
    //  case 7:
      //  break;

    default:
      std::cout << "default: " << group.size() << std::endl;
  }
}
