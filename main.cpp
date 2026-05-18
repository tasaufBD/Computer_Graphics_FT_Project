#include <windows.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cctype>

//My defined headers
#include "utils.h"
#include "weather.h"
#include "scene1.h"
#include "scene2.h"

//Constants
#define TIMER_MS        16
#define DT              (TIMER_MS / 1000.0f)
#define FILE_INTERVAL   2.0f
#define SMOKE_INTERVAL  0.10f
#define LIGHTNING_MIN   2.0f
#define LIGHTNING_MAX   5.0f

//Helper
const char* weatherName() {
    if (g_weatherState == WEATHER_CLEAR) return "Clear";
    if (g_weatherState == WEATHER_STORM) return "Kalboishakhi Storm";
    if (g_weatherState == WEATHER_SMOG)  return "Heavy Smog";
    return "?";
}

void drawStartupScreen() {
    glClearColor(0.05f, 0.08f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Title
    glColor3f(0.95f, 0.85f, 0.45f);
    drawText(WIN_W * 0.5f - 140, WIN_H - 160, "WEATHER EXPLORER", GLUT_BITMAP_TIMES_ROMAN_24);
    glColor3f(0.75f, 0.80f, 0.90f);
    drawText(WIN_W * 0.5f - 300, WIN_H - 195,
             "An Interactive Urban Simulation with Real-Time Atmospheric Visualization", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.40f, 0.45f, 0.55f);
    glBegin(GL_LINES);
    glVertex2f(WIN_W * 0.5f - 320, WIN_H - 215);
    glVertex2f(WIN_W * 0.5f + 320, WIN_H - 215);
    glEnd();

    // Group Info
    glColor3f(0.90f, 0.90f, 0.95f);
    drawText(WIN_W * 0.5f - 50, WIN_H - 260, "Group: L", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.3f, 0.05f, 0.90f);
    drawText(WIN_W * 0.5f - 85, WIN_H - 305,
             "Team Members:", GLUT_BITMAP_HELVETICA_18);
    drawText(WIN_W * 0.5f - 225, WIN_H - 350, "Md: Tasauf Islam       -   Student ID: 23-54025-3", GLUT_BITMAP_HELVETICA_18);
    drawText(WIN_W * 0.5f - 225, WIN_H - 380, "Mehereen Khan Mila  -   Student ID: 23-53560-3", GLUT_BITMAP_HELVETICA_18);

    glColor3f(0.60f, 0.70f, 0.85f);
    drawText(WIN_W * 0.5f - 130, WIN_H - 440, "Course: Computer Graphics", GLUT_BITMAP_HELVETICA_18);

    glColor3f(1.0f, 0.85f, 0.30f);
    drawText(WIN_W * 0.5f - 140, 110, "Press  S  to start the simulation", GLUT_BITMAP_HELVETICA_18);
    glColor3f(0.55f, 0.60f, 0.70f);
    drawText(WIN_W * 0.5f - 60, 75, "Press  ESC  to exit", GLUT_BITMAP_HELVETICA_12);
}

// HUD
void drawHUD() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.15f);
    drawRect(4, WIN_H - 100, 200, 134);
    glDisable(GL_BLEND);

    char buf[128];
    glColor3f(1.0f, 1.0f, 1.0f);

    sprintf(buf, "Scene: %s",
            g_currentScene == 1 ? "Residential" : "Industrial");
    drawText(18, WIN_H - 28, buf, GLUT_BITMAP_HELVETICA_12);

    sprintf(buf, "Weather: %s", weatherName());
    drawText(18, WIN_H - 46, buf, GLUT_BITMAP_HELVETICA_12);

    sprintf(buf, "AQI: %d   Wind: %.0f   Speed x%.1f",
            g_aqi, g_windSpeed, g_animSpeed);
    drawText(18, WIN_H - 64, buf, GLUT_BITMAP_HELVETICA_12);

    sprintf(buf, "Time: %s   Live mode: %s",
            g_isDay ? "Day" : "Night",
            g_liveMode ? "ON" : "off");
    drawText(18, WIN_H - 82, buf, GLUT_BITMAP_HELVETICA_12);
}

//Call backs
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (g_currentScene == 0) {
        drawStartupScreen();
        glutSwapBuffers();
        return;
    }

    drawSky();
    if (!g_isDay) drawStars();
    drawSunMoon();
    drawClouds();

    if (g_currentScene == 1) drawScene1();
    else                     drawScene2();

    drawSmoke();
    drawRain();
    drawLightning();
    drawSmogOverlay();
    drawHUD();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y) {
    unsigned char k = (unsigned char)tolower(key);

    if (g_currentScene == 0) {
        if (k == 's')       g_currentScene = 1;
        else if (key == 27) exit(0);
        glutPostRedisplay();
        return;
    }

    if (k == '1') {
        if (g_currentScene != 1) clearSmoke();
        g_currentScene = 1;
    } else if (k == '2') {
        g_currentScene = 2;
    } else if (k == 'c') {
        g_weatherState = WEATHER_CLEAR;
        if (g_aqi > 100) g_aqi = 50;
    } else if (k == 'k') {
        g_weatherState = WEATHER_STORM;
    } else if (k == 'm') {
        g_weatherState = WEATHER_SMOG;
        g_aqi = 250;
    } else if (k == 'n') {
        g_isDay = !g_isDay;
    } else if (k == 'l') {
        g_liveMode = !g_liveMode;
    } else if (k == 'a') {
        g_playerX -= 18.0f;
    } else if (k == 'd') {
        g_playerX += 18.0f;
    } else if (key == 27) {
        exit(0);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;
    if (g_currentScene == 0) return;

    if (button == GLUT_LEFT_BUTTON) {
        g_windSpeed += 12.0f;
        g_animSpeed += 0.15f;
    } else if (button == GLUT_RIGHT_BUTTON) {
        g_windSpeed -= 12.0f;
        g_animSpeed -= 0.15f;
    }
    g_windSpeed = clampf(g_windSpeed, 0.0f, 200.0f);
    g_animSpeed = clampf(g_animSpeed, 0.3f, 3.0f);
}

// Timing
float smokeT = 0.0f;
float lightT = LIGHTNING_MIN;
float fileT = 0.0f;

void timer(int v) {
    if (g_currentScene != 0) {
        g_time += DT;

        // Sun/moon drifting
        g_sunAngle += 0.012f * DT * g_animSpeed;
        if (g_sunAngle > PI) g_sunAngle = 0.0f;

        updateClouds(DT);
        updateRain(DT);
        updateSmoke(DT);

        smokeT += DT;
        if (smokeT >= SMOKE_INTERVAL) {
            smokeT = 0.0f;
            if (g_currentScene == 2) emitScene2Smoke();
        }

        // Lightning
        if (g_weatherState == WEATHER_STORM) {
            lightT -= DT;
            if (lightT <= 0.0f) {
                triggerLightning();
                lightT = LIGHTNING_MIN +
                         (rand() % 100) / 100.0f * (LIGHTNING_MAX - LIGHTNING_MIN);
            }
            g_lightningFlash -= DT * 3.5f;
            if (g_lightningFlash < 0.0f) g_lightningFlash = 0.0f;
        } else {
            g_lightningFlash = 0.0f;
        }

        // Live-mode file poll
        if (g_liveMode) {
            fileT += DT;
            if (fileT >= FILE_INTERVAL) {
                fileT = 0.0f;
                readWeatherFile();
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(TIMER_MS, timer, 0);
}



int main(int argc, char** argv) {
    srand((unsigned)time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize((int)WIN_W, (int)WIN_H);
    glutInitWindowPosition(40, 40);
    glutCreateWindow("Weather Explorer - Interactive Urban Simulation");

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIN_W, 0, WIN_H);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initWeather();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(TIMER_MS, timer, 0);

    glutMainLoop();
    return 0;
}
