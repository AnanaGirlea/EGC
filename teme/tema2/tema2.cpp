#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/models.h"

#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/tema2/transform3D_tema2.h"
#include "lab_m1/tema2/models.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    pext = {};
    pint = {};
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    polygonMode = GL_FILL;

    translateX = 3;
    translateY = 2;
    translateZ = 0;

    cameraTranslateX += 0;
    cameraTranslateY += 0.5;
    cameraTranslateZ += 0;

    scaleX = 1;
    scaleY = 1;
    scaleZ = 1;

    angularStepOX = 0;
    angularStepOY = 0;
    angularStepOZ = 0;

    positionTree = glm::vec3(0, 0, 0);

    angle = 0;

    //create camera
    camera = new implemented::Camera();
    camera->Set(glm::vec3(translateX - 5, translateY, translateZ), glm::vec3(translateX, translateY, translateZ), glm::vec3(0, 1, 0));
    camera->distanceToTarget = 5;

   projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    //create minicamera
    miniCamera = new implemented::Camera();
    miniCamera->Set(glm::vec3(translateX, translateY + 10, translateZ), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    miniCamera->distanceToTarget = 10;

    // Sets the resolution of the small viewport
    resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    // Objects
    Mesh* grass = models::createGrass("grass", 100, glm::vec3(0, 0.8, 0));
    AddMeshToList(grass);

    Mesh* trunk = models::createCube("trunk", glm::vec3(0.3, 0.3, 0));
    Mesh* crown = models::createCube("crown", glm::vec3(0, 0.5, 0));
    AddMeshToList(trunk);
    AddMeshToList(crown);

    Mesh* trunk2 = models::createCube("trunk2", glm::vec3(0.5, 0.5, 0));
    Mesh* crown2 = models::createSphere("crown2", glm::vec3(1, 0, 1), 1.0f, 32, 16);
    AddMeshToList(trunk2);
    AddMeshToList(crown2);

    Mesh* bush = models::createCube("bush", glm::vec3(0, 1.2, 0));
    Mesh* bush2 = models::createCube("bush2", glm::vec3(0.5, 1, 0.5));
    AddMeshToList(bush);
    AddMeshToList(bush2);

    Mesh* track = models::createTrack("track", glm::vec3(0.5, 0.5, 0), pext, pint, realPoints);
    AddMeshToList(track);

    Mesh* car = models::createCube("car", glm::vec3(1, 0, 0));    
    AddMeshToList(car);
}

void Tema2::FrameStart()
{
    glClearColor(0, 0.5, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{   
    isOnTrack = true;
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    CreateScenary(camera, projectionMatrix);
    CreateMiniViewportArea();

    VerifyOnTrack();
    printf("isOnTrack = %d \n", isOnTrack);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera* camera, const glm::mat4& projMatr)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projMatr));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}

void Tema2::FrameEnd()
{
}

void Tema2::CreateScenary(implemented::Camera* camera, const glm::mat4& projMatr)
{
    // creates the track
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D_tema2::Scale(3, 3, 3);
    RenderMesh(meshes["track"], shaders["VertexColor"], modelMatrix, camera, projMatr);

    // creates the grass
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D_tema2::Translate(0, -0.1, 0);
    RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix, camera, projMatr);

    // creates trees
    for (int i = 0; i < 24; i++) {
        // normal trees
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0] * 4, 0, pozTrees[i][2]* 4);
        modelMatrix *= transform3D_tema2::Translate(1, 0, -2);
        modelMatrix *= transform3D_tema2::Scale(0.2, 2, 0.2);
        RenderMesh(meshes["trunk"], shaders["VertexColor"], modelMatrix, camera, projMatr);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0]* 4, 0, pozTrees[i][2] * 4);
        modelMatrix *= transform3D_tema2::Translate(1, 2, -2);
        modelMatrix *= transform3D_tema2::Scale(0.8, 0.8, 0.8);
        modelMatrix *= transform3D_tema2::Translate(0.0, 0.5, 0.0);
        RenderMesh(meshes["crown"], shaders["VertexColor"], modelMatrix, camera, projMatr);

        // sphere trees
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0] * 2, 0, pozTrees[i][2]  * 1.5);
        modelMatrix *= transform3D_tema2::Translate(1, 0, 2);
        modelMatrix *= transform3D_tema2::Scale(0.08, 0.6, 0.08);
        modelMatrix *= transform3D_tema2::Translate(0.0, 0, 0.0);
        RenderMesh(meshes["trunk2"], shaders["VertexColor"], modelMatrix, camera, projMatr);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0] * 2, 0, pozTrees[i][2] * 1.5);
        modelMatrix *= transform3D_tema2::Translate(1, 1.2, 2);
        modelMatrix *= transform3D_tema2::Scale(0.6, 0.6, 0.6);
        RenderMesh(meshes["crown2"], shaders["VertexColor"], modelMatrix, camera, projMatr);

        // bushes
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0] * 2, 0, pozTrees[i][2]);
        modelMatrix *= transform3D_tema2::Translate(1, 0, 4);
        modelMatrix *= transform3D_tema2::Scale(0.3, 0.7, 0.5);
        RenderMesh(meshes["bush"], shaders["VertexColor"], modelMatrix, camera, projMatr);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D_tema2::Translate(pozTrees[i][0] * 4 + 5, 0, pozTrees[i][2] * 5);
        modelMatrix *= transform3D_tema2::Translate(1, 0, -3);
        modelMatrix *= transform3D_tema2::Scale(1.2, 0.9, 0.9);
        RenderMesh(meshes["bush2"], shaders["VertexColor"], modelMatrix, camera, projMatr);
    }

    // creates the player's car
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D_tema2::Translate(translateX, 0.1, translateZ);
    modelMatrix *= transform3D_tema2::RotateOY(angle);
    modelMatrix *= transform3D_tema2::RotateOY(RADIANS(90));
    modelMatrix *= transform3D_tema2::Scale(0.3, 0.3, 0.5);
    RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix, camera, projMatr);
}

void Tema2::CreateMiniViewportArea()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    miniCamera->Set(glm::vec3(translateX, translateY + 20, translateZ), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    float left = -30;
    float right = 30;
    float bottom = -30;
    float top = 30;
    float zNear = 0.5;
    float zFar = 200;

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    glm::mat4 projectionOrto = glm::ortho(left, right, bottom, top, zNear, zFar);
    CreateScenary(miniCamera, projectionOrto);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

bool Tema2::VerifyInTriangle(glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 car) {

    // create vectors
    glm::vec3 v0 = P1 - P3;             // vector from P3 to P1
    glm::vec3 v1 = P2 - P3;             // vector from P3 to P2
    glm::vec3 v2 = car - P3;            // vector from P3 to car pozition

    // scalar products of above vectors
    float dot00 = glm::dot(v0, v0);
    float dot01 = glm::dot(v0, v1);
    float dot02 = glm::dot(v0, v2);
    float dot11 = glm::dot(v1, v1);
    float dot12 = glm::dot(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0) && (v >= 0) && (u + v < 1);
}


bool Tema2::VerifyOnTrack()
{
    glm::vec3 P1 = glm::vec3(0, 0, 0);          // triangle point 1 - base in pint
    glm::vec3 P2 = glm::vec3(0, 0, 0);          // triangle point 2 - base in pext
    glm::vec3 P3 = glm::vec3(0, 0, 0);          // triangle point 3 - point in pint
    glm::vec3 P4 = glm::vec3(0, 0, 0);          // triangle point 4 - point in pext
    glm::vec3 car = glm::vec3(translateX, translateY, translateZ); // actual pozition of the car

    isOnTrack = false;
    for (int i = 0; i < pint.size() - 1; i++) {
        P1 = pint[i].position;
        P2 = pext[i].position;

        P3 = pint[0].position;
        P4 = pext[0].position;
        if (VerifyInTriangle(P1, P2, P3, car))
            isOnTrack = true;
        if (VerifyInTriangle(P2, P3, P4, car))
            isOnTrack = true;

        P3 = pint[i + 1].position;
        P4 = pext[i + 1].position;
        if (VerifyInTriangle(P1, P2, P3, car))
            isOnTrack = true;
        if (VerifyInTriangle(P2, P3, P4, car))
            isOnTrack = true;

    }
    return isOnTrack;
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    int speed = 3.f;
    // when pressing W,A,S,D,R,F the first cube moves

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(speed * deltaTime);
        translateZ += camera->forward.z * speed * deltaTime;
        translateX += camera->forward.x * speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-speed * deltaTime);
        translateZ -= camera->forward.z * speed * deltaTime;
        translateX -= camera->forward.x * speed * deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        angle += 0.02;
        camera->RotateThirdPerson_OY(0.02);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        angle -= 0.02;
        camera->RotateThirdPerson_OY(-0.02);
    }

    // when pressing I,J,K,L the viewpoint moves

    if (window->KeyHold(GLFW_KEY_J))
        miniViewportArea.x -= 4;
    else if (window->KeyHold(GLFW_KEY_L))
        miniViewportArea.x += 4;

    if (window->KeyHold(GLFW_KEY_K))
        miniViewportArea.y -= 4;
    else if (window->KeyHold(GLFW_KEY_I))
        miniViewportArea.y += 4;

    if (window->KeyHold(GLFW_KEY_U)) {
        miniViewportArea.height -= 4;
        miniViewportArea.width -= 4;
    }
    else if (window->KeyHold(GLFW_KEY_O)) {
        miniViewportArea.height += 4;
        miniViewportArea.width += 4;
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE)
    {
        switch (polygonMode)
        {
        case GL_POINT:
            polygonMode = GL_FILL;
            break;
        case GL_LINE:
            polygonMode = GL_POINT;
            break;
        default:
            polygonMode = GL_LINE;
            break;
        }
    }
}



void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            camera->RotateFirstPerson_OX(-sensivityOX * deltaY);
            camera->RotateFirstPerson_OY(-sensivityOY * deltaX);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            camera->RotateThirdPerson_OX(-sensivityOX * deltaY);
            camera->RotateThirdPerson_OY(-sensivityOY * deltaX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
