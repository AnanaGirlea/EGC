#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void Duck(float deltaTimeSeconds);
        void WingsDuck(float deltaTimeSeconds);
        void Collision(float deltaTimeSeconds);
        void Orientation(float deltaTimeSeconds);
        void FlyDirection(float deltaTimeSeconds);
        void ShootDuck();
        void EscapeDuck(float deltaTimeSeconds);
        void GameOver();
        void UpdateScore();
        void Restart();
        void FrameEnd() override;
        void ScenaryOptions();

        void GameStart(float deltaTimeSeconds);
        void GameFail(float deltaTimeSeconds);
        void GameWin(float deltaTimeSeconds);


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        
        float windowSizeX, windowSizeY;
        
        int dx, dy, topx, topy;
        float duckSize, cx, cy;
        int aux;

        gfxc::TextRenderer* textAbove, * textBelow, * textMiddle;
        int gameState;
        
        float score;
        float nr_bullets;
        float nr_lives;
        float clock;
        
        int randX, randY, randS;
        float angle;
        bool restart;
        bool game_over;

        float scaleX, scaleY;
        int fly;
        float speed, speedx, speedy;


        float colx, coly;
        int speed_wings;
        bool isDead, isEscape;
        bool change_direction;
        int degree;
        float trajectory;

        int playMode;

    };
}   // namespace m1
