#include <iostream>

#include "cube.h"

using namespace Geometry;

Cube::Cube(Voxels &voxels_, Vertices &vertices_) :
  voxels(Voxels(voxels_)),
  vertices(vertices_)
{
  //  Ids negSignedVoxIds;
  //  for(int i = 0; i < 8; i++) {
    //  if (voxels[i]->sign == false) negSignedVoxIds.insert(i);
  //  }
  Ids recordedIds;
  for(int i = 0; i < 8; i++) {
    if (recordedIds.find(i) != recordedIds.end()) continue;

    Ids newGroup;
    crawl(i, newGroup);
    if(newGroup.empty()) continue;

    recordedIds.insert(newGroup.begin(), newGroup.end());
    groupedVoxelIds.push_back(newGroup);
    //  for(auto id: newGroup) {
      //  std::cout << id << ", ";
    //  }
    //  std::cout << std::endl;
  }


  //  Id negSignedVoxCount = negSignedVoxIds.size();
  //  if (negSignedVoxCount == 0 || negSignedVoxCount == 8) {
  if (recordedIds.size() == 0 || recordedIds.size() == 8) {
    isIsosurface = false;
  } else {
    isIsosurface = true;
    //  groupVoxels(negSignedVoxIds);
  }
}

void Cube::crawl(Id id, Ids &targetGroup) {
  if (targetGroup.find(id) != targetGroup.end()) return;
  if (voxels[id]->sign != false) return;

  targetGroup.insert(id);

  for(int i = 0; i < 3; i++) {
    crawl(adjacencyLookup[id][i], targetGroup);
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

  Id otherId1 = *getFreeVoxIds(*voxId, group).begin();
  Vertex *vt1 = vertices.findVertex(*voxels[*voxId], *voxels[otherId1]);

  Id otherId2 = *getFreeVoxIds(*(voxId+1), group).begin();
  Vertex *vt2 = vertices.findVertex(*voxels[*(voxId+1)], *voxels[otherId2]);

  Id otherId3 = *getFreeVoxIds(*(voxId+2), group).begin();
  Vertex *vt3 = vertices.findVertex(*voxels[*(voxId+2)], *voxels[otherId3]);

  Id otherId4 = *getFreeVoxIds(*(voxId+3), group).begin();
  Vertex *vt4 = vertices.findVertex(*voxels[*(voxId+3)], *voxels[otherId4]);

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

Ids Cube::getFreeVoxIds(Id voxId, Ids &group) {
  Ids ids;
  for (int i = 0; i < 3; i++) {
    if (group.find(adjacencyLookup[voxId][i]) == group.end()) {
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

Id Cube::getRightAdjacentVoxId(Id voxId, Id fromVoxId) {
  Id indexInLookup;
  for(int i = 0; i < 3; i++) {
    if (fromVoxId == adjacencyLookup[voxId][i]) indexInLookup = i;
  }
  return (indexInLookup + 1) % 3;
}

Id Cube::getLeftAdjacentVoxId(Id voxId, Id fromVoxId) {
  Id indexInLookup;
  for(int i = 0; i < 3; i++) {
    if (fromVoxId == adjacencyLookup[voxId][i]) indexInLookup = i;
  }
  return (indexInLookup + 2) % 3;
}

void Cube::generatePolygon3Corner(Ids &group) {
  Id id1, id2, id3, idx;
  Id idA, idB, idC, idD, idE;
  Vertex *vtxA, *vtxB, *vtxC, *vtxD, *vtxE;

  for (auto id: group) {
    Ids freeVoxIds = getFreeVoxIds(id, group);

    if (freeVoxIds.size() == 1) {
      id1 = id;
      idA = *freeVoxIds.begin();
      vtxA = vertices.findVertex(*voxels[id1], *voxels[idA]);
      break;
    }
  }

  id2 = getRightAdjacentVoxId(id1, idA);
  idB = getRightAdjacentVoxId(id2, id1);
  std::cout << "  checking idB" << std::endl;
  vtxB = vertices.findVertex(*voxels[id2], *voxels[idB]);

  id3 = getLeftAdjacentVoxId(id1, idA);
  idC = getLeftAdjacentVoxId(id3, id1);
  std::cout << "  checking idC" << std::endl;
  vtxC = vertices.findVertex(*voxels[id3], *voxels[idC]);

  idx = getRightAdjacentVoxId(id2, idB);
  if (idx != getLeftAdjacentVoxId(id3, idC)) {
    std::cout << "  wrong here" << std::endl;

    std::cout << "  "
      << id1 << ", "
      << id2 << ", "
      << id3 << ", "
      << std::endl;
  }

  vtxD = vertices.findVertex(*voxels[id2], *voxels[idx]);
  vtxE = vertices.findVertex(*voxels[id3], *voxels[idx]);

  Polygon *newPolygon1 = new Polygon(vtxA, vtxB, vtxC);
  polygons.push_back(newPolygon1);
  Polygon *newPolygon2 = new Polygon(vtxB, vtxD, vtxC);
  polygons.push_back(newPolygon2);
  Polygon *newPolygon3 = new Polygon(vtxD, vtxE, vtxC);
  polygons.push_back(newPolygon3);
}

int Cube::countFreeVox(Id voxId, Ids &group) {
  return 3 - getConnectedVoxIds(voxId, group).size();
}

void Cube::generatePolygonsFromGroup(Ids &group) {
  std::cout << "group size: " << group.size() << std::endl;
  switch(group.size()) {
    case 1:
      generatePolygon1Corner(group);
      break;
    case 2:
      generatePolygon1Side(group);
      break;

    case 3:
      generatePolygon3Corner(group);
      break;

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
