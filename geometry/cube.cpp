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

  Id negSignedVoxCount = negSignedVoxIds.size();
  if (negSignedVoxCount == 0 || negSignedVoxCount == 8) {
    isIsosurface = false;
  } else {
    isIsosurface = true;
    groupVoxels(negSignedVoxIds);
  }
}

int Cube::extractAdjacent(Ids &negSignedVoxIds, Id id, Ids &targetGroup) {
  int count = 0;
  for (auto nextId: adjacencyLookup[id]) {
    if (targetGroup.find(nextId) != targetGroup.end()) continue;

    if (negSignedVoxIds.find(nextId) != negSignedVoxIds.end()) {
      targetGroup.insert(nextId);
      negSignedVoxIds.erase(negSignedVoxIds.find(nextId));
      count++;
    }
  }
  return count;
}

void Cube::groupVoxels(Ids &negSignedVoxIds) {
  while(!negSignedVoxIds.empty()) {
    auto it = negSignedVoxIds.begin();
    Id id = *it;
    negSignedVoxIds.erase(it);

    Ids newGroup;
    newGroup.insert(id);

    Ids checkedIds;

    Id currentId = id;
    while(extractAdjacent(negSignedVoxIds, currentId, newGroup) > 0) {
      checkedIds.insert(currentId);

      for (auto id_: newGroup) {
        if (checkedIds.find(id_) == checkedIds.end()) {
          currentId = *newGroup.find(id_);
        }
      }
    }
    groupedVoxelIds.push_back(newGroup);
  }
}

void Cube::generatePolygons() {
  for(auto &group: groupedVoxelIds) {
    generatePolygonsFromGroup(group);
  }
}

void Cube::generatePolygon1Corner(Ids &group) {
  Id id = *group.begin();
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
  Id voxId1 = *it;
  it++;
  Id voxId2 = *it;

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

VertexPair Cube::getVertices(Id voxId, Id otherVoxId) {
  const Id *ajacentVoxIds = &(adjacencyLookup[voxId][0]);
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

void Cube::generatePolygon4CornerPlane(Ids&group) {
  int planeId = findPlaneId(group);
  const Id *voxId = planeConfigs[planeId];

  Id otherId1 = getFreeAdajacentVoxId(*voxId, *(voxId + 1), *(voxId + 3));
  Vertex *vt1 = vertices.findVertex(*voxels[*voxId], *voxels[otherId1]);

  Id otherId2 = getFreeAdajacentVoxId(*(voxId + 1), *(voxId + 2), *voxId);
  Vertex *vt2 = vertices.findVertex(*voxels[*(voxId + 1)], *voxels[otherId2]);

  Id otherId3 = getFreeAdajacentVoxId(*(voxId + 2), *(voxId + 1), *(voxId + 3));
  Vertex *vt3 = vertices.findVertex(*voxels[*(voxId + 2)], *voxels[otherId3]);

  Id otherId4 = getFreeAdajacentVoxId(*(voxId + 3), *voxId, *(voxId + 2));
  Vertex *vt4 = vertices.findVertex(*voxels[*(voxId + 3)], *voxels[otherId4]);

  Polygon *polygon1 = new Polygon(vt1, vt4, vt3);
  polygons.push_back(polygon1);
  Polygon *polygon2 = new Polygon(vt1, vt3, vt2);
  polygons.push_back(polygon2);
}

Ids Cube::getConnectedVoxIds(Id voxId, Ids &group) {
  Ids ids;
  for (int i = 0; i < 3; i++) {
    if (group.find(adjacencyLookup[voxId][i]) != group.end()) {
      ids.insert(adjacencyLookup[voxId][i]);
    }
  }
  return Ids(ids);
}


int Cube::findPlaneId(Ids&group) {
  for (int i = 0; i < 6; i++) {
    Ids plane(planeConfigs[i], planeConfigs[i] + 4);
    if (plane == group) return i;
  }
  return -1;
}

Id Cube::getFreeAdajacentVoxId(Id voxId, Id otherVoxId1, Id otherVoxId2) {
  for(auto id: adjacencyLookup[voxId]) {
    if (id != otherVoxId1 && id != otherVoxId2) return id;
  }
  return -1;
}

//  Id Cube::getRightAdjacentVoxId(Id voxId, Id freeVoxId) {
  //  Id indexInLookup;
  //  for(int i = 0; i < 3; i++) {
    //  if (freeVoxId == adjacencyLookup[voxId][i]) indexInLookup = i;
  //  }
  //  return (i + 1) % 3;
//  }

//  Id Cube::getLeftAdjacentVoxId(Id voxId, Id freeVoxId) {
  //  Id indexInLookup;
  //  for(int i = 0; i < 3; i++) {
    //  if (freeVoxId = adjacencyLookup[voxId][i]) indexInLookup = i;
  //  }
  //  return (i + 2) % 3;
//  }

//  void Cube::generatePolygon3Corner(Ids &group) {
  //  Id VoxIdWith1Free;
  //  for(auto id: group) {
    //  if (countFreeVox(id) == 1) {
      //  VoxIdWith1Free = id;
      //  break;
    //  }
  //  }
//  }

void Cube::generatePolygonsFromGroup(Ids &group) {
  std::cout << "group size: " << group.size() << std::endl;
  switch(group.size()) {
    case 1:
      generatePolygon1Corner(group);
      break;
    case 2:
      generatePolygon1Side(group);
      break;
    //  case 3:
      //  break;

    case 4:
      if (findPlaneId(group) >= 0) {
        generatePolygon4CornerPlane(group);
      } else {
        std::cout << "not plane" << std::endl;
      }
      break;

    //  case 5:
    //  case 6:
    //  case 7:
      //  break;

    default:
      std::cout << "default: " << std::endl;
  }
}
