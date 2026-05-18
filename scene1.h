#pragma once
#include "weather.h"

void drawGroundScene1() {
    // Grass
    glColor3f(0.30f, 0.55f, 0.20f);
    drawRect(0, 0, WIN_W, GROUND_Y);

    // Sidewalks
    glColor3f(0.70f, 0.68f, 0.62f);
    drawRect(0, ROAD_TOP, WIN_W, 20);
    drawRect(0, ROAD_BOT - 20, WIN_W, 20);
    glColor3f(0.45f, 0.43f, 0.40f);
    drawRect(0, ROAD_TOP, WIN_W, 3);
    drawRect(0, ROAD_BOT - 20, WIN_W, 3);

    // Road
    glColor3f(0.20f, 0.20f, 0.22f);
    drawRect(0, ROAD_BOT, WIN_W, ROAD_TOP - ROAD_BOT);

    // Lane markings
    glColor3f(0.95f, 0.95f, 0.6f);
    float midY = (ROAD_TOP + ROAD_BOT) * 0.5f - 2;
    for (float x = 0; x < WIN_W; x += 60) {
        drawRect(x, midY, 30, 4);
    }
}

void drawPond() {
    glPushMatrix();
    glTranslatef(140, 35, 0);
    glScalef(1.0f, 0.35f, 1.0f);
    glColor3f(0.20f, 0.45f, 0.65f);
    drawCircle(0, 0, 80);
    glColor3f(0.30f, 0.60f, 0.80f);
    drawCircle(-15, 10, 60);
    glPopMatrix();
}


void drawApartment(float x, float y, float w, float h,
                   float bodyR, float bodyG, float bodyB,
                   int cols, int rows) {
    // Body
    glColor3f(bodyR, bodyG, bodyB);
    drawRect(x, y, w, h);

    // Roof
    glColor3f(bodyR * 0.7f, bodyG * 0.7f, bodyB * 0.7f);
    drawRect(x - 4, y + h, w + 8, 8);

    // Window
    float topMargin = 18;
    float bottomMargin = 50;
    float marginX = 12;
    float winW = (w - marginX * (cols + 1)) / cols;
    float winH = 22;
    float gapY = (h - bottomMargin - topMargin - winH * rows) / (rows - 1);

    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            float wx = x + marginX + c * (winW + marginX);
            float wy = y + bottomMargin + r * (winH + gapY);
            bool lit = (!g_isDay) && (((c + r * 3) % 5) != 0);
            if (lit) glColor3f(1.0f, 0.85f, 0.45f);
            else     glColor3f(0.30f, 0.35f, 0.45f);
            drawRect(wx, wy, winW, winH);
        }
    }

    // Door
    glColor3f(0.25f, 0.15f, 0.10f);
    drawRect(x + w * 0.5f - 14, y, 28, 36);
}

void drawMosque(float x, float y) {
    // Body
    glColor3f(0.92f, 0.88f, 0.78f);
    drawRect(x, y, 200, 130);

    // Arched door
    glColor3f(0.30f, 0.20f, 0.10f);
    drawRect(x + 85, y, 30, 60);
    drawDome(x + 100, y + 60, 15);

    // Side windows
    glColor3f(0.20f, 0.30f, 0.55f);
    drawRect(x + 25, y + 75, 25, 30);
    drawRect(x + 150, y + 75, 25, 30);

    // dome
    glColor3f(0.78f, 0.78f, 0.85f);
    drawDome(x + 100, y + 130, 70);
    glColor3f(0.85f, 0.7f, 0.2f);
    drawRect(x + 98, y + 200, 4, 18);
    drawCircle(x + 100, y + 222, 4);

    // Minar
    float mx = x + 215;
    glColor3f(0.92f, 0.88f, 0.78f);
    drawRect(mx, y, 22, 200);
    glColor3f(0.78f, 0.78f, 0.85f);
    drawRect(mx - 4, y + 160, 30, 8);
    drawDome(mx + 11, y + 200, 16);
    glColor3f(0.85f, 0.7f, 0.2f);
    drawRect(mx + 9, y + 216, 4, 14);
}

void drawTree(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Trunk
    glColor3f(0.40f, 0.25f, 0.12f);
    drawRect(-6 * scale, 0, 12 * scale, 28 * scale);

    // Foliage
    glPushMatrix();
    glTranslatef(0, 28 * scale, 0);
    if (g_weatherState == WEATHER_STORM) {
        float a = sinf(g_time * 3.0f + x * 0.01f) * (g_windSpeed * 0.06f);
        glRotatef(a, 0, 0, 1);
    }
    glScalef(scale, scale, 1);
    glColor3f(0.10f, 0.50f, 0.18f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-22, 0);  glVertex2f(22, 0);   glVertex2f(0, 32);
    glVertex2f(-20, 14); glVertex2f(20, 14);  glVertex2f(0, 48);
    glVertex2f(-16, 28); glVertex2f(16, 28);  glVertex2f(0, 60);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}

void drawStreetlight(float x, float y) {
    glColor3f(0.30f, 0.30f, 0.32f);
    drawRect(x - 2, y, 4, 70);
    drawRect(x, y + 64, 16, 4);
    if (!g_isDay) glColor3f(1.0f, 0.92f, 0.50f);
    else          glColor3f(0.60f, 0.60f, 0.60f);
    drawRect(x + 14, y + 58, 8, 10);
    if (!g_isDay) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.95f, 0.5f, 0.25f);
        drawCircle(x + 18, y + 63, 18);
        glDisable(GL_BLEND);
    }
}

void drawBench(float x, float y) {
    // legs
    glColor3f(0.35f, 0.22f, 0.12f);
    drawRect(x, y, 4, 18);
    drawRect(x + 36, y, 4, 18);
    // Seat
    glColor3f(0.55f, 0.35f, 0.18f);
    drawRect(x - 4, y + 14, 48, 5);
    // Back
    drawRect(x - 4, y + 19, 48, 4);
    drawRect(x - 4, y + 24, 48, 4);
    // Backrest supports
    drawRect(x + 2,  y + 19, 3, 14);
    drawRect(x + 35, y + 19, 3, 14);
}

void drawRickshaw(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-18, 0, 11);
    drawCircle( 18, 0, 11);
    glColor3f(0.45f, 0.45f, 0.45f);
    drawCircle(-18, 0, 4);
    drawCircle( 18, 0, 4);
    glColor3f(0.85f, 0.20f, 0.20f);
    drawRect(-24, 8, 40, 32);

    // Hood
    glColor3f(0.10f, 0.40f, 0.15f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-26, 40); glVertex2f(18, 40); glVertex2f(-4, 58);
    glEnd();

    // Driver seat
    glColor3f(0.55f, 0.30f, 0.10f);
    drawRect(16, 8, 14, 18);
    // Handlebar
    glColor3f(0.20f, 0.20f, 0.20f);
    drawRect(28, 22, 3, 16);
    drawRect(28, 36, 10, 3);
    // Headlight
    glColor3f(1.0f, 0.95f, 0.5f);
    drawCircle(34, 14, 3);

    glPopMatrix();
}

void drawCNG(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.10f, 0.55f, 0.25f);
    drawRect(-22, 6, 44, 26);
    glColor3f(0.55f, 0.75f, 0.85f);
    drawRect(8, 14, 12, 14);
    glColor3f(0.95f, 0.85f, 0.20f);
    drawRect(-22, 32, 44, 4);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-14, 0, 7);
    drawCircle( 14, 0, 7);
    glPopMatrix();
}

void drawCar1(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.75f, 0.20f, 0.30f);
    drawRect(-32, 6, 64, 16);
    glColor3f(0.85f, 0.30f, 0.40f);
    drawRect(-20, 22, 40, 14);
    glColor3f(0.60f, 0.80f, 0.90f);
    drawRect(-18, 24, 16, 10);
    drawRect(2,   24, 16, 10);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-20, 4, 7);
    drawCircle( 20, 4, 7);
    glColor3f(0.40f, 0.40f, 0.40f);
    drawCircle(-20, 4, 3);
    drawCircle( 20, 4, 3);
    glColor3f(1.0f, 0.95f, 0.5f);
    drawRect(28, 10, 4, 4);
    glPopMatrix();
}

void drawScene1() {
    drawGroundScene1();
    drawPond();

    drawApartment(70,  GROUND_Y, 200, 320, 0.85f, 0.65f, 0.45f, 4, 7);
    drawApartment(300, GROUND_Y, 200, 280, 0.70f, 0.75f, 0.85f, 4, 6);
    drawApartment(530, GROUND_Y, 200, 340, 0.80f, 0.55f, 0.55f, 4, 7);
    drawMosque(  860, GROUND_Y);

    drawTree( 40,  GROUND_Y - 8, 1.0f);
    drawTree(285, GROUND_Y - 8, 0.9f);
    drawTree(515, GROUND_Y - 8, 1.1f);
    drawTree(745, GROUND_Y - 8, 0.95f);
    drawTree(1150, GROUND_Y - 8, 1.0f);
    drawTree(1240, GROUND_Y - 8, 0.85f);

    drawStreetlight(390, GROUND_Y);
    drawStreetlight(820, GROUND_Y);
    drawStreetlight(1100, GROUND_Y);

    drawBench(250, 10);
    drawBench(680, 10);

    float roadY = (ROAD_TOP + ROAD_BOT) * 0.5f - 4;
    int wrap = (int)WIN_W + 200;

    float cngX = (float)((int)(g_time * 60.0f * g_animSpeed) % wrap) - 100;
    drawCNG(cngX, roadY + 6);

    float carX = (float)((int)(g_time * 75.0f * g_animSpeed + 600) % wrap) - 100;
    drawCar1(carX, roadY + 4);

    float px = clampf(g_playerX, 40.0f, WIN_W - 40.0f);
    drawRickshaw(px, roadY + 6);
}
