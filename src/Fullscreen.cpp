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
#include <cassert>
#include <vector>

class Color {
public:
    Color(float aR, float aG, float aB, float aA) :
        mR(aR), mG(aG), mB(aB), mA(aA)
    {
        assert(0.0f <= mR); assert(1.0f >= mR);
        assert(0.0f <= mG); assert(1.0f >= mG);
        assert(0.0f <= mB); assert(1.0f >= mB);
        assert(0.0f <= mA); assert(1.0f >= mA);
    }
    inline const float r() {
        return mR;
    }
    inline const float g() {
        return mG;
    }
    inline const float b() {
        return mB;
    }
    inline const float a() {
        return mA;
    }
private:
    float mR;
    float mG;
    float mB;
    float mA;
};

class vect2d {
public:
    vect2d(float aX, float aY) :
        mX(aX), mY(aY)
    {
    }

    const float x() {
        return mX;
    }
    const float y() {
        return mY;
    }
    const float w() {
        return mX;
    }
    const float h() {
        return mY;
    }

    void x(float aX) {
        mX = aX;
    }
    void y(float aY) {
        mY = aY;
    }

private:
    float mX;
    float mY;
};
typedef vect2d Position;
typedef vect2d Dimension;
typedef vect2d Speed;

/**
 * @brief Manage the animation and rendering of a rectangle
 */
class Rectangle {
public:
    typedef std::vector<Rectangle> Vector;

public:
    /**
     * @brief Build a rectangle that must fit into the [(-1.0f, -1.0f), (1.0f, 1.0f)] screen
     */
    Rectangle(const Position& aPosition, const Dimension& aDimension, const Speed& aSpeed, const Color& aColor) :
        mPosition(aPosition), mDimension(aDimension), mSpeed(aSpeed), mColor(aColor)
    {
        assert(0.0f < mDimension.w()); assert(2.0f > mDimension.w());
        assert(0.0f < mDimension.h()); assert(2.0f > mDimension.h());
        assert(-1.0f <= left());    assert(left() < 1.0f);
        assert(-1.0f <= bottom());  assert(bottom() < 1.0f);
        assert(-1.0f <  top());     assert(top() < 1.0f);
    }

    inline const float left() {
        return (mPosition.x());
    }
    inline const float right() {
        return (mPosition.x() + mDimension.w());
    }
    inline const float bottom() {
        return (mPosition.y());
    }
    inline const float top() {
        return (mPosition.y() + mDimension.h());
    }
    
    /**
     * @brief Move the rectangle according to its position, its speed, and the ellapsed time since previous frame
     *
     * Reposition the rectangle to the bottom/left when it reach the screen top/right (or reverse)
     */
    void move(float aDeltaTime) {
        mPosition.x(mPosition.x() + (mSpeed.x() * aDeltaTime));
        mPosition.y(mPosition.y() + (mSpeed.y() * aDeltaTime));
        if (left() < -1.0f) {
            mPosition.x(1.0f);
        }
        if (left() > 1.0f) {
            mPosition.x(-1.0f);
        }
        if (bottom() < -1.0f) {
            mPosition.y(1.0f);
        }
        if (top() > 1.0f) {
            mPosition.y(-1.0f);
        }
    }

    // draw a rectangle polygon
    void render() {
        if (right() < 1.0f) {
            // if no horizontal overflow
            glBegin(GL_POLYGON);
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(left(), bottom());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(left(), top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(right(), top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(right(), bottom());
            glEnd();
        } else {
            // if horizontal overflow
            float offX = right() - 2.0f;
            glBegin(GL_POLYGON);
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(left(), bottom());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(left(), top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(1.0f, top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(1.0f, bottom());
            glEnd();
            glBegin(GL_POLYGON);
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(-1.0f, bottom());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(-1.0f, top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(offX, top());
                glColor4f(mColor.r(), mColor.g(), mColor.b(), mColor.a());
                glVertex2f(offX, bottom());
            glEnd();
        }
    }

private:
    Position    mPosition;
    Dimension   mDimension;
    Speed       mSpeed;
    Color       mColor;
};


static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(void) {
    Rectangle::Vector rectangles;
    rectangles.push_back(Rectangle(Position(0.3f, 0.7f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.4f, 0.0f),
                                   Color(0.0f, 1.0f, 1.0f, 1.0f)));
    rectangles.push_back(Rectangle(Position(0.0f, 0.4f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.1f, 0.0f),
                                   Color(0.0f, 0.0f, 1.0f, 1.0f)));
    rectangles.push_back(Rectangle(Position(-0.6f, 0.1f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.6f, 0.0f),
                                   Color(0.0f, 1.0f, 0.0f, 1.0f)));
    rectangles.push_back(Rectangle(Position(-1.0f, -0.2f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.5f, 0.0f),
                                   Color(1.0f, 0.0f, 0.0f, 1.0f)));
    rectangles.push_back(Rectangle(Position(-1.0f, -0.5f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.2f, 0.0f),
                                   Color(1.0f, 0.0f, 1.0f, 1.0f)));
    rectangles.push_back(Rectangle(Position(-1.0f, -0.8f),
                                   Dimension(1.0f, 0.2f),
                                   Speed(0.3f, 0.0f),
                                   Color(1.0f, 1.0f, 0.0f, 1.0f)));
    
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    // Ask for SRGB linear to gama conversion buffer
    glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
    
    // Ask for vertical synch (not working)
    // glfwSwapInterval(1);

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

    double currTime = glfwGetTime();
    double prevTime = currTime;
    while (!glfwWindowShouldClose(window)) {
        // clear the buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        currTime = glfwGetTime();
        float ellapsedTime = static_cast<float>(currTime - prevTime);
        std::cout << "ellapsedTime=" << ellapsedTime << std::endl;

        // move the rectangles based on ellapsed time
        for (Rectangle::Vector::iterator iRectangle  = rectangles.begin();
                                         iRectangle != rectangles.end();
                                       ++iRectangle) {
            iRectangle->move(ellapsedTime);
        }

        // draw the rectangles
        for (Rectangle::Vector::iterator iRectangle  = rectangles.begin();
                                         iRectangle != rectangles.end();
                                       ++iRectangle) {
            iRectangle->render();
        }
        
        // Swap back & front buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        prevTime = currTime;
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

