#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D_tema.h"
#include "lab_m1/tema1/object2D_tema.h"

using namespace std;
using namespace m1;



/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

#define MAXSCORE 150
#define MAXTIME 5

Tema1::Tema1()
{
    duckSize = 100;
    cx = duckSize / 2;
    cy = duckSize / 2;

    aux = 0;

    topx = 1100;
    topy = 690;

    score = 0;
    nr_bullets = 3;
    nr_lives = 3;
    clock = 0;
    playMode = 0;

    gameState = 0;
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    angularStep = 0;
    randX = rand() % 900;
    randY = rand() % 400;
    angle = M_PI;

    colx = 1;
    coly = 1;
    speed_wings = 1;
    isDead = false;
    isEscape = false;
    change_direction = false;

    restart = false;
    game_over = false;

    speed = 200;

    speedx = 200;
    speedy = 200;

    glm::vec3 bodyPoz = glm::vec3(0, 0, 1);

    glm::vec3 wing1Poz = glm::vec3(- 25, 25, 1);
    glm::vec3 wing2Poz = glm::vec3(duckSize, 25, 1);

    glm::vec3 headPoz = glm::vec3(duckSize, duckSize * 1.5, 2);
    glm::vec3 beakPoz = glm::vec3(duckSize, 25 + duckSize * 1.5, 1);

    glm::vec3 livesPoz = glm::vec3(topx + 10, topy, 1);
    glm::vec3 bulletsPoz = glm::vec3(topx, topy - 60, 1);
    glm::vec3 scorePoz = glm::vec3(topx, topy - 100, 1);

    glm::vec3 grassPoz = glm::vec3(0, 0, 2);

    windowSizeX = resolution.x;
    windowSizeY = resolution.y;

    // duck
    Mesh* body = object2D_tema::CreateTriangle("body", bodyPoz, duckSize, glm::vec3(0.55, 0.55, 0.55));
    Mesh* wing1 = object2D_tema::CreateTriangleWing("wing1", wing1Poz, duckSize / 1.5, glm::vec3(0.55, 0.55, 0.55));
    Mesh* wing2 = object2D_tema::CreateTriangleWing("wing2", wing2Poz, duckSize / 1.5, glm::vec3(0.55, 0.55, 0.55));
    Mesh* head = object2D_tema::CreateCircle("head", headPoz, duckSize / 2.5, glm::vec3(0.224, 0.3, 0.271));
    Mesh* beak = object2D_tema::CreateTriangle("beak", beakPoz, duckSize / 4, glm::vec3(1, 0.5, 0));
    AddMeshToList(body);
    AddMeshToList(wing1);
    AddMeshToList(wing2);
    AddMeshToList(head);
    AddMeshToList(beak);

    //lives
    Mesh* life1 = object2D_tema::CreateCircle("life1", livesPoz, 20, glm::vec3(1, 0, 0));
    Mesh* life2 = object2D_tema::CreateCircle("life2", livesPoz + glm::vec3(50, 0, 0), 20, glm::vec3(1, 0, 0));
    Mesh* life3 = object2D_tema::CreateCircle("life3", livesPoz + glm::vec3(100, 0, 0), 20, glm::vec3(1, 0, 0));
    AddMeshToList(life1);
    AddMeshToList(life2);
    AddMeshToList(life3);

    //bullets
    Mesh* bullet1 = object2D_tema::CreateRectangle("bullet1", bulletsPoz, 20, 30, glm::vec3(0.5, 1, 0.3), true);
    Mesh* bullet2 = object2D_tema::CreateRectangle("bullet2", bulletsPoz + glm::vec3(50, 0, 0), 20, 30, glm::vec3(0.5, 1, 0.3), true);
    Mesh* bullet3 = object2D_tema::CreateRectangle("bullet3", bulletsPoz + glm::vec3(100, 0, 0), 20, 30, glm::vec3(0.5, 1, 0.3), true);
    AddMeshToList(bullet1);
    AddMeshToList(bullet2);
    AddMeshToList(bullet3);
    
    // score
    Mesh* scoreOutline = object2D_tema::CreateRectangle("scoreOutline", scorePoz, 150, 30, glm::vec3(0, 0, 1), false);
    Mesh* scoreFill = object2D_tema::CreateRectangle("scoreFill", scorePoz + glm::vec3(0, 0 , 1), score, 30, glm::vec3(0, 0, 1), true);
    AddMeshToList(scoreOutline);
    AddMeshToList(scoreFill);

    //grass
    Mesh* grass = object2D_tema::CreateRectangle("grass", grassPoz, windowSizeX, windowSizeY / 5, glm::vec3(0, 0.6, 0), true);
    AddMeshToList(grass);

    // initialize text
    textAbove = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textMiddle = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textBelow = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    
    textAbove->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 60);
    textMiddle->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 30);
    textBelow->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 40);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 1, 1, 0);
    ScenaryOptions();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Tema1::Update(float deltaTimeSeconds)
{
    if (gameState == 0) {
        GameStart(deltaTimeSeconds);
    }
    else if (gameState == 2) {
        GameWin(deltaTimeSeconds);
    }
    else if (gameState == 3) {
        GameFail(deltaTimeSeconds);
    }
    else {
        translateX += deltaTimeSeconds;
        translateY += deltaTimeSeconds;

        //keeping track of time spent playing
        clock += deltaTimeSeconds;
        printf("is dead? %d \n", isDead);
        if (isDead && translateY <= 0)
            Init();

        if (isEscape && translateY >= 500)
            Init();

        if (clock >= 4)
            EscapeDuck(deltaTimeSeconds);

        modelMatrix = glm::mat3(1);

        //grass at the bottom of the window
        RenderMesh2D(meshes["grass"], shaders["VertexColor"], modelMatrix);

        //lives
        if (nr_lives < 1)
            gameState = 3;
        else
            RenderMesh2D(meshes["life1"], shaders["VertexColor"], modelMatrix);
        if (nr_lives >= 2)
            RenderMesh2D(meshes["life2"], shaders["VertexColor"], modelMatrix);
        if (nr_lives >= 3)
            RenderMesh2D(meshes["life3"], shaders["VertexColor"], modelMatrix);

        //bullets
        if (nr_bullets < 1)
            EscapeDuck(deltaTimeSeconds);
        else
            RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
        if (nr_bullets >= 2)
            RenderMesh2D(meshes["bullet2"], shaders["VertexColor"], modelMatrix);
        if (nr_bullets >= 3)
            RenderMesh2D(meshes["bullet3"], shaders["VertexColor"], modelMatrix);

        //score
        RenderMesh2D(meshes["scoreOutline"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["scoreFill"], shaders["VertexColor"], modelMatrix);

        ScenaryOptions();

        Orientation(deltaTimeSeconds);
        FlyDirection(deltaTimeSeconds);
    }
}

void Tema1::Duck(float deltaTimeSeconds) 
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D_tema::Translate(translateX, translateY);
    modelMatrix *= transform2D_tema::Translate(randX, randY);
    modelMatrix *= transform2D_tema::Rotate(angle);

    RenderMesh2D(meshes["body"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["head"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["beak"], shaders["VertexColor"], modelMatrix);

    WingsDuck(deltaTimeSeconds);
    RenderMesh2D(meshes["wing1"], shaders["VertexColor"], modelMatrix);
    RenderMesh2D(meshes["wing2"], shaders["VertexColor"], modelMatrix);
}

void Tema1::WingsDuck(float deltaTimeSeconds)
{
    if (aux == 0) {
        if (angularStep < 0.3) {
            angularStep += deltaTimeSeconds;
        }
        else if (angularStep >= 0.3) {
            aux = 1;
            angularStep -= deltaTimeSeconds;
        }
    }

    if (aux == 1) {
        if (angularStep <= 0) {
            angularStep += deltaTimeSeconds;
            aux = 0;
        }
        else if (angularStep > 0) {
            angularStep -= deltaTimeSeconds;
        }

    }

    modelMatrix *= transform2D_tema::Rotate(speed_wings * angularStep);
}

void Tema1::Collision(float deltaTimeSeconds)
{
    if (isEscape) {
        coly = 1;
        return;
    }

    if (translateX >= 500) {
        colx = -1;
    }
    else if (translateX <= -450 ) {
        colx = 1;
    }

    if (translateY >= 450) {
        coly = -1;
    }
    else if (translateY <= 0) {
        coly = 1;
    }
}

void Tema1::Orientation(float deltaTimeSeconds)
{
    if (colx == 1 && coly == 1)
        angle = M_PI * 315 / 180;

    else if (colx == -1 && coly == -1)
        angle = M_PI * 135 / 180;

    else if (colx == -1 && coly == 1)
        angle = M_PI * 45 / 180; 

    else if (colx == 1 && coly == -1)
        angle = M_PI * 225 / 180;
}

void Tema1::FlyDirection(float deltaTimeSeconds)
{
    
    Collision(deltaTimeSeconds);
    Orientation(deltaTimeSeconds);

    if (fly == 0) {
        translateY += coly * deltaTimeSeconds * speed;
    }
    else if (isDead) {
        translateY += coly * deltaTimeSeconds * speed;
    }
    else {
        translateX += colx * deltaTimeSeconds * speedx;
        translateY += coly * deltaTimeSeconds * speedy;
    }

    Duck(deltaTimeSeconds);
}

void Tema1::ShootDuck()
{
    if (!isEscape) {
        score += 15;
        clock = 0;
        nr_bullets = 3;

        angle = M_PI;
        colx = 0;
        coly = -1;
        speed_wings = 0;
        isDead = true;

        if (score < 150)
            UpdateScore();
        else gameState = 2;
    }
}

void Tema1::EscapeDuck(float deltaTimeSeconds)
{
    nr_lives--;
    angle = 0;
    colx = 0;
    coly = 1;
    speed_wings = 2;
    isEscape = true;

    if (nr_lives == 0) {
        gameState = 3;
    }
    else {
        clock = 0;
        nr_bullets = 3;
        randX = rand() % 900;
        randY = rand() % 400;
        Orientation(deltaTimeSeconds);
        FlyDirection(0);
    }
}

void Tema1::GameOver()
{
    if (nr_lives == 0) {
        gameState = 3;
    } 

    if (score >= 150) {
        gameState = 2;
    }
}

void Tema1::Restart()
{
    score = 0;
    clock = 0;
    speed_wings = 1;
    nr_bullets = 3;
    nr_lives = 3;
}

void Tema1::UpdateScore()
{
    glm::vec3 scorePoz = glm::vec3(topx, topy - 100, 0);
    
    Mesh* scoreFill = object2D_tema::CreateRectangle("scoreFill", scorePoz + glm::vec3(0, 0, 1), score, 30, glm::vec3(0, 0, 1), true);
    AddMeshToList(scoreFill);
    
    modelMatrix = glm::mat3(1);
    RenderMesh2D(meshes["scoreFill"], shaders["VertexColor"], modelMatrix);
}

void Tema1::GameStart(float deltaTimeSeconds)
{
    textAbove->RenderText("DUCK HUNT", windowSizeX / 2 - 150, windowSizeY / 2 - 225, 1.0f, glm::vec3(0, 0, 0));
    translateX = 0;
    translateY = 0;
    angle = 45;
    randX = windowSizeX / 2;
    randY = windowSizeY / 2 - 75;
    Duck(deltaTimeSeconds);
    textMiddle->RenderText("Hunt the duck to earn points! Shoot with a just a click!", 150, windowSizeY / 2 + 150, 1.0f, glm::vec3(0, 0, 0));
    textBelow->RenderText("Press ENTER to enter game", windowSizeX / 2 - 250, windowSizeY / 2 + 250, 1.0f, glm::vec3(0, 0, 0));
    
    Restart();
}

void Tema1::GameFail(float deltaTimeSeconds)
{
    textAbove->RenderText("GAME LOST :(", windowSizeX / 2 - 175, windowSizeY / 2 - 225, 1.0f, glm::vec3(0, 0, 0));
    translateX = 0;
    translateY = 0;
    angle = 45;
    randX = windowSizeX / 2;
    randY = windowSizeY / 2 - 75;
    Duck(deltaTimeSeconds);
    textBelow->RenderText("Press ENTER to restart game", windowSizeX / 2 - 275, windowSizeY / 2 + 200, 1.0f, glm::vec3(0, 0, 0));
    
    Restart();
}

void Tema1::GameWin(float deltaTimeSeconds)
{
    textAbove->RenderText("GAME WON!", windowSizeX / 2 - 175, windowSizeY / 2 - 225, 1.0f, glm::vec3(0, 0, 0));
    translateX = 0;
    translateY = 0;
    angle = 45;
    randX = windowSizeX / 2;
    randY = windowSizeY / 2 - 75;
    Duck(deltaTimeSeconds);
    textBelow->RenderText("Press ENTER to restart game", windowSizeX / 2 - 275, windowSizeY / 2 + 200, 1.0f, glm::vec3(0, 0, 0));
    
    Restart();
}


void Tema1::FrameEnd()
{
}

void Tema1::ScenaryOptions()
{
    if (playMode == 1) {
        glClearColor(0, 0.9, 1, 0);

        Mesh* sun = object2D_tema::CreateCircle("sun", glm::vec3(150, 600, 0), 70, glm::vec3(0.9, 0.9, 0));
        AddMeshToList(sun);
        RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);

        Mesh* cloud10 = object2D_tema::CreateCircle("cloud10", glm::vec3(120, 400, 0), 50, glm::vec3(1, 1, 1));
        Mesh* cloud11 = object2D_tema::CreateCircle("cloud11", glm::vec3(160, 410, 0), 60, glm::vec3(1, 1, 1));
        Mesh* cloud12 = object2D_tema::CreateCircle("cloud12", glm::vec3(200, 400, 0), 50, glm::vec3(1, 1, 1));
        AddMeshToList(cloud10);
        AddMeshToList(cloud11);
        AddMeshToList(cloud12);
        RenderMesh2D(meshes["cloud10"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud11"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud12"], shaders["VertexColor"], modelMatrix);

        Mesh* cloud7 = object2D_tema::CreateCircle("cloud7", glm::vec3(400, 550, 0), 50, glm::vec3(1, 1, 1));
        Mesh* cloud8 = object2D_tema::CreateCircle("cloud8", glm::vec3(440, 560, 0), 60, glm::vec3(1, 1, 1));
        Mesh* cloud9 = object2D_tema::CreateCircle("cloud9", glm::vec3(480, 550, 0), 50, glm::vec3(1, 1, 1));
        AddMeshToList(cloud7);
        AddMeshToList(cloud8);
        AddMeshToList(cloud9);
        RenderMesh2D(meshes["cloud7"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud8"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud9"], shaders["VertexColor"], modelMatrix);

        Mesh* cloud4 = object2D_tema::CreateCircle("cloud4", glm::vec3(630, 330, 0), 50, glm::vec3(1, 1, 1));
        Mesh* cloud5 = object2D_tema::CreateCircle("cloud5", glm::vec3(670, 340, 0), 60, glm::vec3(1, 1, 1));
        Mesh* cloud6 = object2D_tema::CreateCircle("cloud6", glm::vec3(710, 330, 0), 50, glm::vec3(1, 1, 1));
        AddMeshToList(cloud4);
        AddMeshToList(cloud5);
        AddMeshToList(cloud6);
        RenderMesh2D(meshes["cloud4"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud5"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud6"], shaders["VertexColor"], modelMatrix);

        Mesh* cloud1 = object2D_tema::CreateCircle("cloud1", glm::vec3(960, 500, 0), 50, glm::vec3(1, 1, 1));
        Mesh* cloud2 = object2D_tema::CreateCircle("cloud2", glm::vec3(1000, 510, 0), 60, glm::vec3(1, 1, 1));
        Mesh* cloud3 = object2D_tema::CreateCircle("cloud3", glm::vec3(1040, 500, 0), 50, glm::vec3(1, 1, 1));
        AddMeshToList(cloud1);
        AddMeshToList(cloud2);
        AddMeshToList(cloud3);
        RenderMesh2D(meshes["cloud1"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["cloud3"], shaders["VertexColor"], modelMatrix);
    }

    if (playMode == 2) {
        glClearColor(0, 0.2, 1, 0);
        
        Mesh* moon = object2D_tema::CreateCircle("moon", glm::vec3(150, 600, 0), 70, glm::vec3(0.7, 0.7, 7));
        AddMeshToList(moon);
        RenderMesh2D(meshes["moon"], shaders["VertexColor"], modelMatrix);

        Mesh* mountain = object2D_tema::CreateTriangle("mountain", glm::vec3(1000, 140, 0), 300, glm::vec3(0.2, 0.2, 0.2));
        AddMeshToList(mountain);
        RenderMesh2D(meshes["mountain"], shaders["VertexColor"], modelMatrix);

        Mesh* tree1 = object2D_tema::CreateTriangle("tree1", glm::vec3(-40, 140, 0), 120, glm::vec3(0, 0.55, 0));
        Mesh* tree2 = object2D_tema::CreateTriangle("tree2", glm::vec3(10, 140, 0), 200, glm::vec3(0, 0.6, 0));
        Mesh* tree3 = object2D_tema::CreateTriangle("tree3", glm::vec3(110, 140, 0), 170, glm::vec3(0, 0.55, 0));
        Mesh* tree4 = object2D_tema::CreateTriangle("tree4", glm::vec3(190, 140, 0), 190, glm::vec3(0, 0.6, 0));
        Mesh* tree5 = object2D_tema::CreateTriangle("tree5", glm::vec3(330, 140, 0), 120, glm::vec3(0, 0.55, 0));
        AddMeshToList(tree1);
        AddMeshToList(tree2);
        AddMeshToList(tree3);
        AddMeshToList(tree4);
        AddMeshToList(tree5);
        RenderMesh2D(meshes["tree1"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tree2"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tree3"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tree4"], shaders["VertexColor"], modelMatrix);
        RenderMesh2D(meshes["tree5"], shaders["VertexColor"], modelMatrix);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (playMode)
        {
        case 1:
            playMode = 2;
            break;
        case 2:
            playMode = 0;
            break;
        default:
            playMode = 1;
            break;
        }
    }

    if (key == GLFW_KEY_ENTER)
    {
        switch (gameState)
        {
        case 0:
            gameState = 1;  //from start screen to game
            break;
        case 1:
            gameState = 3;  //from playng game to lose game
            break;
        case 2:
            gameState = 0;  //from game won to start screen
            break;
        case 3:
            gameState = 0;  //from game lose to start screen
            break;
        }
    }
}

void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    nr_bullets--;

    int hitboxN = translateY + randY + duckSize * 2;
    int hitboxS = translateY + randY - 50;
    int hitboxE = translateX + randX + duckSize + 100;
    int hitboxW = translateX + randX - 100;

    float actualY = windowSizeY - mouseY;

    printf("coord x = %f coord y = %f \n", translateX + randX, translateY + randY);
    printf("mouse x = %d mouse y = %d actualY = %f\n", mouseX, mouseY, actualY);
    if ((mouseX > hitboxW) && (mouseX < hitboxE)) {
        if ((actualY > hitboxS) && (actualY < hitboxN)) {
            ShootDuck();
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
