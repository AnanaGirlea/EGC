#include "models.h"

#include <vector>realPoints

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* models::createTrack(
    const string& name,
    glm::vec3 color,
    vector<VertexFormat> &pext,
    vector<VertexFormat> &pint,
    vector<VertexFormat> &realPoints)
{

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    //vector<VertexFormat> realPoints;
    int i, j;

    //initial points on geogebra
    vector<VertexFormat> initPoints
    {
        VertexFormat(glm::vec3(-11.24, 0, 9.85), color),   // A 1
        VertexFormat(glm::vec3(-7.57, 0, 10.08), color),
        VertexFormat(glm::vec3(-5.22, 0, 8.93), color),
        VertexFormat(glm::vec3(-2.95, 0, 7.89), color),
        VertexFormat(glm::vec3(-1.2, 0, 6.46), color),
        VertexFormat(glm::vec3(0.71, 0, 6.02), color),
        VertexFormat(glm::vec3(3.42, 0, 6.58), color),
        VertexFormat(glm::vec3(5.25, 0, 7.65), color),
        VertexFormat(glm::vec3(7.12, 0, 9.09), color),
        VertexFormat(glm::vec3(10.11, 0, 10.8), color),
        VertexFormat(glm::vec3(14.01, 0, 10.88), color),
        VertexFormat(glm::vec3(16.24, 0, 9.25), color),
        VertexFormat(glm::vec3(17.24, 0, 7.02), color),
        VertexFormat(glm::vec3(16.92, 0, 4.47), color),
        VertexFormat(glm::vec3(15.21, 0, 2.2), color),
        VertexFormat(glm::vec3(12.5, 0, 0.48), color),
        VertexFormat(glm::vec3(8.58, 0, -0.55), color),
        VertexFormat(glm::vec3(5.17, 0, -0.79), color),
        VertexFormat(glm::vec3(1.75, 0, -0.51), color),
        VertexFormat(glm::vec3(-1.84, 0, -0.31), color),
        VertexFormat(glm::vec3(-6, 0, 0), color),
        VertexFormat(glm::vec3(-9.21, 0, 0.72), color),
        VertexFormat(glm::vec3(-12.39, 0, 2.48), color),
        VertexFormat(glm::vec3(-13.19, 0, 5.98), color),   // Z 24
        VertexFormat(glm::vec3(-12.28, 0, 8.73), color),   // A1 25
    };

    // filling in the space betweeen the point on geogebra for points 1->24 and for 24->0
    // creating the actual set of points
    glm::vec3 P1 = glm::vec3(0, 0, 0);          // point 1
    glm::vec3 P2 = glm::vec3(0, 0, 0);          // point 2
    float dist = 0;                             // distance between P1 and P2
    glm::vec3 D = glm::vec3(0, 0, 0);           // direction of the segment
    glm::vec3 aux = glm::vec3(0, 0, 0);

    for (i = 0; i < 24; i++) {
        P1 = initPoints[i].position;
        P2 = initPoints[i + 1].position;

        D = normalize(P2 - P1);
        dist = sqrt(pow(P2.x - P1.x, 2) + pow(P2.z - P1.z, 2));

        for (j = 0; j < 100; j++) {
            aux = P1 + j * (dist / 100) * D;
            realPoints.push_back(VertexFormat(aux, color));
        }
    }

    P1 = initPoints[24].position;
    P2 = initPoints[0].position;

    D = normalize(P2 - P1);
    dist = sqrt(pow(P2.x - P1.x, 2) + pow(P2.z - P1.z, 2));

    for (i = 0; i < 100; i++) {
        aux = P1 + i * (dist / 100) * D;
        realPoints.push_back(VertexFormat(aux, color));
    }

    // generating D and P for the actual(real) points points (100*25)
    vector<VertexFormat> vint;                  // interior points
    vector<VertexFormat> vext;                  // exterior points
    glm::vec3 UP = glm::vec3(0, 1, 0);          // vector perpendicular on plane XOZ
    glm::vec3 P = glm::vec3(0, 0, 0);           // vector perpendicular on D
    VertexFormat R = VertexFormat(glm::vec3(0, 0, 0), color);   // interior point
    VertexFormat A = VertexFormat(glm::vec3(0, 0, 0), color);   // exterior point
    dist = 0.6;

    for (i = 0; i < realPoints.size() - 1; i++) {
        P1 = realPoints[i].position;
        P2 = realPoints[i + 1].position;

        D = P2 - P1;
        P = normalize(cross(D, UP));

        R = VertexFormat(P1 + dist * P, color);
        A = VertexFormat(P1 - dist * P, color);
        vint.push_back(R);
        vext.push_back(A);

        vertices.push_back(vext[i]);
    }
    P1 = realPoints[25 * 100 - 1].position;
    P2 = realPoints[0].position;

    D = P2 - P1;
    P = normalize(cross(D, UP));

    R = VertexFormat(P1 + dist * P, color);
    A = VertexFormat(P1 - dist * P, color);
    vint.push_back(R);
    vext.push_back(A);

    vertices.push_back(vext[realPoints.size() - 1]);

    // coping vint and vext
    pint = vint;
    pext = vext;

    // populateing vertices and indices

    Mesh* track = new Mesh(name);
    float size = realPoints.size();

    for (i = 0; i <= size - 1; i++)
    vertices.push_back(vint[i]);

    for (i = 0; i < size - 1; i++) {
        indices.push_back(i);
        indices.push_back(i + size);
        indices.push_back(i + size + 1);

        indices.push_back(i);
        indices.push_back(i + size + 1);
        indices.push_back(i + 1);
    }

    indices.push_back(size - 1);
    indices.push_back(2 * size - 1);
    indices.push_back(size);
    indices.push_back(size - 1);
    indices.push_back(size);
    indices.push_back(0);

    track->InitFromData(vertices, indices);
    return track;
}

Mesh* models::createGrass(
    const string& name,
    int L,
    glm::vec3 color)
{
    glm::vec3 corner = glm::vec3(-L, 0, -L);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(3 * L, 0, 0), color),
        VertexFormat(corner + glm::vec3(3 * L, 0, L * 3), color),
        VertexFormat(corner + glm::vec3(0, 0, L * 3), color)
    };

    // filling in the space betweeen the points of the square
    int i, j;
    int size = 100;

    for (i = 0; i < size; i++)
        for (j = 0; j < size; j++)
            vertices.push_back(VertexFormat(corner + glm::vec3(j, 0, i), color));

    Mesh* grass = new Mesh(name);

    vector<unsigned int> indices;
    for (int i = 0; i < size * (size - 1); i++) {
            indices.emplace_back(i);
            indices.emplace_back(i + 1);
            indices.emplace_back(i + size);

            indices.emplace_back(i + 1);
            indices.emplace_back(i + size + 1);
            indices.emplace_back(i + size);
    }

    grass->InitFromData(vertices, indices);
    return grass;
}

Mesh* models::createCube(
    const string& name,
    glm::vec3 color)
{

    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-1, -1, -1), color),
        VertexFormat(glm::vec3(1, -1, -1), color),
        VertexFormat(glm::vec3(1, 1, -1), color),
        VertexFormat(glm::vec3(-1, 1, -1), color),
        VertexFormat(glm::vec3(-1, -1, 1), color),
        VertexFormat(glm::vec3(1, -1, 1), color),
        VertexFormat(glm::vec3(1, 1, 1), color),
        VertexFormat(glm::vec3(-1, 1, 1), color)
    };

    const vector<unsigned int> indices =
    {
        0, 1, 2, 2, 3, 0, // Front
        1, 5, 6, 6, 2, 1, // Right
        5, 4, 7, 7, 6, 5, // Back
        4, 0, 3, 3, 7, 4, // Left
        3, 2, 6, 6, 7, 3, // Top
        4, 5, 1, 1, 0, 4  // Bottom
    };

    Mesh* tree = new Mesh(name);
    tree->InitFromData(vertices, indices);
    return tree;
}

Mesh* models::createSphere(const string& name, glm::vec3 color, float radius, int sectors, int stacks)
{
    vector<VertexFormat> vertices;

    for (int i = 0; i <= stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * glm::pi<float>() / stacks; 
        float y = radius * glm::sin(stackAngle);
        float xy = radius * glm::cos(stackAngle); 

        for (int j = 0; j <= sectors; ++j)
        {
            float sectorAngle = j * 2 * glm::pi<float>() / sectors; 
            float x = xy * glm::cos(sectorAngle);
            float z = xy * glm::sin(sectorAngle);

            glm::vec3 vertex = glm::vec3(x, y, z);
            vertices.push_back(VertexFormat(vertex, color));
        }
    }

    vector<unsigned int> indices;

    for (int i = 0; i < stacks; ++i)
    {
        int k1 = i * (sectors + 1); 
        int k2 = k1 + sectors + 1;

        for (int j = 0; j < sectors; ++j, ++k1, ++k2)
        {
            // Two triangles per sector, forming the spherical shape
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            if (i != stacks - 1)
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    Mesh* sphere = new Mesh(name);
    sphere->InitFromData(vertices, indices);
    return sphere;
}
