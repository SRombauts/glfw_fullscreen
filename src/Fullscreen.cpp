/**
 * @file    Fullscreen.cpp
 * @brief   Tests of glfw fullscreen mode.
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void) {
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

    // get the current Desktop screen resolution and colour depth
    const GLFWvidmode* pCurrentVideoMod = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int width = pCurrentVideoMod->width;
    int height = pCurrentVideoMod->height;
    std::cout << "fullscreen (" << width << " x " << height << ")\n";

    // Open a fullscreen window on the first monitor
    window = glfwCreateWindow(width, height, "Simple example", glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    std::cout << "fullscreen (" << width << " x " << height << ")\n";

    // configure projection matrix with the ratio of the windows
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = width / (float)height;
    glOrtho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

    while (!glfwWindowShouldClose(window)) {
        // clear the buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // configure model view matrix, with timed rotation
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.0f, 0.0f, 0.0f, 1.0f);
        
        // draw a triangle
        glBegin(GL_TRIANGLES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(-0.6f, -0.4f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.6f, -0.4f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.6f, 0.0f);
        glEnd();
        
        // Swap back & front buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

