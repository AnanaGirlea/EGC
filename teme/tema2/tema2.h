#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods);
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void CreateScenary(implemented::Camera* camera, const glm::mat4& projMatr);
        void CreateMiniViewportArea();
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera* camera, const glm::mat4& projMatr); 

        bool VerifyOnTrack();
        bool VerifyInTriangle(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 car);
       
     protected:
        glm::mat4 modelMatrix;
        float translateX, translateY, translateZ;
        float scaleX, scaleY, scaleZ;
        float angularStepOX, angularStepOY, angularStepOZ;
        GLenum polygonMode;

        glm::vec3 positionTree;

        ViewportArea miniViewportArea;
        glm::ivec2 resolution;

        float angle;

        float top, bottom, left, right;
        float fov;
        float znear, zfar;
        bool perspective = true;

        float cameraTranslateX = 6;
        float cameraTranslateY = 2;
        float cameraTranslateZ = 0;

        glm::vec3 camPoz;
        glm::vec3 camForw;
        glm::vec3 camUp;
        glm::vec3 camRight;

        bool isOnTrack;

        bool isOrtho = true;
        float orthoRight = 8.0f, orthoUp = 4.5f, orthoDown = -4.5f, orthoLeft = -8.0f;

        implemented::Camera* camera;
        implemented::Camera* miniCamera;
        glm::mat4 projectionMatrix, projectionMatrixMini;
        bool renderCameraTarget;

        float rotationAngle;

        glm::vector<VertexFormat> pext;
        glm::vector<VertexFormat> pint;
        glm::vector<VertexFormat> realPoints;

        const glm::vector<glm::vec3> pozTrees = {
            glm::vec3(-11.24, 0, 9.85),  //A1 25
            glm::vec3(-7.57, 0, 10.08),  // B1 26
            glm::vec3(-5.22, 0, 8.93),  //C1 27
            glm::vec3(-2.95, 0, 7.89), // D1 28
            glm::vec3(-1.2, 0, 6.46),  // E1 29
            glm::vec3(0.71, 0, 6.02), // F1 30
            glm::vec3(3.42, 0, 6.58),  // G1 31
            glm::vec3(5.25, 0, 7.65), // H1 32
            glm::vec3(7.12, 0, 9.09),  // I1 33
            glm::vec3(10.11, 0, 10.8),  // J1 34
            glm::vec3(14.01, 0, 10.88),//K1 35
            glm::vec3(16.24, 0, 9.25), // L1 36
            glm::vec3(17.24, 0, 7.02),  // M1 37
            glm::vec3(16.92, 0, 4.47),  // N1 38
            glm::vec3(15.21, 0, 2.2),  // O1 39
            glm::vec3(12.5, 0, 0.48),   // P1 40
            glm::vec3(8.58, 0, -0.55),   // Q 41
            glm::vec3(5.17, 0, -0.79),   // R 42
            glm::vec3(1.75, 0, -0.51),   // S 43
            glm::vec3(-1.84, 0, -0.31),   // T 44
            glm::vec3(-6, 0, 0),           // U 45
            glm::vec3(-9.21, 0, 0.72),   // V 46
            glm::vec3(-12.39, 0, 2.48),  // W 47
            glm::vec3(-13.19, 0, 5.98),   // Z 48
            glm::vec3(-12.28, 0, 8.73)   // A2 49
        };
    };
}   // namespace m1

