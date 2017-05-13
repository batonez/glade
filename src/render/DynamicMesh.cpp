#include <glade/render/meshes/DynamicMesh.h>
#include <glade/math/Vector.h>

void DynamicMesh::calculateNormals()
{
  // FIXME everything is bad because a vertex belongs to several triangles
  int numberOfTriangles = indices.size() / 3;

  for (int triangle = 0; triangle < numberOfTriangles; ++triangle) {
    int index1 = indices[triangle * 3 + 0];
    int index2 = indices[triangle * 3 + 1];
    int index3 = indices[triangle * 3 + 2];
    Vector3f vertex1(vertices[index1 * 8 + 0], vertices[index1 * 8 + 1], vertices[index1 * 8 + 2]);
    Vector3f vertex2(vertices[index2 * 8 + 0], vertices[index2 * 8 + 1], vertices[index2 * 8 + 2]);
    Vector3f vertex3(vertices[index3 * 8 + 0], vertices[index3 * 8 + 1], vertices[index3 * 8 + 2]);

    Vector3f side1, side2;
    vertex2.subtract(vertex1, side1);
    vertex1.subtract(vertex3, side2);
    Vector3f normal;
    side2.cross(side1, normal);
    normal.normalize();

    Vector3f oldNormal, newNormal;

    oldNormal.set(vertices[index1 * 8 + 3], vertices[index1 * 8 + 4], vertices[index1 * 8 + 5]);
    oldNormal.add(normal, newNormal);
    newNormal.normalize();
    vertices[index1 * 8 + 3] = newNormal.x;
    vertices[index1 * 8 + 4] = newNormal.y;
    vertices[index1 * 8 + 5] = newNormal.z;

    oldNormal.set(vertices[index2 * 8 + 3], vertices[index2 * 8 + 4], vertices[index2 * 8 + 5]);
    oldNormal.add(normal, newNormal);
    newNormal.normalize();
    vertices[index2 * 8 + 3] = newNormal.x;
    vertices[index2 * 8 + 4] = newNormal.y;
    vertices[index2 * 8 + 5] = newNormal.z;

    oldNormal.set(vertices[index3 * 8 + 3], vertices[index3 * 8 + 4], vertices[index3 * 8 + 5]);
    oldNormal.add(normal, newNormal);
    newNormal.normalize();
    vertices[index3 * 8 + 3] = newNormal.x;
    vertices[index3 * 8 + 4] = newNormal.y;
    vertices[index3 * 8 + 5] = newNormal.z;    
  }
}


void DynamicMesh::invertNormals()
{
  int numberOfVertices = vertices.size() / 8;

  for (int vertIndex = 0; vertIndex < numberOfVertices; ++vertIndex) {
    Vector3f normal(vertIndex * 8 + 3, vertIndex * 8 + 4, vertIndex * 8 + 5);
    vertices[vertIndex * 8 + 3] = -normal.x;
    vertices[vertIndex * 8 + 4] = -normal.y;
    vertices[vertIndex * 8 + 5] = -normal.z;
  }
}
