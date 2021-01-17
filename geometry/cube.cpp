#include <iostream>

#include "cube.h"

using namespace Geometry;

Cube::Cube(Voxels &voxels_, Vertices &vertices_) :
  voxels(Voxels(voxels_)),
  vertices(vertices_)
{
  Ids recordedIds;
  for(int i = 0; i < 8; i++) {
    if (recordedIds.find(i) != recordedIds.end()) continue;

    Ids newGroup;
    crawl(i, newGroup);
    if(newGroup.empty()) continue;

    recordedIds.insert(newGroup.begin(), newGroup.end());
    groupedVoxelIds.push_back(newGroup);
  }


  if (recordedIds.size() == 0 || recordedIds.size() == 8) {
    isIsosurface = false;
  } else {
    isIsosurface = true;
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

void Cube::generatePolygons() {
  for(auto &group: groupedVoxelIds) {
    generatePolygonsFromGroup(group);
  }
}

void Cube::generatePolygon1Corner(Ids &group, bool reverse = false) {
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

  if (reverse) newPolygon->reverse();
  polygons.push_back(newPolygon);
}

void Cube::generatePolygon2Corner(Ids &group, bool reverse = false) {
  auto it = group.begin();
  Id voxId1 = *it;
  it++;
  Id voxId2 = *it;

  VertexPair vertices1 = getVertices(voxId1, voxId2);
  VertexPair vertices2 = getVertices(voxId2, voxId1);

  Polygon *newPolygon1 = new Polygon(
    vertices1.first, vertices1.second, vertices2.first
  );
  if (reverse) newPolygon1->reverse();
  polygons.push_back(newPolygon1);

  Polygon *newPolygon2 = new Polygon(
    vertices2.first, vertices2.second, vertices1.first
  );
  if (reverse) newPolygon2->reverse();
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
  return adjacencyLookup[voxId][(indexInLookup + 1) % 3];
}

Id Cube::getLeftAdjacentVoxId(Id voxId, Id fromVoxId) {
  Id indexInLookup;
  for(int i = 0; i < 3; i++) {
    if (fromVoxId == adjacencyLookup[voxId][i]) indexInLookup = i;
  }
  return adjacencyLookup[voxId][(indexInLookup + 2) % 3];
}

void Cube::generatePolygon4CornerCone(Ids &group, Id headVoxId) {
  Id id1, id2, id3, idx;
  Id idA, idB, idC;

  idx = headVoxId;

  id1 = *getConnectedVoxIds(headVoxId, group).begin();
  id2 = getRightAdjacentVoxId(idx, id1);
  id3 = getLeftAdjacentVoxId(idx, id1);

  idA = getLeftAdjacentVoxId(id1, idx);
  idB = getLeftAdjacentVoxId(id2, idx);
  idC = getLeftAdjacentVoxId(id3, idx);

  Polygon *polygon1 = new Polygon(
    vertices.findVertex(*voxels[id1], *voxels[idA]),
    vertices.findVertex(*voxels[id2], *voxels[idB]),
    vertices.findVertex(*voxels[id3], *voxels[idC])
  );
  polygons.push_back(polygon1);

  Polygon *polygon2 = new Polygon(
    vertices.findVertex(*voxels[id1], *voxels[idA]),
    vertices.findVertex(*voxels[idA], *voxels[id2]),
    vertices.findVertex(*voxels[id2], *voxels[idB])
  );
  polygons.push_back(polygon2);

  Polygon *polygon3 = new Polygon(
    vertices.findVertex(*voxels[id2], *voxels[idB]),
    vertices.findVertex(*voxels[idB], *voxels[id3]),
    vertices.findVertex(*voxels[id3], *voxels[idC])
  );
  polygons.push_back(polygon3);

  Polygon *polygon4 = new Polygon(
    vertices.findVertex(*voxels[id3], *voxels[idC]),
    vertices.findVertex(*voxels[idC], *voxels[id1]),
    vertices.findVertex(*voxels[id1], *voxels[idA])
  );
  polygons.push_back(polygon4);
}

// This method has not been tested!!
void Cube::generatePolygon4CornerOther(Ids &group) {
  Id id1, id2, id3, id4;
  Id idA, idB, idC, idD;

  bool foundOne = false;
  for(auto i: group) {
    if (getFreeVoxIds(i, group).size() == 2) {
      if (!foundOne) {
        id1 = i;
        id2 = *getConnectedVoxIds(i, group).begin();
      } else {
        id4 = i;
        id3 = *getConnectedVoxIds(i, group).begin();
      }
    }
  }

  idA = getLeftAdjacentVoxId(id1, id2);
  idD = getLeftAdjacentVoxId(id4, id3);

  idB = *getFreeVoxIds(id2, group).begin();
  idC = *getFreeVoxIds(id3, group).begin();

  Polygon *polygon1 = new Polygon(
    vertices.findVertex(*voxels[id1], *voxels[idA]),
    vertices.findVertex(*voxels[id4], *voxels[idB]),
    vertices.findVertex(*voxels[id3], *voxels[idC])
  );
  polygons.push_back(polygon1);

  Polygon *polygon2 = new Polygon(
    vertices.findVertex(*voxels[id1], *voxels[idA]),
    vertices.findVertex(*voxels[id2], *voxels[idB]),
    vertices.findVertex(*voxels[id4], *voxels[idB])
  );
  polygons.push_back(polygon2);

  Polygon *polygon3 = new Polygon(
    vertices.findVertex(*voxels[idB], *voxels[id4]),
    vertices.findVertex(*voxels[idD], *voxels[id4]),
    vertices.findVertex(*voxels[id3], *voxels[idC])
  );
  polygons.push_back(polygon3);

  Polygon *polygon4 = new Polygon(
    vertices.findVertex(*voxels[id1], *voxels[idC]),
    vertices.findVertex(*voxels[id1], *voxels[idA]),
    vertices.findVertex(*voxels[id3], *voxels[idC])
  );
  polygons.push_back(polygon4);
}

void Cube::generatePolygon3Corner(Ids &group, bool reverse = false) {
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
  vtxB = vertices.findVertex(*voxels[id2], *voxels[idB]);

  id3 = getLeftAdjacentVoxId(id1, idA);
  idC = getLeftAdjacentVoxId(id3, id1);
  vtxC = vertices.findVertex(*voxels[id3], *voxels[idC]);

  idx = getRightAdjacentVoxId(id2, idB);

  vtxD = vertices.findVertex(*voxels[id2], *voxels[idx]);
  vtxE = vertices.findVertex(*voxels[id3], *voxels[idx]);

  Polygon *newPolygon1 = new Polygon(vtxA, vtxB, vtxC);
  if (reverse) newPolygon1->reverse();
  polygons.push_back(newPolygon1);

  Polygon *newPolygon2 = new Polygon(vtxB, vtxD, vtxC);
  if (reverse) newPolygon2->reverse();
  polygons.push_back(newPolygon2);

  Polygon *newPolygon3 = new Polygon(vtxD, vtxE, vtxC);
  if (reverse) newPolygon3->reverse();
  polygons.push_back(newPolygon3);
}

int Cube::countFreeVox(Id voxId, Ids &group) {
  return 3 - getConnectedVoxIds(voxId, group).size();
}

Ids Cube::reverseGroup(Ids&group) {
  Ids result;
  for (auto i = 0; i < 8; i++) {
    if (group.find(i) == group.end()) result.insert(i);
  }
  return Ids(result);
}

void Cube::generatePolygonsFromGroup(Ids &group) {
  std::cout << "group size: " << group.size() << std::endl;
  switch(group.size()) {
    case 1: {
      generatePolygon1Corner(group);
      break;
    }

    case 2: {
      generatePolygon2Corner(group);
      break;
    }

    case 3: {
      generatePolygon3Corner(group);
      break;
    }

    case 4:
      if (findPlaneId(group) >= 0) {
        generatePolygon4CornerPlane(group);
      } else {
        bool isCone = false;
        Id headVoxId;
        for(auto i: group) {
          if (getFreeVoxIds(i, group).empty()) {
            isCone = true;
            headVoxId = i;
            break;
          }
        }

        if (isCone) {
          generatePolygon4CornerCone(group, headVoxId);
        } else {
          generatePolygon4CornerOther(group);
        }
      }
      break;

    case 5: {
      Ids reversedGroup = reverseGroup(group);
      generatePolygon3Corner(reversedGroup, true);
      break;
    }

    case 6:  {
      Ids reversedGroup = reverseGroup(group);
      generatePolygon2Corner(reversedGroup, true);
      break;
    }

    case 7: {
      Ids reversedGroup = reverseGroup(group);
      generatePolygon1Corner(reversedGroup, true);
      break;
    }

    case 8: {
      // this shouldn't happen... but it does; investigate
      break;
    }
    default:
      std::cout << "Unexpected Configuration!! " << group.size() << std::endl;
  }
}
