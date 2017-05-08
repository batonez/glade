#include <map>

#include <glade/util/WavefrontObjReader.h>
#include <glade/render/definitions.h>
#include <glade/render/meshes/Mesh.h>

static std::vector<char>::const_iterator readPos;
static int positionInsideTheLine;

static std::vector<std::vector<float> * > positions;
static std::vector<std::vector<float> * > normals;
static std::vector<std::vector<float> * > texCoords;
static std::vector<std::string>         faces;
static std::map<std::string, int>       vertexMap;

static bool getNextLine(const std::vector<char> &in, std::string &line)
{
  line.clear();

  for (; readPos != in.end(); ++readPos) {
    if (*readPos == '\r' || *readPos == '\n') {
      if (line.size() > 0) {
        return true;
      }

      continue;
    }

    line += *readPos;
  }

  return (bool) line.size();
}

static bool getNextToken(std::string &line, std::string &token, const char separator = ' ')
{
  token.clear();

  for (; positionInsideTheLine < line.size(); ++positionInsideTheLine) {
    if (line[positionInsideTheLine] == separator) {
      if (token.size() > 0) {
        return true;
      }

      continue;
    }

    token += line[positionInsideTheLine];
  }

  return (bool) token.size();
}

static void parseVertexPosition(std::string &line)
{
  std::string token;
  std::vector<float> *position = new std::vector<float>;

  for (int i = 1; i <= 3; ++i) {
    getNextToken(line, token);
    position->push_back(std::stof(token));
  }

  positions.push_back(position);
}

static void parseVertexNormal(std::string &line)
{
  std::string token;
  std::vector<float> *normal = new std::vector<float>;

  for (int i = 1; i <= 3; ++i) {
    getNextToken(line, token);
    normal->push_back(std::stof(token));
  }

  normals.push_back(normal);
}

static void parseVertexTexCoord(std::string &line)
{
  std::string token;
  std::vector<float> *texCoord = new std::vector<float>;

  for (int i = 1; i <= 2; ++i) {
    getNextToken(line, token);
    texCoord->push_back(std::stof(token));
  }

  texCoords.push_back(texCoord);
}

bool WavefrontObjReader::putFace(std::string &face, Glade::Mesh *mesh)
{
  std::string token;
  positionInsideTheLine = 0;
  std::vector<std::string> vertices;
  getNextToken(face, token);

  getNextToken(face, token);
  vertices.push_back(token);
  getNextToken(face, token);
  vertices.push_back(token);
  getNextToken(face, token);
  vertices.push_back(token);
  
// TODO: negative indices in OBJ file
  for (std::vector<std::string>::iterator vertexString = vertices.begin(); vertexString != vertices.end(); ++vertexString) {
    int positionIndex = 0, normalIndex = 0, texCoordIndex = 0;
    positionInsideTheLine = 0;
    getNextToken(*vertexString, token, '/');
    positionIndex = std::stof(token);
    positionIndex = positionIndex > 0 ? positionIndex - 1 : positionIndex;

    if (getNextToken(*vertexString, token, '/')) {
      texCoordIndex = std::stof(token);
      texCoordIndex = texCoordIndex > 0 ? texCoordIndex - 1 : texCoordIndex;
    } else {
      // what?
    }

    if (getNextToken(*vertexString, token, '/')) {
      normalIndex = std::stof(token);
      normalIndex = normalIndex > 0 ? normalIndex - 1 : normalIndex;
    } else {
      normalIndex = positionIndex;
    }
    
    std::map<std::string, int>::iterator vertexInMesh = vertexMap.find(*vertexString);
    int vertexIndexInMesh;
    
    if (vertexInMesh == vertexMap.end()) {
      vertexIndexInMesh = mesh->vertices.size() / GLADE_VERTEX_STRIDE_FLOATS;

      //log(">>> %f  %f  %f", positions[positionIndex]->at(0), positions[positionIndex]->at(1), positions[positionIndex]->at(2));
      if (positions.size() <= positionIndex) {
        log("Error reading Wavefront Obj format. The vertex position index %d in the face declaration is larger than the number of faces %d",
          positionIndex, positions.size());

        return false;
      }
      
      mesh->vertices.push_back(positions[positionIndex]->at(0));
      mesh->vertices.push_back(positions[positionIndex]->at(1));
      mesh->vertices.push_back(positions[positionIndex]->at(2));
      
      if (normals.size() > normalIndex) {
        mesh->vertices.push_back(normals[normalIndex]->at(0));
        mesh->vertices.push_back(normals[normalIndex]->at(1));
        mesh->vertices.push_back(normals[normalIndex]->at(2));
      } else {
        mesh->vertices.push_back(0.0f);
        mesh->vertices.push_back(0.0f);
        mesh->vertices.push_back(0.0f);
      }

      // vertex visibility flag
      mesh->vertices.push_back(1.0f);

      if (texCoords.size() > texCoordIndex) {
        mesh->vertices.push_back(texCoords[texCoordIndex]->at(0));
        mesh->vertices.push_back(texCoords[texCoordIndex]->at(1));
      } else {
        mesh->vertices.push_back(0.0f);
        mesh->vertices.push_back(0.0f);
      }

      vertexMap[*vertexString] = vertexIndexInMesh;
    } else {
      vertexIndexInMesh = vertexInMesh->second;
    }

    mesh->indices.push_back(vertexIndexInMesh);
  }

  return true;
}

bool WavefrontObjReader::read(const std::vector<char> &in, Glade::Mesh *mesh)
{
  positions.clear();
  normals.clear();
  texCoords.clear();
  faces.clear();
  std::string line, token;
  readPos = in.begin();

  while (getNextLine(in, line)) {
    positionInsideTheLine = 0;
    getNextToken(line, token);

    if (token == "v") {
      parseVertexPosition(line);
    } else if (token == "vt") {
      parseVertexTexCoord(line);
    } else if (token == "vn") {
      parseVertexNormal(line);
    } else if (token == "f") {
      faces.push_back(line);
    } else {
      continue;
    }
  }

  mesh->vertices.clear();
  mesh->indices.clear();

  // put everything into mesh
  for (std::vector<std::string>::iterator i = faces.begin(); i != faces.end(); ++i) {
    putFace(*i, mesh);
  }

  // clean up
  std::vector<std::vector<float> * >::iterator i;

  for (i = positions.begin(); i != positions.end(); ++i) {
    delete *i;
  }

  for (i = normals.begin(); i != normals.end(); ++i) {
    delete *i;
  }

  for (i = texCoords.begin(); i != texCoords.end(); ++i) {
    delete *i;
  }

  positions.clear();
  normals.clear();
  texCoords.clear();

  faces.clear();
  vertexMap.clear();

  return true;
}
