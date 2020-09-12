#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <math.h>
#include <bitset>
#include <map>

#include "stb_image.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

double lastX = 400.0, lastY = 300.0;

bool firstMouse{ true };

Camera camera{ cameraPos };

float screenRatio{ 800.0f / 600.0f };

float deltaTime = 0.0f;
float lastFrame = 0.0f;
bool outline{ true };
float alpha{ 1.0f };
bool r{ true }, g{ true }, b{ true };

int main()
{   
    //init glfw lib
    glfwInit();
    
    //set opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    //make opengl context before any glad call
    glfwMakeContextCurrent(window);

    //load the process function pointer in opengl
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    
    //set the view first two args for bottom left cornor of the rectangle of the view, the last two args for the top right one
    glViewport(0, 0, 800, 600);

    //register the callback function for the on-call function when resizing
    auto framebuffer_size_callback{ [](GLFWwindow* window,int width, int height) {glViewport(0, 0, width, height); if(width&&height)screenRatio = static_cast<float>(width) / height; } };
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    Shader shader("shaders/1.1.depth_testing.vs", "shaders/1.1.depth_testing.fs");

    std::vector<float> cubeVertices = {
        //vertexPos           textCoords
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right              
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left                
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right        
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left        
        // left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left       
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right      
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right          
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
        // bottom face          
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left        
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
        // top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right                 
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left  
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left              
    };
    
    /*Also make sure to add a call to OpenGL to specify that triangles defined by a clockwise ordering
        are now 'front-facing' triangles so the cube is rendered as normal :*/

    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);

    std::vector<float> planeVertices = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f
    };

    VertexArray cubeVAO{ cubeVertices };
    cubeVAO.bind();
    cubeVAO.addAttribute("pos", 3);
    cubeVAO.addAttribute("textCoords", 2);
    cubeVAO.unbind();

    VertexArray planeVAO{ planeVertices };
    planeVAO.bind();
    planeVAO.addAttribute("pos", 3);
    planeVAO.addAttribute("textCoords", 2);
    planeVAO.unbind();

    // load textures
    // -------------
    Texture::flipY(true);
    Texture cubeTexture{ "images/blending_transparent_window.png" };
    Texture cubeTexfloor{ "images/dirtTop.png" };

    std::vector<glm::vec3> vegetation;
    vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
    vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
    vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
    vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
    vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

    // shader configuration
    // --------------------
    shader.use();
    shader.setUniform("texture1", 0);

    Shader outlineShader{ "shaders/1.1.depth_testing.vs", "shaders/shaderSingleColor.fs" };

    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //redner loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        std::map<float, glm::vec3> sorted;
        for (unsigned int i = 0; i < vegetation.size(); i++)
        {
            float distance = glm::length(camera.m_Pos - vegetation[i]);
            sorted[distance] = vegetation[i];
        }


        // floor
        shader.use();
        glm::mat4 view = camera.m_Matrix;
        glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), screenRatio, 0.1f, 100.0f);
        planeVAO.bind();
        cubeTexfloor.bind(GL_TEXTURE0);
        glm::mat4 model = glm::mat4(1.0f);
        shader.setUniform("model", model);
        shader.setUniform("alpha", 1.0f);
        //glStencilMask(0x00);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // cubes
        shader.setUniform("view", view);
        shader.setUniform("projection", projection);
        cubeVAO.bind();
        cubeTexture.bind(GL_TEXTURE0);

        glStencilFunc(GL_ALWAYS, 1, 0x01);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0xFF);

        for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, it->second);
            shader.setUniform("model", model);
            shader.setUniform("alpha", alpha);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glStencilFunc(GL_NOTEQUAL, 1, 0x01);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);


        if (outline)
        {
            glm::vec3 outlineColor{ r, g, b };
            outlineShader.use();
            outlineShader.setUniform("view", view);
            outlineShader.setUniform("projection", projection);
            outlineShader.setUniform("alpha", 1.0f);
            for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
            {
                model = glm::mat4(1.0f);
                model = glm::translate(model, it->second);
                model = glm::scale(model, glm::vec3{ 1.05f });
                outlineShader.setUniform("model", model);
                outlineShader.setUniform("outlineColor", outlineColor);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clear resource used by glfw
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    float current{ static_cast<float>(glfwGetTime()) };
    static float last{ 0.0f };
    float dt{ current - last };
    last = current;
    if (glfwGetKey(window, GLFW_KEY_W)) camera.move(Camera::MoveDirection::FRONT, dt);
    if (glfwGetKey(window, GLFW_KEY_S)) camera.move(Camera::MoveDirection::BACK, dt);
    if (glfwGetKey(window, GLFW_KEY_A)) camera.move(Camera::MoveDirection::LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D)) camera.move(Camera::MoveDirection::RIGHT, dt);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(ypos - lastY); // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= 0.1f;
    yoffset *= 0.1f;

    xoffset > 0 ? camera.rotate(Camera::RotateDirection::RIGHT, xoffset): camera.rotate(Camera::RotateDirection::LEFT, -xoffset);
    yoffset < 0 ? camera.rotate(Camera::RotateDirection::UP, -yoffset) : camera.rotate(Camera::RotateDirection::DOWN, yoffset);

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    yoffset > 0 ? camera.zoom(static_cast<float>(yoffset)) : camera.zoom(static_cast<float>(yoffset));

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, true); return;
    case GLFW_KEY_LEFT_CONTROL:
        if (action == GLFW_PRESS) 
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetCursorPosCallback(window, nullptr);
        }
        return;
    case GLFW_KEY_LEFT_ALT:
        if (action == GLFW_PRESS)
        {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(window, mouse_callback);
        firstMouse = true;
        }
        return;
    case GLFW_KEY_O:
        if (action == GLFW_PRESS)
            outline = !outline;
        return;
    case GLFW_KEY_RIGHT:
        alpha += 0.01f;
        if (alpha >= 1.0f) alpha = 1.0f;
        return;
    case GLFW_KEY_LEFT:
        alpha -= 0.01f;
        if (alpha <= 0.0f) alpha = 0.0f;
        return;
    case GLFW_KEY_1:
        if (action == GLFW_PRESS)
            r = !r;
        return;
    case GLFW_KEY_2:
        if (action == GLFW_PRESS)
            g = !g;
        return;
    case GLFW_KEY_3:
        if (action == GLFW_PRESS)
            b = !b;
        return;
    }
}