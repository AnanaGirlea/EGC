#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;


namespace models
{
    Mesh* createTrack(const string& name, glm::vec3 color, vector<VertexFormat> &pext, vector<VertexFormat> &pint, vector<VertexFormat> &realPoints);
    Mesh* createGrass(const string& name, int L, glm::vec3 color);
    Mesh* createCube(const string& name, glm::vec3 color);
    Mesh* createSphere(const string& name, glm::vec3 color, float radius, int sectors, int stacks);
}
