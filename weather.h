#pragma once
#include "utils.h"
#include <cstdio>
#include <cstdlib>

//Constraints
#define WIN_W      1280.0f
#define WIN_H      720.0f
#define GROUND_Y   200.0f
#define ROAD_TOP   140.0f
#define ROAD_BOT   70.0f

// Weather states
#define WEATHER_CLEAR 0
#define WEATHER_STORM 1
#define WEATHER_SMOG  2

// Array sizes
#define MAX_RAIN     220
#define MAX_SMOKE    150
#define MAX_CLOUDS   4
#define MAX_STARS    60
#define MAX_LBOLT    16

//Global state
int   g_currentScene  = 0;
int   g_weatherState  = WEATHER_CLEAR;
bool  g_isDay         = true;
bool  g_liveMode      = false;
float g_windSpeed     = 30.0f;
float g_animSpeed     = 1.0f;
int   g_aqi           = 50;
float g_lightningFlash = 0.0f;
float g_sunAngle      = PI * 0.4f;
float g_time          = 0.0f;
float g_playerX       = 600.0f;

//Rain Array
float rainX[MAX_RAIN];
float rainY[MAX_RAIN];
float rainLen[MAX_RAIN];

//Smoke Array
float smokeX[MAX_SMOKE];
float smokeY[MAX_SMOKE];
float smokeSize[MAX_SMOKE];
float smokeAlpha[MAX_SMOKE];
int   smokeIdx = 0;

//Clouds
float cloudX[MAX_CLOUDS];
float cloudY[MAX_CLOUDS];
float cloudScale[MAX_CLOUDS];

//Stars
float starX[MAX_STARS];
float starY[MAX_STARS];
float starBright[MAX_STARS];

//Lightning waypoints
float lboltX[MAX_LBOLT];
float lboltY[MAX_LBOLT];
int   lboltCount = 0;

void initWeather() {
    for (int i = 0; i < MAX_RAIN; i++) {
        rainX[i] = (float)(rand() % (int)WIN_W);
        rainY[i] = (float)(rand() % (int)WIN_H);
        rainLen[i] = 10.0f + (rand() % 8);
    }
    for (int i = 0; i < MAX_STARS; i++) {
        starX[i] = (float)(rand() % (int)WIN_W);
        starY[i] = GROUND_Y + 50 + (rand() % (int)(WIN_H - GROUND_Y - 50));
        starBright[i] = 0.4f + (rand() % 60) / 100.0f;
    }

    cloudX[0] = 200;  cloudY[0] = 580; cloudScale[0] = 1.0f;
    cloudX[1] = 600;  cloudY[1] = 620; cloudScale[1] = 1.2f;
    cloudX[2] = 1000; cloudY[2] = 560; cloudScale[2] = 0.9f;
    cloudX[3] = 1300; cloudY[3] = 600; cloudScale[3] = 1.1f;

    for (int i = 0; i < MAX_SMOKE; i++) smokeAlpha[i] = 0.0f;
    smokeIdx = 0;
}

void readWeatherFile() {
    FILE* f = fopen("weather.txt", "r");
    if (!f) return;
    char line[128];
    int rain = -1, aqi = -1, wind = -1;
    while (fgets(line, sizeof(line), f)) {
        int v;
        if (sscanf(line, "rain=%d", &v) == 1) rain = v;
        else if (sscanf(line, "aqi=%d", &v) == 1) aqi = v;
        else if (sscanf(line, "wind=%d", &v) == 1) wind = v;
    }
    fclose(f);

    if (rain == 1)        g_weatherState = WEATHER_STORM;
    else if (aqi >= 150)  g_weatherState = WEATHER_SMOG;
    else                  g_weatherState = WEATHER_CLEAR;

    if (aqi >= 0)  g_aqi = aqi;
    if (wind >= 0) g_windSpeed = clampf((float)wind * 6.0f, 0.0f, 200.0f);
}

//Sky gradient
void drawSky() {
    float tR, tG, tB, bR, bG, bB;
    if (!g_isDay) {
        tR = 0.04f; tG = 0.05f; tB = 0.14f;
        bR = 0.10f; bG = 0.12f; bB = 0.22f;
    } else if (g_weatherState == WEATHER_STORM) {
        tR = 0.22f; tG = 0.24f; tB = 0.30f;
        bR = 0.42f; bG = 0.44f; bB = 0.50f;
    } else if (g_weatherState == WEATHER_SMOG) {
        tR = 0.55f; tG = 0.45f; tB = 0.30f;
        bR = 0.78f; bG = 0.68f; bB = 0.52f;
    } else {
        tR = 0.30f; tG = 0.55f; tB = 0.92f;
        bR = 0.75f; bG = 0.88f; bB = 1.00f;
    }
    // Lightning brightens sky
    if (g_lightningFlash > 0.0f) {
        float k = g_lightningFlash * 0.75f;
        tR = clampf(tR + k, 0, 1); tG = clampf(tG + k, 0, 1); tB = clampf(tB + k, 0, 1);
        bR = clampf(bR + k, 0, 1); bG = clampf(bG + k, 0, 1); bB = clampf(bB + k, 0, 1);
    }
    glBegin(GL_QUADS);
    glColor3f(bR, bG, bB); glVertex2f(0, GROUND_Y); glVertex2f(WIN_W, GROUND_Y);
    glColor3f(tR, tG, tB); glVertex2f(WIN_W, WIN_H); glVertex2f(0, WIN_H);
    glEnd();
}


void drawSunMoon() {
    float cx = WIN_W * (g_sunAngle / PI);
    float cy = GROUND_Y + 100 + 320 * sinf(g_sunAngle);
    if (g_isDay) {
        glColor3f(1.0f, 0.9f, 0.4f);
        drawCircle(cx, cy, 38);
        // Soft halo
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.95f, 0.6f, 0.18f);
        drawCircle(cx, cy, 60);
        glDisable(GL_BLEND);
    } else {
        glColor3f(0.95f, 0.95f, 0.85f);
        drawCircle(cx, cy, 32);
        glColor3f(0.10f, 0.12f, 0.22f);
        drawCircle(cx + 10, cy + 4, 28);
    }
}

//Stars
void drawStars() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_STARS; i++) {
        float b = starBright[i] + 0.2f * sinf(g_time * 2.0f + i);
        b = clampf(b, 0.2f, 1.0f);
        glColor3f(b, b, b);
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
    glPointSize(1.0f);
}

void drawCloud(float cx, float cy, float scale) {
    float r, g, b;
    if (g_weatherState == WEATHER_STORM) {
        r = 0.55f; g = 0.58f; b = 0.62f;
    } else {
        r = 0.97f; g = 0.97f; b = 0.99f;
    }
    glColor3f(r, g, b);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy + 10 * scale);
    glVertex2f(cx - 100 * scale, cy);

    // Left hump
    for (int i = 1; i <= 12; i++) {
        float t = (float)i / 12, x, y;
        bezierPoint(t,
            cx - 100 * scale, cy,
            cx - 100 * scale, cy + 50 * scale,
            cx -  40 * scale, cy + 55 * scale,
            cx -  30 * scale, cy + 18 * scale, &x, &y);
        glVertex2f(x, y);
    }
    // Middle hump
    for (int i = 1; i <= 12; i++) {
        float t = (float)i / 12, x, y;
        bezierPoint(t,
            cx - 30 * scale, cy + 18 * scale,
            cx - 30 * scale, cy + 75 * scale,
            cx + 30 * scale, cy + 75 * scale,
            cx + 30 * scale, cy + 18 * scale, &x, &y);
        glVertex2f(x, y);
    }
    // Right hump
    for (int i = 1; i <= 12; i++) {
        float t = (float)i / 12, x, y;
        bezierPoint(t,
            cx +  30 * scale, cy + 18 * scale,
            cx +  40 * scale, cy + 55 * scale,
            cx + 100 * scale, cy + 50 * scale,
            cx + 100 * scale, cy,             &x, &y);
        glVertex2f(x, y);
    }
    glVertex2f(cx - 100 * scale, cy);
    glEnd();
}

void drawClouds() {
    for (int i = 0; i < MAX_CLOUDS; i++) {
        drawCloud(cloudX[i], cloudY[i], cloudScale[i]);
    }
}

void updateClouds(float dt) {
    for (int i = 0; i < MAX_CLOUDS; i++) {
        cloudX[i] += (10.0f + g_windSpeed * 0.25f) * dt * g_animSpeed;
        if (cloudX[i] - 120 > WIN_W) cloudX[i] = -120;
    }
}

void updateRain(float dt) {
    if (g_weatherState != WEATHER_STORM) return;
    for (int i = 0; i < MAX_RAIN; i++) {
        rainY[i] -= 700.0f * dt;
        rainX[i] += g_windSpeed * 1.5f * dt;
        if (rainY[i] < 0) {
            rainY[i] = WIN_H + (rand() % 40);
            rainX[i] = (float)(rand() % (int)WIN_W);
        }
        if (rainX[i] > WIN_W) rainX[i] -= WIN_W;
    }
}

void drawRain() {
    if (g_weatherState != WEATHER_STORM) return;
    glColor3f(0.65f, 0.75f, 0.95f);
    glLineWidth(1.4f);
    glBegin(GL_LINES);
    for (int i = 0; i < MAX_RAIN; i++) {
        float tilt = g_windSpeed * 0.04f;
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] - tilt, rainY[i] + rainLen[i]);
    }
    glEnd();
    glLineWidth(1.0f);
}

void emitSmoke(float x, float y) {
    smokeX[smokeIdx] = x + (rand() % 20) - 10;
    smokeY[smokeIdx] = y;
    smokeSize[smokeIdx] = 8.0f + (rand() % 6);
    smokeAlpha[smokeIdx] = 0.6f;
    smokeIdx = (smokeIdx + 1) % MAX_SMOKE;
}

void updateSmoke(float dt) {
    for (int i = 0; i < MAX_SMOKE; i++) {
        if (smokeAlpha[i] <= 0.0f) continue;
        smokeY[i] += 22.0f * dt;
        smokeX[i] += g_windSpeed * 0.4f * dt;
        smokeSize[i] += 5.0f * dt;
        smokeAlpha[i] -= 0.13f * dt;
    }
}

void clearSmoke() {
    for (int i = 0; i < MAX_SMOKE; i++) smokeAlpha[i] = 0.0f;
    smokeIdx = 0;
}

void drawSmoke() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (int i = 0; i < MAX_SMOKE; i++) {
        if (smokeAlpha[i] <= 0.0f) continue;
        float gray = 0.35f + 0.35f * (1.0f - g_aqi / 500.0f);
        glColor4f(gray, gray, gray, smokeAlpha[i]);
        drawCircle(smokeX[i], smokeY[i], smokeSize[i]);
    }
    glDisable(GL_BLEND);
}

void triggerLightning() {
    g_lightningFlash = 1.0f;
    lboltCount = 0;
    float x = 200.0f + rand() % (int)(WIN_W - 400);
    float y = WIN_H - 80.0f;
    lboltX[lboltCount] = x;
    lboltY[lboltCount] = y;
    lboltCount++;
    while (y > GROUND_Y + 20 && lboltCount < MAX_LBOLT) {
        x += (rand() % 80) - 40;
        y -= 50 + rand() % 50;
        if (y < GROUND_Y + 20) y = GROUND_Y + 20;
        lboltX[lboltCount] = x;
        lboltY[lboltCount] = y;
        lboltCount++;
    }
}

void drawLightning() {
    if (g_lightningFlash <= 0.0f || lboltCount < 2) return;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 0.85f, g_lightningFlash);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < lboltCount; i++) {
        glVertex2f(lboltX[i], lboltY[i]);
    }
    glEnd();
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
}

void drawSmogOverlay() {
    float alpha = (g_aqi - 100) / 400.0f * 0.65f;
    if (g_weatherState == WEATHER_SMOG && alpha < 0.35f) alpha = 0.35f;
    if (alpha <= 0.0f) return;
    if (alpha > 0.65f) alpha = 0.65f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.58f, 0.52f, 0.42f, alpha);
    drawRect(0, 0, WIN_W, WIN_H);
    glDisable(GL_BLEND);
}
