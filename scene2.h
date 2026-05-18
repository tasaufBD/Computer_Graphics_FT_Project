#pragma once
#include "weather.h"
#define JUNCTION_X 870.0f

void drawGroundScene2() {
    // Industrial dirt
    glColor3f(0.45f, 0.40f, 0.32f);
    drawRect(0, 0, WIN_W, GROUND_Y);

    // Sidewalk
    glColor3f(0.55f, 0.52f, 0.48f);
    drawRect(0, ROAD_TOP, WIN_W, 20);
    drawRect(0, ROAD_BOT - 20, WIN_W, 20);
    glColor3f(0.40f, 0.38f, 0.35f);
    drawRect(0, ROAD_TOP, WIN_W, 3);
    drawRect(0, ROAD_BOT - 20, WIN_W, 3);

    // Main road
    glColor3f(0.18f, 0.18f, 0.20f);
    drawRect(0, ROAD_BOT, WIN_W, ROAD_TOP - ROAD_BOT);

    // Trapizoidal Sub Road T
    glColor3f(0.18f, 0.18f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(JUNCTION_X - 35, ROAD_TOP);
    glVertex2f(JUNCTION_X + 35, ROAD_TOP);
    glVertex2f(JUNCTION_X + 14, ROAD_TOP + 130);
    glVertex2f(JUNCTION_X - 14, ROAD_TOP + 130);
    glEnd();

    // Lane markings
    glColor3f(0.95f, 0.95f, 0.6f);
    float midY = (ROAD_TOP + ROAD_BOT) * 0.5f - 2;
    for (float x = 0; x < WIN_W; x += 60) {
        if (x + 30 < JUNCTION_X - 35 || x > JUNCTION_X + 35) {
            drawRect(x, midY, 30, 4);
        }
    }

    for (int i = 0; i < 4; i++) {
        float t = i / 4.0f;
        float yy = ROAD_TOP + 20 + t * 95;
        float dw = 6.0f - t * 4.0f;
        drawRect(JUNCTION_X - dw * 0.5f, yy, dw, 10);
    }
}

void drawFactory(float x, float y) {
    // Body
    glColor3f(0.55f, 0.50f, 0.45f);
    drawRect(x, y, 350, 180);

    //roof
    glColor3f(0.40f, 0.35f, 0.30f);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 5; i++) {
        float bx = x + i * 70;
        glVertex2f(bx, y + 180);
        glVertex2f(bx + 70, y + 180);
        glVertex2f(bx + 35, y + 210);
    }
    glEnd();

    // Windows
    glColor3f(0.45f, 0.65f, 0.70f);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == 4 && j == 0) continue;  // door area
            drawRect(x + 18 + i * 66, y + 30 + j * 50, 46, 28);
        }
    }

    // Door
    glColor3f(0.25f, 0.20f, 0.15f);
    drawRect(x + 295, y, 45, 70);

    // Two chimneys
    glColor3f(0.55f, 0.30f, 0.25f);
    drawRect(x + 55,  y + 180, 28, 130);    //x, y, w, h
    drawRect(x + 270, y + 180, 28, 130);
    glColor3f(0.35f, 0.20f, 0.15f);
    drawRect(x + 51,  y + 295, 36, 15);
    drawRect(x + 266, y + 295, 36, 15);
}

// Chimney tip
void getFactoryChimneys(float fx, float fy,
                        float* x1, float* y1, float* x2, float* y2) {
    *x1 = fx + 69;   *y1 = fy + 305;
    *x2 = fx + 284;  *y2 = fy + 305;
}

void drawBrickKiln(float x, float y) {
    glColor3f(0.75f, 0.30f, 0.20f);
    glBegin(GL_QUADS);
    glVertex2f(x - 10, y);
    glVertex2f(x + 160, y);
    glVertex2f(x + 140, y + 80);
    glVertex2f(x + 10,  y + 80);
    glEnd();

    // Brick lines
    glColor3f(0.45f, 0.18f, 0.10f);
    glBegin(GL_LINES);
    for (int i = 1; i < 4; i++) {
        glVertex2f(x + 5,   y + 20 * i);
        glVertex2f(x + 150, y + 20 * i);
    }
    glEnd();

    // Doorway
    glColor3f(0.20f, 0.10f, 0.05f);
    drawRect(x + 65, y, 22, 36);

    // Tall chimney
    glColor3f(0.70f, 0.28f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(x + 70, y + 80);
    glVertex2f(x + 90, y + 80);
    glVertex2f(x + 86, y + 260);
    glVertex2f(x + 74, y + 260);
    glEnd();
    glColor3f(0.40f, 0.20f, 0.10f);
    drawRect(x + 70, y + 252, 20, 8);
}

void getKilnChimney(float kx, float ky, float* x, float* y) {
    *x = kx + 80;
    *y = ky + 262;
}

void drawTrafficLight(float x, float y) {
    // Post
    glColor3f(0.30f, 0.30f, 0.32f);
    drawRect(x - 2, y, 4, 90);
    // box
    glColor3f(0.20f, 0.20f, 0.22f);
    drawRect(x - 8, y + 90, 16, 42);

    // Light change
    int phase = ((int)g_time / 3) % 3;

    if (phase == 0) glColor3f(1.0f, 0.20f, 0.20f);
    else            glColor3f(0.30f, 0.10f, 0.10f);
    drawCircle(x, y + 122, 4);

    if (phase == 1) glColor3f(1.0f, 0.85f, 0.10f);
    else            glColor3f(0.30f, 0.25f, 0.10f);
    drawCircle(x, y + 110, 4);

    if (phase == 2) glColor3f(0.20f, 1.0f, 0.30f);
    else            glColor3f(0.10f, 0.30f, 0.15f);
    drawCircle(x, y + 98, 4);
}

void drawPylon(float cx, float baseY, float h) {
    glColor3f(0.30f, 0.30f, 0.35f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    float topW = 14;
    float botW = 38;

    // Two outer legs
    glVertex2f(cx - botW, baseY);  glVertex2f(cx - topW, baseY + h);
    glVertex2f(cx + botW, baseY);  glVertex2f(cx + topW, baseY + h);

    // 5 horizontal cross beams
    for (int i = 0; i < 5; i++) {
        float t = i / 4.0f;
        float yy = baseY + t * h;
        float w  = botW + (topW - botW) * t;
        glVertex2f(cx - w, yy);  glVertex2f(cx + w, yy);
    }

    // Diagonal X braces
    for (int i = 0; i < 4; i++) {
        float t1 = i / 4.0f;
        float t2 = (i + 1) / 4.0f;
        float y1 = baseY + t1 * h;
        float y2 = baseY + t2 * h;
        float w1 = botW + (topW - botW) * t1;
        float w2 = botW + (topW - botW) * t2;
        glVertex2f(cx - w1, y1);  glVertex2f(cx + w2, y2);
        glVertex2f(cx + w1, y1);  glVertex2f(cx - w2, y2);
    }

    // Cross-arm
    glVertex2f(cx - 28, baseY + h - 6);  glVertex2f(cx + 28, baseY + h - 6);
    glVertex2f(cx - 22, baseY + h - 18); glVertex2f(cx + 22, baseY + h - 18);

    glEnd();
    glLineWidth(1.0f);
}

void drawTransformer(float x, float y) {
    // Two poles
    glColor3f(0.40f, 0.28f, 0.15f);
    drawRect(x - 2,  y, 4, 110);
    drawRect(x + 78, y, 4, 110);

    // Cross-bar
    drawRect(x - 8, y + 98, 96, 4);

    // Metal box
    glColor3f(0.40f, 0.40f, 0.45f);
    drawRect(x + 28, y + 70, 32, 28);
    glColor3f(0.30f, 0.30f, 0.35f);
    drawRect(x + 28, y + 96, 32, 4);

    // Insulator
    glColor3f(0.65f, 0.65f, 0.30f);
    drawCircle(x + 36, y + 102, 3);
    drawCircle(x + 44, y + 102, 3);
    drawCircle(x + 52, y + 102, 3);
}

void drawOverheadWires() {
    glColor3f(0.10f, 0.10f, 0.10f);
    glLineWidth(1.2f);

    float p1 = 220, p2 = 640, p3 = 1150;
    float topY = GROUND_Y + 270;
    float tfL = 396;
    float tfR = 412;
    float tfY = GROUND_Y + 102;

    // Pylon1 -> transformer
    drawBezier(p1, topY,
               (p1 + tfL) * 0.5f, topY - 30,
               (p1 + tfL) * 0.5f, topY - 30,
               tfL, tfY);

    // Transformer-> pylon2
    drawBezier(tfR, tfY,
               (tfR + p2) * 0.5f, topY - 35,
               (tfR + p2) * 0.5f, topY - 35,
               p2, topY);

    // Pylon2 -> pylon3
    drawBezier(p2, topY,
               (p2 + p3) * 0.5f, topY - 50,
               (p2 + p3) * 0.5f, topY - 50,
               p3, topY);
    drawBezier(p2, topY - 12,
               (p2 + p3) * 0.5f, topY - 60,
               (p2 + p3) * 0.5f, topY - 60,
               p3, topY - 12);

    glLineWidth(1.0f);
}

void drawBillboard(float x, float y) {
    // Posts
    glColor3f(0.35f, 0.25f, 0.15f);
    drawRect(x + 18, y, 6, 90);
    drawRect(x + 134, y, 6, 90);

    // Frame
    glColor3f(0.20f, 0.20f, 0.20f);
    drawRect(x, y + 90, 160, 90);

    // Ad area
    float ax = x + 6;
    float ay = y + 96;
    float aw = 148;
    float ah = 78;

    int adIndex = ((int)g_time / 3) % 5;

    if (adIndex == 0) {
        glColor3f(0.85f, 0.15f, 0.20f);
        drawRect(ax, ay, aw, ah);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(ax + 42, ay + 46, "AIUB", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(ax + 6, ay + 20, "Where Leaders are Created!", GLUT_BITMAP_HELVETICA_10);
    } else if (adIndex == 1) {
        glColor3f(0.15f, 0.55f, 0.25f);
        drawRect(ax, ay, aw, ah);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(ax + 25, ay + 46, "Teletalk", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(ax + 30, ay + 20, "Stay Connected", GLUT_BITMAP_HELVETICA_12);
    } else if (adIndex == 2) {
        glColor3f(0.20f, 0.50f, 0.85f);
        drawRect(ax, ay, aw, ah);
        glColor3f(1.0f, 0.95f, 0.30f);
        drawCircle(ax + 22, ay + 50, 9);
        glColor3f(1.0f, 1.0f, 1.0f);
        drawText(ax + 38, ay + 46, "Visit", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(ax + 40, ay + 20, "Cox's Bazar", GLUT_BITMAP_HELVETICA_12);
    } else if (adIndex == 3) {
        glColor3f(0.0f, 0.96f, 0.96f);
        drawRect(ax, ay, aw, ah);
        glColor3f(0.80f, 0.15f, 0.20f);
        glColor3f(0.10f, 0.10f, 0.30f);
        drawText(ax + 30, ay + 38, "Osman Hadi", GLUT_BITMAP_HELVETICA_18);
        glColor3f(0.40f, 0.40f, 0.40f);
        drawText(ax + 8, ay + 18, "We have not Forgotten you", GLUT_BITMAP_HELVETICA_10);
    } else {
        glColor3f(0.98f, 0.78f, 0.15f);
        drawRect(ax, ay, aw, ah);
        glColor3f(0.20f, 0.10f, 0.05f);
        drawText(ax + 40, ay + 46, "PRAN", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(ax + 42, ay + 20, "Fresh Juice", GLUT_BITMAP_HELVETICA_12);
    }
}

void drawBus(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.10f, 0.30f, 0.65f);
    drawRect(-55, 5, 110, 42);
    glColor3f(0.95f, 0.80f, 0.20f);
    drawRect(-55, 22, 110, 4);
    glColor3f(0.70f, 0.85f, 0.95f);
    for (int i = 0; i < 5; i++) {
        drawRect(-50 + i * 20, 30, 16, 14);
    }
    glColor3f(0.75f, 0.88f, 0.95f);
    drawRect(40, 28, 16, 18);
    glColor3f(0.20f, 0.20f, 0.30f);
    drawRect(-50, 5, 12, 28);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-32, 0, 9);
    drawCircle( 32, 0, 9);
    glColor3f(0.45f, 0.45f, 0.45f);
    drawCircle(-32, 0, 4);
    drawCircle( 32, 0, 4);
    glPopMatrix();
}

void drawTanker(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    // Tank body
    glColor3f(0.80f, 0.80f, 0.85f);
    drawRect(-50, 10, 70, 26);
    drawCircle(-50, 23, 13);
    drawCircle( 20, 23, 13);
    // Stripe
    glColor3f(0.85f, 0.15f, 0.20f);
    drawRect(-50, 20, 70, 5);
    // Cab
    glColor3f(0.20f, 0.55f, 0.30f);
    drawRect(20, 5, 28, 30);
    glColor3f(0.70f, 0.85f, 0.95f);
    drawRect(24, 18, 20, 14);
    // Wheel
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-38, 0, 8);
    drawCircle(-10, 0, 8);
    drawCircle( 32, 0, 8);
    glPopMatrix();
}

void drawPickup(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.50f, 0.45f, 0.40f);
    drawRect(-40, 6, 36, 22);
    glColor3f(0.95f, 0.85f, 0.20f);
    drawRect(-4, 6, 30, 32);
    glColor3f(0.70f, 0.85f, 0.95f);
    drawRect(0, 20, 22, 14);
    glColor3f(0.95f, 0.85f, 0.20f);
    drawRect(26, 6, 14, 16);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-30, 0, 7);
    drawCircle( 28, 0, 7);
    glPopMatrix();
}

void drawCarA(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.25f, 0.40f, 0.85f);
    drawRect(-32, 6, 64, 16);
    glColor3f(0.35f, 0.50f, 0.95f);
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
    glPopMatrix();
}

void drawCarB(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3f(0.20f, 0.20f, 0.22f);
    drawRect(-32, 6, 64, 16);
    glColor3f(0.30f, 0.30f, 0.32f);
    drawRect(-20, 22, 40, 14);
    glColor3f(0.55f, 0.75f, 0.85f);
    drawRect(-18, 24, 16, 10);
    drawRect(2,   24, 16, 10);
    glColor3f(0.10f, 0.10f, 0.10f);
    drawCircle(-20, 4, 7);
    drawCircle( 20, 4, 7);
    glColor3f(0.55f, 0.55f, 0.55f);
    drawCircle(-20, 4, 3);
    drawCircle( 20, 4, 3);
    glPopMatrix();
}

void drawScene2() {
    drawGroundScene2();
    // Background pylons
    drawPylon(220,  GROUND_Y, 270);
    drawPylon(640,  GROUND_Y, 270);
    drawPylon(1150, GROUND_Y, 270);

    drawFactory(80,  GROUND_Y);
    drawBrickKiln(480, GROUND_Y);
    drawTransformer(360, GROUND_Y);
    drawOverheadWires();
    drawBillboard(940, GROUND_Y);

    drawTrafficLight(JUNCTION_X - 50, ROAD_TOP + 22);
    drawTrafficLight(JUNCTION_X + 50, ROAD_TOP + 22);

    float roadY = (ROAD_TOP + ROAD_BOT) * 0.5f - 4;
    int wrap = (int)WIN_W + 200;

    float tankerX = (float)((int)(g_time * 45.0f * g_animSpeed + 100) % wrap) - 100;
    drawTanker(tankerX, roadY + 6);

    float pickupX = (float)((int)(g_time * 70.0f * g_animSpeed + 500) % wrap) - 100;
    drawPickup(pickupX, roadY + 4);

    float carAX = (float)((int)(g_time * 85.0f * g_animSpeed + 900) % wrap) - 100;
    drawCarA(carAX, roadY + 4);

    float carBX = (float)((int)(g_time * 60.0f * g_animSpeed + 300) % wrap) - 100;
    drawCarB(carBX, roadY + 4);

    // Player bus
    float px = clampf(g_playerX, 60.0f, WIN_W - 60.0f);
    drawBus(px, roadY + 6);
}

void emitScene2Smoke() {
    float fx1, fy1, fx2, fy2, kx, ky;
    getFactoryChimneys(80, GROUND_Y, &fx1, &fy1, &fx2, &fy2);
    getKilnChimney(480, GROUND_Y, &kx, &ky);
    emitSmoke(fx1, fy1);
    emitSmoke(fx2, fy2);
    emitSmoke(kx,  ky);
}
