// Utilities Header
#pragma once
#include <windows.h>
#include <GL/glut.h>
#include <cmath>
#define PI 3.14159f

void drawCircle(float cx, float cy, float r) {
    int segments = 24;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float t = 2.0f * PI * i / segments;
        glVertex2f(cx + r * cosf(t), cy + r * sinf(t));
    }
    glEnd();
}

// Half-circle(Dome)
void drawDome(float cx, float cy, float r) {
    int segments = 20;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= segments; i++) {
        float t = PI * i / segments;
        glVertex2f(cx + r * cosf(t), cy + r * sinf(t));
    }
    glEnd();
}

// Filled rectangle
void drawRect(float x, float y, float w, float h) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

// x = (1-t)^3*x0 + 3t(1-t)^2*x1 + 3t^2(1-t)*x2 + t^3*x3
void bezierPoint(float t,
                 float x0, float y0, float x1, float y1,
                 float x2, float y2, float x3, float y3,
                 float* outX, float* outY) {
    float u = 1.0f - t;
    *outX = u*u*u*x0 + 3*t*u*u*x1 + 3*t*t*u*x2 + t*t*t*x3;
    *outY = u*u*u*y0 + 3*t*u*u*y1 + 3*t*t*u*y2 + t*t*t*y3;
}

// Bezier as connected line strip
void drawBezier(float x0, float y0, float x1, float y1,
                float x2, float y2, float x3, float y3) {
    int segments = 24;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= segments; i++) {
        float t = (float)i / segments;
        float x, y;
        bezierPoint(t, x0,y0, x1,y1, x2,y2, x3,y3, &x, &y);
        glVertex2f(x, y);
    }
    glEnd();
}

// Bitmap text
void drawText(float x, float y, const char* text, void* font) {
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(font, *text++);
    }
}

// Clamp a value to a range
float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
