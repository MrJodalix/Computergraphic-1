/**
 * @file
 * Darstellungs-Moduls für die 3D-Szene.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glut.h>
#endif

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/* ---- Eigene Header einbinden ---- */
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "debugGL.h"
#include "types.h"
#include "logic.h"
#include "stringOutput.h"
#include "snake.h"
#include "unitObjekt3D.h"

/* ---- Globale Variablen ---- */
double interval = 0.0;
CGPoint3i prevPos = {0, 0, 0};

/**
 *  Zeichenfunktion eines skalierten Tetraeders
 */
void drawScaledTetrahedron(void)
{
    glPushMatrix();
    {
        glScalef(0.75f, 0.75f, 0.75f);
        drawUnitTetrahedron();
    }
    glPopMatrix();
}

/**
 *  Zeichenfunktion eines skalierten Tetraeders
 */
void drawScaledTetrahedronInverse(void)
{
    glPushMatrix();
    {
        glScalef(0.75f, 0.75f, 0.75f);
        drawUnitTetrahedronInverse();
    }
    glPopMatrix();
}

/**
 * Transformieren des Einheitsquadrats in ein freies Feld
 */
void drawFree3D(void)
{
    /* Material */
    float matAmbient[] = {0.2, 0.2, 0.2, 1.0};
    float matDiffuse[] = {0.4, 0.4, 0.4, 1.0};
    float matSpecular[] = {0.1, 0.1, 0.1, 1.0};
    float matShininess = 1;
    int *drawNorm = getDrawNorm();

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.44f, 0.44f, 0.44f);
        glScalef(FIELDSIZE3D, FIELDSIZE3D, 1);
        drawUnitSquare3D((*drawNorm), 200);
    }
    glPopMatrix();
}

/**
 * Transformieren der Einheitskugel in den Schlangenkopf und die Augen
 */
void drawSnakeHead3D(void)
{
    List snakeHead = getSnakeList();

    /* Material */
    float matAmbientWhite[] = {0.8f, 0.8f, 0.8f, 1.0f};
    float matDiffuseWhite[] = {0.9f, 0.9f, 0.9f, 1.0f};
    float matSpecularWhite[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float matShininessWhite = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessWhite);

    // linkes Auge
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef((float)snakeHead->snakePart[2], 0.0f, 1.0f, 0.0f);
        glTranslatef(-0.15f, 0.3f, 0.35f);
        glScalef(0.075f, 0.075f, 0.075f);
        drawUnitSphere();
    }
    glPopMatrix();
    // rechtes Auge
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef((float)snakeHead->snakePart[2], 0.0f, 1.0f, 0.0f);
        glTranslatef(0.15f, 0.3f, 0.35f);
        glScalef(0.075f, 0.075f, 0.075f);
        drawUnitSphere();
    }
    glPopMatrix();

    // Kopf
    /* Material */
    float matAmbientGreen[] = {0.1, 0.6, 0.1, 1.0};
    float matDiffuseGreen[] = {0.1, 0.6, 0.1, 1.0};
    float matSpecularGreen[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessGreen = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientGreen);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseGreen);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularGreen);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessGreen);

    glPushMatrix();
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glScalef(0.5f, 0.5f, 0.5f);
        drawUnitSphere();
    }
    glPopMatrix();
}

/**
 * Zeichnet den Koerper der Schlange als Wuerfel
 */
void drawSnakeCube(int drawNorm)
{
    /* Material des Wuerfels */
    float matAmbient[] = {0.1, 0.6, 0.1, 1.0};
    float matDiffuse[] = {0.1, 0.6, 0.1, 1.0};
    float matSpecular[] = {0.1, 0.1, 0.1, 1.0};
    float matShininess = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

    /* Frontflaeche */
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();

    /* rechte Seitenflaeche */
    glPushMatrix();
    {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();

    /* Rueckseitenflaeche */
    glPushMatrix();
    {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();

    /* linke Seitenflaeche */
    glPushMatrix();
    {
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();

    /* Deckelflaeche */
    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();

    /* Bodenflaeche */
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        glColor3f(0.0, 1.0, 0.0);
        drawUnitSquare3D(drawNorm, 50);
    }
    glPopMatrix();
}

/**
 *  Zeichnet die Schwanzspitze als Pyramide
 */
static void drawTail3D(void)
{
    /* Material des Wuerfels */
    float matAmbient[] = {0.1, 0.6, 0.1, 1.0};
    float matDiffuse[] = {0.1, 0.6, 0.1, 1.0};
    float matSpecular[] = {0.1, 0.1, 0.1, 1.0};
    float matShininess = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

    List snakeList = getSnakeList();
    int *score = getScore();
    CGPoint3i *snakeTail = getAt(snakeList, (*score) - 2);
    glPushMatrix();
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRotatef((*snakeTail)[2], 0.0f, 1.0f, 0.0f);
        drawRotatePyramid();
    }
    glPopMatrix();
}

/**
 *  Zeichnet die Waende in 3D als Wuerfel
 */
void drawWall3D(int drawNorm)
{
    /* Frontflaeche */
    glPushMatrix();
    {
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();
    /* rechte Seitenflaeche */
    glPushMatrix();
    {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();

    /* Rueckseitenflaeche */
    glPushMatrix();
    {
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();
    /* linke Seitenflaeche */
    glPushMatrix();
    {
        glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();
    /* Deckelflaeche */
    glPushMatrix();
    {
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();
    /* Bodenflaeche */
    glPushMatrix();
    {
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawWall(drawNorm);
    }
    glPopMatrix();
}

/**
 *  Zeichnet die Bombe in 3D als Kugel mit Lunte und Funken
 */
void drawBomb3D(void)
{
    // Funke
    /* Material */
    float matAmbientR[] = {1.0, 0.0, 0.0, 1.0};
    float matDiffuseR[] = {1.0, 0.0, 0.0, 1.0};
    float matSpecularR[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessR = 1;
    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientR);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseR);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularR);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessR);
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.85f, 0.85f, 0);
        glScalef(0.5f, 0.5f, 1.0);
        glLineWidth(2);
        drawUnitLine();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.85f, 0.85f, 0);
        glRotatef(90.0, 0, 0, 1);
        glScalef(0.5f, 0.5f, 1.0);
        glLineWidth(2);
        drawUnitLine();
    }
    glPopMatrix();
    // Bombe
    /* Material */
    float matAmbient[] = {0.0, 0.0, 0.0, 1.0};
    float matDiffuse[] = {0.0, 0.0, 0.0, 1.0};
    float matSpecular[] = {1.0, 1.0, 1.0, 1.0};
    float matShininess = 20;
    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glScalef(1.0f, 1.0f, 1.0f);
        drawUnitSphere();
    }
    glPopMatrix();

    // Lunte
    /* Material */
    float matAmbientWhite[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float matDiffuseWhite[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float matSpecularWhite[] = {0.1f, 0.1f, 0.1f, 1.0f};
    float matShininessWhite = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientWhite);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseWhite);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularWhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessWhite);

    glPushMatrix();
    {
        glColor3f(1.0f, 0.5f, 0.5f);
        glTranslatef(0.5f, 0.5f, 0);
        glRotatef(225.0, 0, 0, 1);
        glLineWidth(2);
        drawUnitLine();
    }
    glPopMatrix();
}

/**
 * Zeichnen-Funktion zur Zeichnung der Bombe mit der richtigen Skalierung
 */
void drawScaledBomb3D(void)
{
    CGPoint3i *bombCoord = getBombCoord3D();
    float *scale = getScale();
    glPushMatrix();
    {
        glTranslatef(-FIELDSIZE3D / 2 + (*bombCoord)[0] * (FIELDSIZE3D / FIELDSIZE) + 1, 1.0f, FIELDSIZE3D / 2 - (*bombCoord)[2] * (FIELDSIZE3D / FIELDSIZE) - 0.5f);
        glScalef((*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2));
        drawBomb3D();
    }
    glPopMatrix();
}

/**
 * Zeichnet die Explosion in 3D als mehrere bunte Kugeln
 */
void drawExplosion3D(void)
{
    // ROT
    /* Material */
    float matAmbientR[] = {0.6, 0.0, 0.0, 1.0};
    float matDiffuseR[] = {1.0, 0.0, 0.0, 1.0};
    float matSpecularR[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessR = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientR);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseR);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularR);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessR);

    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.1f, 0.3f);
        glScalef(0.125f, 0.125f, 0.125f);
        drawUnitSphere();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.2f, 0.2f, 0.1f);
        glScalef(0.2f, 0.2f, 0.2f);
        drawUnitSphere();
    }
    glPopMatrix();

    // GELB
    /* Material */
    float matAmbientY[] = {0.6, 0.6, 0.0, 1.0};
    float matDiffuseY[] = {0.8, 0.8, 0.0, 1.0};
    float matSpecularY[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessY = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientY);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseY);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularY);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessY);

    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslatef(0.2f, 0.05f, -0.25f);
        glScalef(0.15f, 0.15f, 0.15f);
        drawUnitSphere();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslatef(-0.25f, 0.05f, 0.2f);
        glScalef(0.15f, 0.15f, 0.15f);
        drawUnitSphere();
    }
    glPopMatrix();

    // ORANGE
    /* Material */
    float matAmbientO[] = {0.4, 0.2, 0.0, 1.0};
    float matDiffuseO[] = {1.0, 0.5, 0.0, 1.0};
    float matSpecularO[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessO = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientO);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseO);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularO);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessO);

    glPushMatrix();
    {
        glColor3f(1.0f, 0.5f, 0.0f);
        glTranslatef(0.1f, 0.2f, 0.35f);
        glScalef(0.15f, 0.15f, 0.15f);
        drawUnitSphere();
    }
    glPopMatrix();

    // GRAU
    /* Material */
    float matAmbientG[] = {0.2, 0.2, 0.2, 1.0};
    float matDiffuseG[] = {0.4, 0.4, 0.4, 1.0};
    float matSpecularG[] = {0.1, 0.1, 0.1, 1.0};
    float matShininessG = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbientG);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuseG);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecularG);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininessG);
    glPushMatrix();
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glTranslatef(0.3f, 0.2f, 0.25f);
        glScalef(0.125f, 0.125f, 0.125f);
        drawUnitSphere();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glTranslatef(-0.15f, 0.2f, -0.1f);
        glScalef(0.15f, 0.15f, 0.15f);
        drawUnitSphere();
    }
    glPopMatrix();
}

/**
 * Zeichnet die Nahrung in 3D als Tetraeder mit Transparenz
 */
void drawFood3D(void)
{
    CGPoint3i *foodCoord = getFoodCoord3D();
    float *scale = getScale();

    /* Material */
    float matAmbient2[] = {0.6, 0.6, 0.1, 0.4};
    float matDiffuse2[] = {1.0, 1.0, 0.1, 0.4};
    float matSpecular2[] = {0.1, 0.1, 0.1, 0.4};
    float matShininess2 = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient2);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse2);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular2);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess2);

    glPushMatrix();
    {
        glColor4f(1.0f, 1.0f, 0.0f, 0.4f);
        glTranslatef(-FIELDSIZE3D / 2 + (*foodCoord)[0] * (FIELDSIZE3D / FIELDSIZE) + 1, 1.0f, FIELDSIZE3D / 2 - (*foodCoord)[2] * (FIELDSIZE3D / FIELDSIZE) - 0.5f);
        glScalef((*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2));
        drawScaledTetrahedronInverse();
    }
    glPopMatrix();

    /* Material */
    float matAmbient[] = {0.6, 0.6, 0.1, 0.6};
    float matDiffuse[] = {1.0, 1.0, 0.1, 0.6};
    float matSpecular[] = {0.1, 0.1, 0.1, 0.6};
    float matShininess = 1;

    /* Setzen der Material-Parameter */
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);

    glPushMatrix();
    {
        glColor4f(1.0f, 1.0f, 0.0f, 0.6f);
        glTranslatef(-FIELDSIZE3D / 2 + (*foodCoord)[0] * (FIELDSIZE3D / FIELDSIZE) + 1, 1.0f, FIELDSIZE3D / 2 - (*foodCoord)[2] * (FIELDSIZE3D / FIELDSIZE) - 0.5f);
        glScalef((*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2), (*scale) * (FIELDSIZE3D / 2));
        drawScaledTetrahedron();
    }
    glPopMatrix();
}

/**
 * Zeichen-Funktion schreibt den Score(Anzahl der Koerperteile) und
 * Highscore (bisher groeszte Anzahl an Koerperteilen)
 */
void drawText(void)
{
    /* Textfarbe */
    GLfloat textColor[3] = {1.0f, 0.0f, 0.0f};

    int *score = getScore();

    int *highScore = getHighScore();

    /* Text ausgegeben */
    drawString(0.15f, 0.99f, textColor, "Score: %i", score[0]);
    /* Text ausgegeben */
    drawString(0.3f, 0.99f, textColor, "Highscore: %i", highScore[0]);
}

/**
 * Zeichen-Funktion zeigt den verloren Screen.
 */
void drawLosingScene(void)
{
    /* Textfarbe */
    GLfloat textColor[3] = {1.0f, 0.0f, 0.0f};

    int *highScore = getHighScore();

    /* Text ausgegeben */
    drawString(0.05f, 0.19f, textColor, "GAME OVER");
    drawString(0.05f, 0.39f, textColor, "Highscore: %i", highScore[0]);
    drawString(0.05f, 0.59f, textColor, "Beliebige normale Taste druecken um neu zu starten.");
    drawString(0.05f, 0.79f, textColor, "Oder 1/2/3/4 um ein explizites Level neu zu waehlen.");
    drawString(0.05f, 0.99f, textColor, "Oder q/Q/Esc zum Beenden.");
}

/**
 * Zeichen-Funktion zeigt den Hilfe Screen.
 */
void drawHelp(void)
{
    /* Textfarbe */
    GLfloat textColor[3] = {1.0f, 0.0f, 0.0f};

    /* Text ausgegeben */
    drawString(0.05f, 0.09f, textColor, "Tastenbelegung");
    drawString(0.05f, 0.19f, textColor, "1/2/3/4 : Waehle ein explizites Level.");
    drawString(0.05f, 0.29f, textColor, "q/Q/Esc : Beendet das Spiel.");
    drawString(0.05f, 0.39f, textColor, "h/H : Ruft diese Hilfeausgabe auf und pausiert das Spiel.");
    drawString(0.05f, 0.44f, textColor, "p/P : Pausiert das Spiel.");
    drawString(0.05f, 0.49f, textColor, "Steuerung der Schlange mit den Pfeiltasten.");
    drawString(0.05f, 0.59f, textColor, "e/s/d/f/+/- : steuert die Kamerabewegung");
    drawString(0.05f, 0.64f, textColor, "a/A : wechselt zwischen zeitlich Animiert und statisch");
    drawString(0.05f, 0.69f, textColor, "c/C : wechselt zwischen Vogel- und Egoperspektive");
    drawString(0.05f, 0.74f, textColor, "F1 : Schaltet zwischen normalem und WireFrame/-Mode");
    drawString(0.05f, 0.79f, textColor, "F2 : Normale zeichnen");
    drawString(0.05f, 0.84f, textColor, "F3 : Globale Beleuchtung (an/aus)");
    drawString(0.05f, 0.89f, textColor, "F4 : Spotlight (an/aus)");
    drawString(0.05f, 0.94f, textColor, "Beliebige normale Taste druecken um neu zu starten.");
}

/**
 * Hilfsfunktion die die Position für die Animation festlegt
 * @param snakePartX die X-Koordinate des Schlangenkopfes
 * @param snakePartZ die Z-Koordinate des Schlangenkopfes
 * @param snakeDirect die Richtung in die die schlange wandert
 * @return der veraenderte Wert
 */
float getNewPos(int snakePartX, int snakePartZ, int snakeDirect)
{
    List snakeList = getSnakeList();
    float bodyCoordX = snakePartX;
    float bodyCoordZ = snakePartZ;

    if ((prevPos[0] != snakeList->snakePart[0]) ||
        (prevPos[1] != snakeList->snakePart[1]))
    {
        prevPos[0] = snakeList->snakePart[0];
        prevPos[1] = snakeList->snakePart[1];
        interval = 0;
    }
    if (snakeDirect == 0)
    {
        bodyCoordZ -= interval - 1;
    }
    if (snakeDirect == 90)
    {
        bodyCoordX += interval - 1;
    }
    if (snakeDirect == 180)
    {
        bodyCoordZ += interval - 1;
    }
    if (snakeDirect == 270)
    {
        bodyCoordX -= interval - 1;
    }
    if (bodyCoordX != snakePartX)
    {
        return bodyCoordX;
    }
    else
    {
        return bodyCoordZ;
    }
}

/**
 * Zeichen-Funktion der 3D-Szene.
 * Stellt die Waende, die Schlange, die Bombe und die Nahrung dar sowie die
 * Explosionen dar.
 */
void drawScene3D(void)
{
    int *score = getScore();
    // Soll nur gezeichnet werden, wenn eine Schlange vorhanden ist.
    if (*score != 0)
    {
        int *mainLight = getDrawMainLight();
        int *spotlight = getDrawSpotlight();

        List snakeList = getSnakeList();
        if ((!*mainLight) || (!*spotlight))
        {
            /* Lichtberechnung aktivieren */
            glEnable(GL_LIGHTING);

            if (!*mainLight)
            {
                /* erste (feste) Lichtquelle */
                CGPoint4f lightPos = {3.0f, 2.0f, 1.0f, 0.0f};

                /* Position der ersten Lichtquelle setzen */
                glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
                /* Erste Lichtquelle aktivieren */
                glEnable(GL_LIGHT0);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            if (!*spotlight)
            {
                float lightX = snakeList->snakePart[0];
                float lightZ = snakeList->snakePart[1];
                float lightDirectX = 0.0f;
                float lightDirectZ = 0.0f;
                if (snakeList->snakePart[2] == 0 || snakeList->snakePart[2] == 180)
                {
                    lightZ = getNewPos(snakeList->snakePart[0], snakeList->snakePart[1], snakeList->snakePart[2]);
                    if (snakeList->snakePart[2] == 0)
                    {
                        lightDirectZ += 1;
                    }
                    else
                    {
                        lightDirectZ -= 1;
                    }
                }
                else
                {
                    lightX = getNewPos(snakeList->snakePart[0], snakeList->snakePart[1], snakeList->snakePart[2]);
                    if (snakeList->snakePart[2] == 90)
                    {
                        lightDirectX += 1;
                    }
                    else
                    {
                        lightDirectX -= 1;
                    }
                }
                /* zweite (sich bewegende) Lichtquelle */
                /* Spotlichtquelle im Ursprung, Richtung entlang negativer z-Achse */
                /* wird nachfolgend noch endgueltig positioniert */

                float spotX = (-FIELDSIZE3D / 2 + lightX * (FIELDSIZE3D / FIELDSIZE) + 1);
                float spotZ = (FIELDSIZE3D / 2 - lightZ * (FIELDSIZE3D / FIELDSIZE) - 1);
                CGPoint4f lightPos1 = {spotX, 1.0f, spotZ, 1.0f};
                CGVector3f lightDirection1 = {lightDirectX, -1.0f, lightDirectZ};
                glPushMatrix();
                {
                    /* Matrixoperationen fuer Position der zweiten Lichtquelle*/
                    glTranslatef(0.0f, 5.0f, 0.0f);

                    /* Zweite Lichtquelle positionieren */
                    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
                    /* Richtung der zweiten Lichtquelle festlegen */
                    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection1);
                }
                glPopMatrix();

                /* Zweite Lichtquelle aktivieren */
                glEnable(GL_LIGHT1);
            }
        }

        int *score = getScore();
        int *toggle = getToggle();
        esLevel *level = switchLevel();
        esFieldType fieldType = FT_FREE;

        GLuint *wallListId = getDisplayList(3);
        GLuint *explodeListId = getDisplayList(4);
        GLuint *bodyListId = getDisplayList(5);

        GLuint *wallListIdNorm = getDisplayList(6);
        GLuint *bodyListIdNorm = getDisplayList(7);

        int *drawNorm = getDrawNorm();
        int *pause = getPause();

        //  Ablaufen des Level-Arrays
        for (int y = 0; y < FIELDSIZE; y++)
        {
            for (int x = 0; x < FIELDSIZE; x++)
            {
                fieldType = (*level)[y][x];
                switch (fieldType)
                {
                case FT_FREE:
                    break;
                // Zeichnen der Waende
                case FT_WALL:
                    glPushMatrix();
                    {
                        glTranslatef(-FIELDSIZE3D / 2 + x * (FIELDSIZE3D / FIELDSIZE) + 1,
                                     1,
                                     FIELDSIZE3D / 2 - y * (FIELDSIZE3D / FIELDSIZE) - 1);
                        glScalef(FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE);
                        if ((*drawNorm))
                        {
                            glCallList(*wallListIdNorm);
                        }
                        else
                        {
                            glCallList(*wallListId);
                        }
                    }
                    glPopMatrix();
                    break;
                // Zeichnen der Explosion
                case FT_EXPLODED:
                    glPushMatrix();
                    {
                        glTranslatef(-FIELDSIZE3D / 2 + x * (FIELDSIZE3D / FIELDSIZE) + 1,
                                     1,
                                     FIELDSIZE3D / 2 - y * (FIELDSIZE3D / FIELDSIZE) - 1);
                        glScalef(FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE);
                        glCallList(*explodeListId);
                    }
                    glPopMatrix();
                    break;
                default:
                    break;
                }
            }
        }

        // Zeichnen des Schlangenkopfes
        if (!isEmpty(snakeList))
        {
            fieldType = (*level)[snakeList->snakePart[1]][(snakeList->snakePart)[0]];
        }

        if (!*pause)
        {
            interval += 0.032;
        }

        // Zeichnen des Schlangenkoerpers und des Schwanzspitze
        if (fieldType == FT_FREE)
        {

            List run = snakeList;

            int *drawAnimated = getAnimated();

            float bodyCoordX = snakeList->snakePart[0];
            float bodyCoordZ = snakeList->snakePart[1];
            if (!*drawAnimated)
            {
                if (run->snakePart[2] == 90 || run->snakePart[2] == 270)
                {
                    bodyCoordX = getNewPos(run->snakePart[0], run->snakePart[1], run->snakePart[2]);
                }
                else
                {
                    bodyCoordZ = getNewPos(run->snakePart[0], run->snakePart[1], run->snakePart[2]);
                }
            }
            float bodyX = (-FIELDSIZE3D / 2 + bodyCoordX * (FIELDSIZE3D / FIELDSIZE) + 1);
            float bodyZ = (FIELDSIZE3D / 2 - bodyCoordZ * (FIELDSIZE3D / FIELDSIZE) - 1);

            glPushMatrix();
            {
                glTranslatef(bodyX, 1.0f, bodyZ);
                glScalef(FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE);
                drawSnakeHead3D();
            }
            glPopMatrix();
            for (int i = 1; i < (*score); i++)
            {
                run = run->next;
                float bodyCoordX = (run->snakePart[0]);
                float bodyCoordZ = (run->snakePart[1]);
                if (!*drawAnimated)
                {
                    if (run->snakePart[2] == 90 || run->snakePart[2] == 270)
                    {
                        bodyCoordX = getNewPos(run->snakePart[0], run->snakePart[1], run->snakePart[2]);
                    }
                    else
                    {
                        bodyCoordZ = getNewPos(run->snakePart[0], run->snakePart[1], run->snakePart[2]);
                    }
                }
                float bodyX = (-FIELDSIZE3D / 2 + bodyCoordX * (FIELDSIZE3D / FIELDSIZE) + 1);
                float bodyZ = (FIELDSIZE3D / 2 - bodyCoordZ * (FIELDSIZE3D / FIELDSIZE) - 1);
                if (run->next == NULL)
                {

                    glPushMatrix();
                    {
                        // Schwanzspitze verschieben
                        glTranslatef(bodyX, 1.0f, bodyZ);
                        // Schwanzspitze skalieren
                        glScalef(FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE);
                        drawTail3D();
                    }
                    glPopMatrix();
                }
                else
                {
                    glPushMatrix();
                    {
                        // Body verschieben
                        glTranslatef(bodyX, 1.0f, bodyZ);
                        // Body groesser skalieren
                        glScalef(FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE, FIELDSIZE3D / FIELDSIZE);
                        // Body in DisplayList zeichnen
                        if ((*drawNorm))
                        {
                            glCallList(*bodyListIdNorm);
                        }
                        else
                        {
                            glCallList(*bodyListId);
                        }
                    }
                    glPopMatrix();
                }
            }
        }

        // Zeichnen der Nahrung und der Bombe
        if (*toggle == 0)
        {
            drawFood3D();
        }
        else
        {
            drawScaledBomb3D();
        }

        drawText();
        // Zeichnen des Bodens
        drawFree3D();

        if (!*mainLight)
        {
            /* Lichtquelle deaktivieren */
            glDisable(GL_LIGHT0);

            /* Spotlight deaktivieren */
            glDisable(GL_LIGHT1);

            /* Lichtberechnung deaktivieren */
            glDisable(GL_LIGHTING);
        }
    }
    else
    {
        int *helpScore = getHelpScore();
        if (*helpScore == 0)
        {
            drawLosingScene();
        }
        else
        {
            drawHelp();
        }
    }
}
/**
 * Initialisierung der Lichtquellen.
 * Setzt Eigenschaften der Lichtquellen (Farbe, Oeffnungswinkel, ...)
 */
void initLight(void)
{
    /* Farbe der ersten Lichtquelle */
    CGPoint4f lightColor0[3] =
        {{0.3f, 0.3f, 0.3f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.8f, 0.8f, 0.8f, 1.0f}};

    /* Farbe der zweiten Lichtquelle */
    CGPoint4f lightColor1[3] =
        {{0.0f, 0.0f, 0.0f, 1.0f}, {2.0f, 2.0f, 2.0f, 1.0f}, {3.0f, 3.0f, 3.0f, 1.0f}};

    /* Oeffnungswinkel der zweiten Lichtquelle */
    GLdouble lightCutoff1 = 10.0f;
    /* Lichtverteilung im Lichtkegel der zweiten Lichtquelle */
    GLdouble lightExponent1 = 30.0f;

    /* Farbe der ersten Lichtquelle setzen */
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightColor0[0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0[1]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0[2]);

    /* Farbe der zweiten Lichtquelle setzen */
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightColor1[0]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1[1]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1[2]);

    /* Spotlight-Eigenschaften der zweiten Lichtquelle setzen */
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lightCutoff1);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lightExponent1);
}

/**
 * (De-)aktiviert den Normalen-Modus.
 */
void toggleNormals(void)
{
    /* Flag: Normals: ja/nein */
    static GLboolean normals = GL_FALSE;
    int *drawNorm = getDrawNorm();

    /* Modus wechseln */
    normals = !normals;

    if (normals)
    {
        (*drawNorm) = 1;
    }
    else
    {
        (*drawNorm) = 0;
    }
}

/**
 * (De-)aktiviert die globale Beleuchtung.
 */
void toggleMainLight(void)
{
    /* Flag: MainLight: ja/nein */
    static GLboolean mainLight = GL_FALSE;
    int *drawMainLight = getDrawMainLight();

    /* Modus wechseln */
    mainLight = !mainLight;

    if (mainLight)
    {
        (*drawMainLight) = 1;
    }
    else
    {
        (*drawMainLight) = 0;
    }
}

/**
 * (De-)aktiviert das Spotlight.
 */
void toggleSpotlight(void)
{
    /* Flag: Spotlight: ja/nein */
    static GLboolean spotlight = GL_FALSE;
    int *drawSpotlight = getDrawSpotlight();

    /* Modus wechseln */
    spotlight = !spotlight;

    if (spotlight)
    {
        (*drawSpotlight) = 1;
    }
    else
    {
        (*drawSpotlight) = 0;
    }
}

/**
 * (De-)aktiviert den Animations-Modus.
 */
void toggleAnimation(void)
{
    /* Flag: animation: ja/nein */
    static GLboolean animation = GL_FALSE;
    int *drawAnimated = getAnimated();

    /* Modus wechseln */
    animation = !animation;

    if (animation)
    {
        (*drawAnimated) = 1;
    }
    else
    {
        (*drawAnimated) = 0;
    }
}