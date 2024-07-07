/**
 * @file
 * Darstellungs-Modul für die 2D-Szene.
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
#include "logic.h"
#include "types.h"
#include "debugGL.h"
#include "stringOutput.h"
#include "snake.h"

/* ---- Globale Daten ---- */

// Skalierungsvariable fuer Food und Bomb
int scaling = SCALING;
// Timervariable fuer die Dauer der Explosion
int explodingTimer = SCALING;
// Timerkonstante fuer die Skalierung
int const timer = 1000000;

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats
 */
void drawUnitSquare(int drawNorm)
{
    /* In der Logik berechnete Position beziehen */
    CGPoint2f *quadCenter = getQuadCenter();

    /* Festlegung eines Einheitsobjektes für Quadrate */
    glBegin(GL_QUADS);
    {
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex2d((*quadCenter)[0] + 0.5, (*quadCenter)[1] + 0.5);
        glVertex2d((*quadCenter)[0] - 0.5, (*quadCenter)[1] + 0.5);
        glVertex2d((*quadCenter)[0] - 0.5, (*quadCenter)[1] - 0.5);
        glVertex2d((*quadCenter)[0] + 0.5, (*quadCenter)[1] - 0.5);
    }
    glEnd();
    if (drawNorm)
    {
        drawUnitNormalLine(0.0f, 0.0f, 1.0f);
    }
}

/**
 * Zeichen-Funktion zur Erstellung des Einheitskreis.
 */
void drawUnitCircle(void)
{
    GLUquadricObj *unitCircle = 0;
    unitCircle = gluNewQuadric();
    gluDisk(unitCircle, 0, 1, 32, 1);
    gluDeleteQuadric(unitCircle);
}

/**
 * Zeichen-Funktion zur Erstellung einer Einheitslinie.
 */
void drawUnitLine(void)
{
    CGPoint2f *lineCenter = getLineCenter();

    glBegin(GL_LINES);
    {
        glVertex2d((*lineCenter)[0] + 0.5f, (*lineCenter)[1]);
        glVertex2d((*lineCenter)[0] - 0.5f, (*lineCenter)[1]);
    }
    glEnd();
}

/**
 * Zeichen-Funktion fuer ein gleichschenkliges Einheitsdreieck
 */
void drawUnitIsoscelesTriangle(void)
{
    CGPoint2f *triangleCenter = getTriangleCenter();

    glBegin(GL_TRIANGLES);
    {
        glVertex2d((*triangleCenter)[0] + 0.5f, (*triangleCenter)[1] - 0.5f);
        glVertex2d((*triangleCenter)[0], (*triangleCenter)[1] + 0.5f);
        glVertex2d((*triangleCenter)[0] - 0.5f, (*triangleCenter)[1] - 0.5f);
    }
    glEnd();
}

/**
 * Zeichen-Funktion fuer ein gleichseitiges Einheitsdreieck
 */
void drawUnitEquilateralTriangle(void)
{
    GLUquadricObj *unitCircle = 0;
    unitCircle = gluNewQuadric();
    gluDisk(unitCircle, 0, 1, 3, 1);
    gluDeleteQuadric(unitCircle);
}

/**
 * Transformieren des Einheitsquadrats in ein freies Feld
 */
void drawFree(void)
{
    int *drawNorm = getDrawNorm();
    // Spielfeld
    glPushMatrix();
    {
        glColor3f(0.44f, 0.44f, 0.44f);
        glScalef(1.8f, 1.8f, 1.0f);
        drawUnitSquare((*drawNorm));
    }
    glPopMatrix();
    // Untergrund
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        glScalef(1.85f, 1.85f, 1.0f);
        drawUnitSquare((*drawNorm));
    }
    glPopMatrix();
}

/**
 * Transformieren des Einheitsquadrats und der Einheitslinien in eine Wand.
 */
void drawWall(int drawNorm)
{
    float linePosX = -0.375f;
    float linePosY = -0.375f;

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

    // Horizontale Linien
    for (float i = -0.25f; i <= 0.25f; i += 0.25f)
    {
        glPushMatrix();
        {
            glColor3f(0.827f, 0.827f, 0.827f);
            glTranslatef(0, i, 0);
            glScalef(1, 0.015, 1);
            glLineWidth(2);
            drawUnitLine();
        }
        glPopMatrix();
    }
    // Vertikale Linien
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        {
            glTranslatef(linePosX, linePosY, 0);
            glRotatef(90.0f, 0, 0, 1);
            glScalef(0.25f, 0.015, 1);
            glLineWidth(2);
            drawUnitLine();
        }
        glPopMatrix();
        // Zur korrekten Darstellung der Moertellinien in der Wand
        if (i % 5 != 3)
        {
            // Der Abstand zwischen den vertikalen Linien
            linePosX += 0.375f;
        }
        else
        {
            // Der Abstand der vertikalen Linien bei einer Verschiebung in der Y-Achse
            linePosX += 0.5f;
        }
        // Reset X-Achse, Hochsetzen Y-Achse
        if (linePosX >= 0.5f)
        {
            linePosX -= 1.0f;
            linePosY += 0.25f;
        }
    }

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
        drawUnitSquare(drawNorm);
    }
    glPopMatrix();
}

/**
 * Transformieren des Einheitskreises und der Einheitslinien in eine Bombe.
 */
void drawBomb(void)
{
    // Funke
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
    glPushMatrix();
    {
        glColor3f(0.0f, 0.0f, 0.0f);
        drawUnitCircle();
    }
    glPopMatrix();
    // Lunte
    glPushMatrix();
    {
        glColor3f(1.0f, 0.5f, 0.5f);
        glTranslatef(0.5f, 0.5f, 0);
        glRotatef(45.0, 0, 0, 1);
        glLineWidth(2);
        drawUnitLine();
    }
    glPopMatrix();
}

/**
 * Zeichnen-Funktion zur Zeichnung der Bombe mit der richtigen Skalierung
 */
void drawScaledBomb(void)
{
    CGPoint2i *bombCoord = getBombCoord();
    float *scale = getScale();
    glPushMatrix();
    {
        glTranslatef(FIELDBORDER + (*bombCoord)[0] * QUAD_SIDE, FIELDBORDER + (*bombCoord)[1] * QUAD_SIDE, 0.0f);
        glScalef(*scale, *scale, 1);
        drawBomb();
    }
    glPopMatrix();
}

/*
 * Zeichenfunktion der Explosion
 * mit 8 skalierten Einheitskreisen
 */
void drawExplosion(void)
{
    // ROT
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    // GELB
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslatef(0.2f, 0.05f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    // ORANGE
    glPushMatrix();
    {
        glColor3f(1.0f, 0.5f, 0.0f);
        glTranslatef(0.1f, 0.2f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    // GRAU
    glPushMatrix();
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glTranslatef(0.2f, -0.2f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(-0.25f, 0.2f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslatef(-0.2f, 0.05f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(1.0f, 0.5f, 0.0f);
        glTranslatef(-0.1f, -0.2f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glColor3f(0.2f, 0.2f, 0.2f);
        glTranslatef(-0.1f, 0.2f, 0.0f);
        glScalef(0.125f, 0.125f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
}

/**
 * Zeichen-Funktion.
 * Transformieren des Einheitskreis in den Schlangenkopf.
 */
void drawSnakeHead(void)
{
    List snakeHead = getSnakeList();
    // linkes Auge
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef((float)snakeHead->snakePart[2], 0.0f, 0.0f, 1.0f);
        glTranslatef(-0.15f, -0.3f, 0.0f);
        glScalef(0.075f, 0.075f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    // rechtes Auge
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glRotatef((float)snakeHead->snakePart[2], 0.0f, 0.0f, 1.0f);
        glTranslatef(0.15f, -0.3f, 0.0f);
        glScalef(0.075f, 0.075f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
    // Kopf
    glPushMatrix();
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glScalef(0.5f, 0.5f, 0.0);
        drawUnitCircle();
    }
    glPopMatrix();
}

/**
 * Zeichen-Funktion des Schlangenkoerpers
 */
void drawBody(void)
{
    int *drawNorm = getDrawNorm();
    glPushMatrix();
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        drawUnitSquare((*drawNorm));
    }
    glPopMatrix();
}

/**
 * Zeichen-Funktion der Schlangenschwanzspitze
 */
void drawTail(void)
{
    List snakeList = getSnakeList();
    int *score = getScore();
    CGPoint3i *snakeTail = getAt(snakeList, (*score) - 2);
    glPushMatrix();
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glRotatef((*snakeTail)[2], 0.0f, 0.0f, 1.0f);
        drawUnitIsoscelesTriangle();
    }
    glPopMatrix();
}

/**
 * Zeichenfunktion die das Essen fuer Snake zeichnet
 * Transformiert das gleichseitige Einheitsdreieck
 */
void drawFood(void)
{
    CGPoint2i *foodCoord = getFoodCoord();
    float *scale = getScale();
    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 0.0f);
        glTranslatef((FIELDBORDER) + ((*foodCoord)[0] * QUAD_SIDE), (FIELDBORDER - 0.02f) + ((*foodCoord)[1] * QUAD_SIDE), 0.0f);
        glScalef(*scale, *scale, 1);
        drawUnitEquilateralTriangle();
    }
    glPopMatrix();
}

/**
 * Zeichenfunktion der Gitterstruktur
 * Transformiert die Einheitslinien zu einer Gitterstruktur
 */
void drawGridLines(void)
{
    for (int y = 0; y < 2; y++)
    {
        for (int x = 0; x <= FIELDSIZE; x++)
        {
            glPushMatrix();
            {
                glColor3f(0.0f, 0.0f, 0.0f);
                if (y != 0)
                {
                    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
                }
                glTranslatef(0.0f, ((-FIELDBORDER) - QUAD_SIDE / 2) - (x * QUAD_SIDE), 0.0f);
                glScalef(1.81f, 1, 1);
                glLineWidth(3);
                drawUnitLine();
            }
            glPopMatrix();
        }
    }
}


/**
 * setzt FT_EXPLODE auf dem Spielfeld
 * @param level das Spielfeld auf das FT_EXPLODE gesetzt wird
 */
void explodeOnField(esLevel *level)
{
    CGPoint2i *bombCoord = getBombCoord();
    esFieldType fieldType = FT_FREE;
    int countY = 0;
    int countX = 0;

    (*level)[(*bombCoord)[1] + countY][(*bombCoord)[0] + countX] = FT_EXPLODED;

    for (int i = 0; i < 12; i++)
    {
        switch (i / 3)
        {
        // i == 0,1,2
        case 0:
            countX = (((*bombCoord)[0] + countX + 1) >= FIELDSIZE) ? countX - FIELDSIZE + 1 : countX + 1;
            break;
        // i == 3,4,5
        case 1:
            countX = (i % 3 == 0) ? -1 : countX - 1;
            countX = ((*bombCoord)[0] + countX < 0) ? countX + FIELDSIZE : countX;
            break;
        // i == 6,7,8
        case 2:
            countX = 0;
            countY = (((*bombCoord)[1] + countY + 1) >= FIELDSIZE) ? countY - FIELDSIZE + 1 : countY + 1;
            break;
        // i == 9,10,11
        case 3:
            countY = (i % 3 == 0) ? -1 : countY - 1;
            countY = ((*bombCoord)[1] + countY < 0) ? countY + FIELDSIZE : countY;
            break;
        }

        fieldType = (*level)[(*bombCoord)[1] + countY][(*bombCoord)[0] + countX];

        if (fieldType == FT_FREE && !damagedSnake((*bombCoord)[1] + countY, (*bombCoord)[0] + countX))
        {
            (*level)[(*bombCoord)[1] + countY][(*bombCoord)[0] + countX] = FT_EXPLODED;
        }
        else
        {
            // Ueberspringen der restlichen Explosionsfelder in einer Richtung
            i += 3 - (i % 3) - 1;
        }
    }
}

/**
 * Setzt alle Explosionsfelder wieder zurueck auf freie Felder
 */
void deleteExplode(void)
{
    esLevel *level = switchLevel();

    for (int y = 0; y < FIELDSIZE; y++)
    {
        for (int x = 0; x < FIELDSIZE; x++)
        {
            if ((*level)[y][x] == FT_EXPLODED)
            {
                (*level)[y][x] = FT_FREE;
            }
        }
    }
    explodingTimer = SCALING;
}

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar mit Waenden, Bloecken, Schlange, Essen, Bombe und Explosionen.
 */
void drawScene2D(void)
{
    int *score = getScore();

    // Soll nur gezeichnet werden, wenn eine Schlange vorhanden ist.
    if (*score != 0)
    {
        int *toggle = getToggle();

        esFieldType fieldType = FT_FREE;

        List snakeList = getSnakeList();
        GLuint *wallListId = getDisplayList(0);
        GLuint *explodeListId = getDisplayList(1);
        GLuint *bodyListId = getDisplayList(2);

        // Die Wahl des Levels wird abgespeichert
        esLevel *level = switchLevel();

        drawGridLines();

        // Ablaufen des Level-Arrays
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
                        glTranslatef(FIELDBORDER + x * QUAD_SIDE, FIELDBORDER + y * QUAD_SIDE, 0.0f);
                        glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
                        glCallList(*wallListId);
                    }
                    glPopMatrix();
                    break;
                // Zeichnen der Explosion
                case FT_EXPLODED:
                    glPushMatrix();
                    {
                        glTranslatef(FIELDBORDER + x * QUAD_SIDE, FIELDBORDER + y * QUAD_SIDE, 0.0f);
                        glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
                        glCallList(*explodeListId);
                    }
                    glPopMatrix();
                    break;
                default:
                    break;
                }
            }
        }

        int *pause = getPause();

        float *scale = getScale();

        if (!isEmpty(snakeList))
        {
            fieldType = (*level)[snakeList->snakePart[1]][(snakeList->snakePart)[0]];
        }

        if (fieldType == FT_FREE)
        {
            glPushMatrix();
            {
                glTranslatef(FIELDBORDER + snakeList->snakePart[0] * QUAD_SIDE, FIELDBORDER + snakeList->snakePart[1] * QUAD_SIDE, 0.0f);
                glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
                drawSnakeHead();
            }
            glPopMatrix();
            List run = snakeList;
            for (int i = 1; i < (*score); i++)
            {
                run = run->next;
                if (run->next == NULL)
                {
                    glPushMatrix();
                    {
                        // Schwanzspitze verschieben
                        glTranslatef(FIELDBORDER + (run->snakePart[0]) * QUAD_SIDE, FIELDBORDER + (run->snakePart[1]) * QUAD_SIDE, 0.0f);
                        // Schwanzspitze skalieren
                        glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
                        drawTail();
                    }
                    glPopMatrix();
                }
                else
                {
                    glPushMatrix();
                    {
                        // Body verschieben
                        glTranslatef(FIELDBORDER + (run->snakePart[0]) * QUAD_SIDE, FIELDBORDER + (run->snakePart[1]) * QUAD_SIDE, 0.0f);
                        // Body groesser skalieren
                        glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
                        // Body in DisplayList zeichnen
                        glCallList(*bodyListId);
                    }
                    glPopMatrix();
                }
            }

            if (*toggle == 0)
            {
                // Zeichnen des Essens
                if (!*pause)
                {
                    calcScaleFood((float)scaling / timer);
                    explodingTimer--;
                }

                drawFood();

                if (explodingTimer <= 0)
                {
                    deleteExplode();
                }

                if (*scale <= 0)
                {
                    calcRandomPosition(*toggle);
                    *toggle = 1;
                }
            }
            else
            {
                // Zeichnen der Bombe
                if (!*pause)
                {
                    calcScaleBomb((float)scaling / timer);
                }

                drawScaledBomb();

                if (*scale >= QUAD_SIDE / 2)
                {
                    explodeOnField(level);
                    calcRandomPosition(*toggle);
                    *toggle = 0;
                }
            }

            // Zeichnen freier Felder
            drawFree();
        }
        else
        {
            // Verloren falls das Feld nicht frei war
            *score = 0;
        }
    }

    /* DEBUG-Ausgabe */
    // GLSTATE;
}

/**
 * löscht alle Displaylisten
 */
void deleteDisplayLists(void)
{
    GLuint *wallListId = getDisplayList(0);
    glDeleteLists(*wallListId, 3);
}

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode(void)
{
    /* Flag: Wireframe: ja/nein */
    static GLboolean wireframe = GL_FALSE;

    /* Modus wechseln */
    wireframe = !wireframe;

    if (wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
