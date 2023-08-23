#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
     public:
        Tema3();
        ~Tema3();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1), Texture2D* texture2 = NULL);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 lightPosition[2];
        glm::vec3 lightDirection[2];
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        // TODO(student): If you need any other class variables, define them here.

        std::unordered_map<std::string, Texture2D*> mapTextures;

        GLint light_mode;
		GLfloat angleOX, angleOY;
		GLfloat cut_off_angle;

    };
}   // namespace m1
