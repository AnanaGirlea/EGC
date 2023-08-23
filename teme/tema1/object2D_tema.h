#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_tema
{

    // Create square, triangle, circle and rectangle with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
    Mesh* CreateCircle(const std::string& name, glm::vec3 center, float radius, glm::vec3 color, int grade = 360);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);
    Mesh* CreateTriangleWing(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color);
}
