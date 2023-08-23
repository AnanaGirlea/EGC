#include "object2D_tema.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D_tema::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D_tema::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length * 1.5 , 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2};


    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D_tema::CreateCircle(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color,
    const int grade)
{
    int i = 0;
    float curr;
    float inc = 3.14 / static_cast<float>(grade);

    std::vector<VertexFormat> vertices;
    vertices.emplace_back(center, color);

    std::vector<unsigned int> indices;
    indices.push_back(i++);


    for (curr = 0; curr <= 2 * 3.14 + 1; curr += inc) {
        vertices.emplace_back(center + glm::vec3(radius * cos(curr), radius * sin(curr), 0), color);
        indices.push_back(i++);
    }

    Mesh* circle = new Mesh(name);
    circle->SetDrawMode(GL_TRIANGLE_FAN);
    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D_tema::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, width, 0), color),
        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }
   
    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D_tema::CreateTriangleWing(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length , 0), color),
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    triangle->InitFromData(vertices, indices);
    return triangle;
}