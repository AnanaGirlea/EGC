#include "lab_m1/tema3/tema3.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

Tema3::Tema3() {}

Tema3::~Tema3() {}

void Tema3::Init() {
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ground.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(
            PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"),
            "quad.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("body");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("leg1");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("leg2");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the
    // normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3",
            "shaders", "VertexShader.glsl"),
            GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema3",
            "shaders", "FragmentShader.glsl"),
            GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    
    {
        lightPosition[0] = glm::vec3(0, 1, 1);
        lightDirection[0] = glm::vec3(0, -1, 0);
        lightPosition[1] = glm::vec3(0, 2.3, 0);
        lightDirection[1] = glm::vec3(0, -1, 0);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;

        light_mode = 0;
    }
}

void Tema3::FrameStart() {
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds) {

    float x = 0.5;
    float y = 1;
    float z = 0.2;
    
    // Render ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(-1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1), mapTextures["snow"]); 

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(-1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["body"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 0)); 

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x - 0.1, y, z - 0.05));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(-1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.7, 0));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["leg1"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));

    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(x + 0.1, y, z - 0.1));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(30.0f), glm::vec3(-1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1, 0.7, 0));
        // TODO(student): Add or change the object colors
        RenderSimpleMesh(meshes["leg2"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));

    }


    // Render the point light in the scene
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, lightPosition[0]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3));
        RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix);
    }

}

void Tema3::FrameEnd() { DrawCoordinateSystem(); }

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, Texture2D* texture) {
    if (!mesh || !shader || !shader->GetProgramID()) return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    int light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(light_position, 2, glm::value_ptr(lightPosition[0]));

    int light_direction =
        glGetUniformLocation(shader->program, "light_direction");
    glUniform3fv(light_direction, 2, glm::value_ptr(lightDirection[0]));

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color)
    int material_shininess =
        glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // TODO(student): Set any other shader uniforms that you need
    int light_type = glGetUniformLocation(shader->program, "light_type");
    glUniform1i(light_type, light_mode);


    int cut_off = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off, cut_off_angle);
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
        glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix =
        glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE,
        glm::value_ptr(projectionMatrix));

    if (texture)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        glActiveTexture(GL_TEXTURE0);
        // - bind the texture1 ID
        glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
        // - send theuniform value
        glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
    }

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()),
        GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema3::OnInputUpdate(float deltaTime, int mods) {
    float speed = 5;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W))
            lightPosition[0] -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A))
            lightPosition[0] -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S))
            lightPosition[0] += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D))
            lightPosition[0] += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) lightPosition[0] += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) lightPosition[0] -= up * deltaTime * speed;

        // TODO(student): Set any other keys that you might need
        if (window->KeyHold(GLFW_KEY_UP))
        {
            angleOX += deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_DOWN))
        {
            angleOX -= deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_LEFT))
        {
            angleOY += deltaTime * speed;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT))
        {
            angleOY -= deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_M))
        {
            cut_off_angle += deltaTime * 10;
        }
        if (window->KeyHold(GLFW_KEY_N))
        {
            cut_off_angle -= deltaTime * 10;
        }
    }
}

void Tema3::OnKeyPress(int key, int mods) {
    // Add key press event

    // TODO(student): Set keys that you might need
    if (key == GLFW_KEY_F) {
        light_mode ^= 1;
    }
}

void Tema3::OnKeyRelease(int key, int mods) {
    // Add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
    // Add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema3::OnWindowResize(int width, int height) {}