//
//  main.cpp
//  Challenge 2
//
//  Created by Luis Marcelo Flores Canales on 10/13/19.
//  José Luis Felán Villaseñor A00821099
//  César Buenfil Vázquez A01207499
//  Copyright © 2019 Luis Marcelo Flores Canales. All rights reserved.
//
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000

using namespace std;

void keyMovement(GLFWwindow * window, int key, int scancode, int action, int mods);
void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength);

//Para encontrar el punto (0, 0) en la pantalla
GLfloat halfScreenWidth = SCREEN_WIDTH / 2;
GLfloat halfScreenHeight = SCREEN_HEIGHT / 2;
GLfloat halfScreenDepth = -500;
//El punto de rotacion de la figura
GLfloat rotationX = 0.0f;
GLfloat rotationY = 0.0f;
/* GLfloat color[] =
 {
 255,0,0,
 0,255,0,
 0,0,255
 }; */
//El punto donde se encuentra la figura de forma inicial
GLfloat moveX = SCREEN_WIDTH/2;
GLfloat moveY = SCREEN_HEIGHT/2;
GLfloat moveZ = SCREEN_WIDTH/2;
GLfloat edge_length = 10;
GLenum mode = GL_QUADS;
GLenum fill_mode = GL_FILL;

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snail", NULL, NULL);

    //Funcion que detecta cuando el teclado es presionado
    glfwSetKeyCallback(window, keyMovement);

    //??
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glViewport(0.0f, 0.0f, screenWidth, screenHeight); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
    glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
    glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1000); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
    glLoadIdentity(); // same as above comment

    cout << "Key up, down, left and right: Rotate object\n";
    cout << "Key W, A, S, D: Move object\n";
    cout << "Key J, K: Move Z axis, doesnt have an effect\n";
    cout << "Key N and M: Scale object\n";
    cout << "Key X: Render points\n";
    cout << "Key C: Render lines\n";
    cout << "Key V: Render triangles\n";
    cout << "Key B: Render quads\n";
    cout << "Key Z: Toggle between lines and fill\n";

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render OpenGL here
        glPopMatrix();

        glPushMatrix();
        glTranslatef(halfScreenWidth, halfScreenHeight, halfScreenDepth);
        glRotatef(rotationX, 1, 0, 0);
        glRotatef(rotationY, 0, 1, 0);
        glTranslatef(-halfScreenWidth, -halfScreenHeight, -halfScreenDepth);
        // glScalef();

        DrawCube(halfScreenWidth, halfScreenHeight, halfScreenDepth, edge_length);
        glPopMatrix();


        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}



void keyMovement(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //std::cout << key << std::endl;

    const GLfloat rotationSpeed = 10;
    const GLfloat moveSpeed = 10;

    // actions are GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
            rotationX -= rotationSpeed;
            break;
            case GLFW_KEY_DOWN:
            rotationX += rotationSpeed;
            break;
            case GLFW_KEY_RIGHT:
            rotationY += rotationSpeed;
            break;
            case GLFW_KEY_LEFT:
            rotationY -= rotationSpeed;
            break;
            case GLFW_KEY_W:
            halfScreenHeight = moveSpeed + halfScreenHeight;
            break;
            case GLFW_KEY_S:
            halfScreenHeight = halfScreenHeight - moveSpeed;
            break;
            case GLFW_KEY_D:
            halfScreenWidth = moveSpeed + halfScreenWidth;
            break;
            case GLFW_KEY_J:
            halfScreenDepth += moveSpeed;
            break;
            case GLFW_KEY_K:
            halfScreenDepth -= moveSpeed;
            break;
            case GLFW_KEY_A:
            halfScreenWidth = halfScreenWidth - moveSpeed;
            break;
            case GLFW_KEY_N:
            edge_length -= 1;
            break;
            case GLFW_KEY_M:
            edge_length += 1;
            break;
            case GLFW_KEY_X:
            mode = GL_POINTS;
            break;
            case GLFW_KEY_C:
            mode = GL_LINES;
            break;
            case GLFW_KEY_V:
            mode = GL_TRIANGLES;
            break;
            case GLFW_KEY_B:
            mode = GL_QUADS;
            break;
            case GLFW_KEY_Z:
            if(fill_mode == GL_FILL){
                fill_mode = GL_LINE;
            } else {
                fill_mode = GL_FILL;
            }
            break;
        }


    }
}

void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat edgeLength)
{
    // Face
    GLfloat faceWidthLength = edgeLength * 5.0f; // y
    GLfloat faceHeigthLength = edgeLength * 10.0f; // x
    // Body
    GLfloat bodyWidthLength = edgeLength * 5.0f; // y
    GLfloat bodyHeigthLength = edgeLength  * 20.0f; // z
    // Shell
    GLfloat shellWidthLength = faceHeigthLength * 0.9f; // x
    GLfloat shellHeigthLength = edgeLength * 20.0f; // y
    GLfloat shellDepthLength = bodyHeigthLength * 0.9f; // z
    // Shell Draw
    // Eye Length
    GLfloat eyeLengthWidthLength = faceHeigthLength * 0.4f; // x
    GLfloat eyeLengthHeigthLength = edgeLength * 10.0f; // y
    GLfloat eyeLengthDepthLength = bodyHeigthLength - edgeLength; // z
    // Eye
    GLfloat eyeWidthLength = faceHeigthLength * 0.2f; // x
    GLfloat eyeHeigthLength = edgeLength * 10.0f; // y
    GLfloat eyeDepthLength = bodyHeigthLength - (edgeLength * 3.0f); // z
    // Mouth
    GLfloat mouthWidthLength = faceHeigthLength * 0.6f; // x
    GLfloat mouthHeigthLength = edgeLength * 10.0f; // y
    GLfloat mouthDepthLength = bodyHeigthLength + 0.1f; // z
    // Pupil
    GLfloat pupilWidthLength = faceHeigthLength * 0.6f; // x
    GLfloat pupilHeigthLength = edgeLength * 10.0f; // y
    GLfloat pupilDepthLength = bodyHeigthLength + 0.1f; // z


    GLfloat vertices[] = {
        // Face ////////////////////////////////////
        // front face
        centerPosX - faceHeigthLength, centerPosY + faceWidthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX + faceHeigthLength, centerPosY + faceWidthLength, centerPosZ + bodyHeigthLength, // top right
        centerPosX + faceHeigthLength, centerPosY - faceWidthLength, centerPosZ + bodyHeigthLength, // bottom right
        centerPosX - faceHeigthLength, centerPosY - faceWidthLength, centerPosZ + bodyHeigthLength, // bottom left
        // back face
        centerPosX - faceHeigthLength, centerPosY + faceWidthLength, centerPosZ - bodyHeigthLength, // top left
        centerPosX + faceHeigthLength, centerPosY + faceWidthLength, centerPosZ - bodyHeigthLength, // top right
        centerPosX + faceHeigthLength, centerPosY - faceWidthLength, centerPosZ - bodyHeigthLength, // bottom right
        centerPosX - faceHeigthLength, centerPosY - faceWidthLength, centerPosZ - bodyHeigthLength, // bottom left

        // Body ////////////////////////////////////////
        // left face
        centerPosX - faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX - faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ - bodyHeigthLength, // top right
        centerPosX - faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ - bodyHeigthLength, // bottom right
        centerPosX - faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ + bodyHeigthLength, // bottom left

        // right face
        centerPosX + faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX + faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ - bodyHeigthLength, // top right
        centerPosX + faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ - bodyHeigthLength, // bottom right
        centerPosX + faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ + bodyHeigthLength, // bottom left

        // top face
        centerPosX - faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX - faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ - bodyHeigthLength, // top right
        centerPosX + faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ - bodyHeigthLength, // bottom right
        centerPosX + faceHeigthLength, centerPosY + bodyWidthLength, centerPosZ + bodyHeigthLength, // bottom left

        // bottom face
        centerPosX - faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX - faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ - bodyHeigthLength, // top right
        centerPosX + faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ - bodyHeigthLength, // bottom right
        centerPosX + faceHeigthLength, centerPosY - bodyWidthLength, centerPosZ + bodyHeigthLength,  // bottom left

        // Shell/////////////////////////////////////////////
        // Front face
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top left
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top right
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // bottom right
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // bottom left

        // Back face
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top left
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top right
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // bottom right
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // bottom left

        // top face
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top left back
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top right back
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top right front
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top left front

        // bottom face
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // top left back
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // top right back
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // top right front
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // top left front

        // Right side
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top left back
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // bottom left back
        centerPosX - shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // bottom left front
        centerPosX - shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top left front

        // left side
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ - shellDepthLength, // top right
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ - shellDepthLength, // bottom right
        centerPosX + shellWidthLength, centerPosY + faceWidthLength, centerPosZ + (shellDepthLength * .18f), // bottom right
        centerPosX + shellWidthLength, centerPosY + faceWidthLength + shellHeigthLength, centerPosZ + (shellDepthLength * .18f), // top right


        // Shell Draw /////////////////////////

        // Eye Length /////////////////////////
        // Left //
        // front face
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom right
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom left

        // back face
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom right
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom left

        // top face
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // bottom face
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // right side
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom left
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom left
        centerPosX - eyeLengthWidthLength - (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // left side
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom right
        centerPosX - eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom right
        centerPosX - eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right


        // Right //
        // front face
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom right
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom left

        // back face
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom right
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom left

        // top face
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // bottom face
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // right side
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top left
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom left
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom left
        centerPosX + eyeLengthWidthLength + (faceHeigthLength * 0.2f), centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top left

        // Left side
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength, // top right
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength, // bottom right
        centerPosX + eyeLengthWidthLength, centerPosY + faceWidthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // bottom right
        centerPosX + eyeLengthWidthLength, centerPosY + eyeLengthHeigthLength, centerPosZ + eyeLengthDepthLength - edgeLength, // top right


        // Eye /////////////////////////
        // Left //
        // front face
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // bottom right
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // bottom left

        // back face
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // top left
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // top right
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom left

        // top face
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom right
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom left

        // bottom face
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top right
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom left

        // left face
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // bottom left
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX - (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom left

        //right face
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX - eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom right
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX - eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top right

        // Right //
        // front face
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // bottom right
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // bottom left

        // back face
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // top left
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // top right
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom left

        // top face
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top left
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom right
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom left

        // bottom face
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top left
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top right
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom left

        // left face
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength + 0.1f, // top left
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength + 0.1f, // bottom left
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength + 0.1f, // top left
        centerPosX + (eyeWidthLength * 4.0f), centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength + 0.1f, // bottom left

        //right face
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + bodyHeigthLength, // top right
        centerPosX + eyeWidthLength, centerPosY + (eyeHeigthLength * 2.0f), centerPosZ + eyeDepthLength, // bottom right
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + eyeDepthLength, // bottom right
        centerPosX + eyeWidthLength, centerPosY + eyeHeigthLength, centerPosZ + bodyHeigthLength, // top right

        // Mouth /////////////////////////
        // front face
        centerPosX - mouthWidthLength, centerPosY + (faceWidthLength * 0.6f), centerPosZ + mouthDepthLength, // top left
        centerPosX + mouthWidthLength, centerPosY + (faceWidthLength * 0.6f), centerPosZ + mouthDepthLength, // top right
        centerPosX + mouthWidthLength, centerPosY - (faceWidthLength * 0.4f), centerPosZ + mouthDepthLength, // bottom right
        centerPosX - mouthWidthLength, centerPosY - (faceWidthLength * 0.4f), centerPosZ + mouthDepthLength, // bottom left

        // Pupil /////////////////////////
        // Left //
        // front face
        centerPosX - pupilWidthLength, centerPosY + (faceWidthLength * 3.5f), centerPosZ + pupilDepthLength, // top left
        centerPosX - (faceHeigthLength * 0.4f), centerPosY + (faceWidthLength * 3.5f), centerPosZ + pupilDepthLength, // top right
        centerPosX - (faceHeigthLength * 0.4f), centerPosY + (faceWidthLength * 2.5f), centerPosZ + pupilDepthLength, // bottom right
        centerPosX - pupilWidthLength, centerPosY + (faceWidthLength * 2.5f), centerPosZ + pupilDepthLength, // bottom left
        // Right //
        // front face
        centerPosX + pupilWidthLength, centerPosY + (faceWidthLength * 3.5f), centerPosZ + pupilDepthLength, // top left
        centerPosX + (faceHeigthLength * 0.4f), centerPosY + (faceWidthLength * 3.5f), centerPosZ + pupilDepthLength, // top right
        centerPosX + (faceHeigthLength * 0.4f), centerPosY + (faceWidthLength * 2.5f), centerPosZ + pupilDepthLength, // bottom right
        centerPosX + pupilWidthLength, centerPosY + (faceWidthLength * 2.5f), centerPosZ + pupilDepthLength, // bottom left

    };


    GLfloat color[] = {
        0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216,  // front face
        0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216,  // back face
        // body
        0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216,  // back face
        0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216,  // back face
        0.88627451, 0.850980392, 0.71372549, 0.88627451, 0.850980392, 0.71372549, 0.88627451, 0.850980392, 0.71372549, 0.88627451, 0.850980392, 0.71372549,  // back face
        0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216, 0.862745098, 0.823529412, 0.698039216,  // back face
        // shell
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843,  // front face
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843,  // back face
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, // top face
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, // bottom face
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, // bottom face
        0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, 0.4, 0.239215686, 0.219607843, // bottom face
        // eye length
        // left
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, // front face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // back face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // top face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        // right
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, // front face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // back face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // top face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078, 0.823529412, 0.819607843, 0.690196078,  // bottom face
        // eye
        // left
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // front face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // back face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // top face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // bottom face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // bottom face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // bottom face
        // right
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // front face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // back face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // top face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // top face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // top face
        0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, 0.984313725, // bottom face
        // Mouth
        0.905882353, 0.341176471, 0.258823529, 0.905882353, 0.341176471, 0.258823529, 0.905882353, 0.341176471, 0.258823529, 0.905882353, 0.341176471, 0.258823529,
        // Pupil
        // left
        0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725,
        // right
        0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725, 0.349019608, 0.192156863, 0.184313725,
    };


    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);
    // glColor3f(color[0], color[1], color[2] ); //Pinto las lineas
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glColorPointer(3, GL_FLOAT, 0, color);
    glDrawArrays(mode, 0, 156);
    glDisableClientState(GL_DEPTH_TEST);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
