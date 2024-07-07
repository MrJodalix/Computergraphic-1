/**
 * @file
 * Schlangenkoerper-Modul.
 * Das Modul kapselt die Listenlogik. 
 * Dieses Modul beinhaltet die Operationen zur Verwaltung der Schlangenliste.
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

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/* ---- Eigene Header einbinden ---- */
#include "unitObjekt3D.h"

/**
 * Zeichen-Funktion zur Erstellung einer Einheitslinie.
 */
void drawUnitNormalLine(float x, float y, float z)
{
    glBegin(GL_LINES);
    {
        glVertex3d(0, 0, 0);
        glVertex3d(x, y, z);
    }
    glEnd();
}

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats in 3D
 */
void drawUnitSquare3D(int drawNorm, int subDivs)
{
    CGPoint3f *quadCenter = getQuadCenter3D();
    int x, y;

    for (y = 0; y < subDivs + 1; y++)
    {
        glBegin(GL_QUAD_STRIP);
        glNormal3f(0.0f, 0.0f, 1.0f);
        for (x = 0; x <= subDivs + 1; x++)
        {
            glVertex3f((*quadCenter)[0] - 0.5f + x / (subDivs + 1.0f),
                       (*quadCenter)[1] + 0.5f - y / (subDivs + 1.0f),
                       (*quadCenter)[2]);

            glVertex3f((*quadCenter)[0] - 0.5f + x / (subDivs + 1.0f),
                       (*quadCenter)[1] + 0.5f - (y + 1) / (subDivs + 1.0f),
                       (*quadCenter)[2]);
        }
        glEnd();
        if (drawNorm)
        {
            drawUnitNormalLine(0.0f, 0.0f, 1.0f);
        }
    }
}

/**
 * Zeichen-Funktion zur Erstellung der Einheitskugel.
 */
void drawUnitSphere(void)
{
    GLUquadricObj *unitCircle = 0;
    unitCircle = gluNewQuadric();
    gluQuadricNormals(unitCircle, 1);
    gluSphere(unitCircle, 1, 32, 32);
    gluDeleteQuadric(unitCircle);
}

/**
 * Zeichen-Funktion fuer die gleichschenkligen Einheitsdreiecke der Pyramide
 */
void drawUnitPyramidTriangle(void)
{
    CGPoint3f *triangleCenter = getTriangleCenter3D();
    int *drawNorm = getDrawNorm();
    glBegin(GL_TRIANGLES);
    {
        glNormal3f(0.196f, 0.588f, 0.784f);
        glVertex3f((*triangleCenter)[0] + 0.5f, (*triangleCenter)[1] - 0.5, (*triangleCenter)[2] + 0.5);
        glVertex3f((*triangleCenter)[0], (*triangleCenter)[1] + 0.5f, (*triangleCenter)[2]);
        glVertex3f((*triangleCenter)[0] - 0.5f, (*triangleCenter)[1] - 0.5, (*triangleCenter)[2] + 0.5);
    }
    glEnd();
    if (*drawNorm)
    {
        drawUnitNormalLine(0.196f, 0.588f, 0.784f);
    }
}

/**
 * Zeichenfunktion zur Erstellung der Einheitspyramide
 */
void drawUnitPyramid(void)
{
    int *drawNorm = getDrawNorm();
    // Grundflaeche
    glPushMatrix();
    {
        glRotatef(90, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, 0.5f);
        drawUnitSquare3D((*drawNorm), 10);
    }
    glPopMatrix();
    // 4 Dreiecke
    glPushMatrix();
    {
        drawUnitPyramidTriangle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotated(90.0f, 0.0f, 1.0f, 0.0f);
        drawUnitPyramidTriangle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotated(180.0f, 0.0f, 1.0f, 0.0f);
        drawUnitPyramidTriangle();
    }
    glPopMatrix();
    glPushMatrix();
    {
        glRotated(270.0f, 0.0f, 1.0f, 0.0f);
        drawUnitPyramidTriangle();
    }
    glPopMatrix();
}

/**
 * Zeichenfunktion der Pyramide, die um die x-Achse rotiert wurde (270 Grad)
 */
void drawRotatePyramid(void)
{
    glPushMatrix();
    {
        glRotatef(270.0f, 1, 0, 0);
        drawUnitPyramid();
    }
    glPopMatrix();
}

/**
 * Zeichenfunktion eines Einheits-Tetraeders mit Radius 1
 */
void drawUnitTetrahedron(void)
{
    CGPoint3f *triangleCenter = getTriangleCenter3D();
    int *drawNorm = getDrawNorm();
    glBegin(GL_TRIANGLES);
    {
        // vier gleichseitige Dreiecke

        // vorne (nach links gedreht)
        glNormal3f(-0.669f, 0.446f, 0.595f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);

        // unten
        glNormal3f(0.48, -0.685f, 0.548f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);

        // rechts
        glNormal3f(0.595, 0.669f, -0.446);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);

        // hinten
        glNormal3f(-0.597, -0.697f, -0.398f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);
    }
    glEnd();
    if (*drawNorm)
    {
        drawUnitNormalLine(-0.669f, 0.446f, 0.595f);
        drawUnitNormalLine(0.48, -0.685f, 0.548f);
        drawUnitNormalLine(0.595, 0.669f, -0.446);
        drawUnitNormalLine(-0.597, -0.697f, -0.398f);
    }
}

/**
 * Zeichnet ein invertiertes Tetraeder
*/
void drawUnitTetrahedronInverse(void)
{
    CGPoint3f *triangleCenter = getTriangleCenter3D();
    int *drawNorm = getDrawNorm();
    glBegin(GL_TRIANGLES);
    {
        // vorne (nach links gedreht)
        glNormal3f(0.669f, -0.446f, -0.595f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);

        // unten
        glNormal3f(-0.48, 0.685f, -0.548f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);

        // rechts
        glNormal3f(-0.595, -0.669f, 0.446);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] + 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);

        // hinten
        glNormal3f(0.597, 0.697f, 0.398f);
        glVertex3f((*triangleCenter)[0] + 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] + 1.0f, (*triangleCenter)[2] - 1.0f);
        glVertex3f((*triangleCenter)[0] - 1.0f, (*triangleCenter)[1] - 1.0f, (*triangleCenter)[2] + 1.0f);
    }
    glEnd();
    if (*drawNorm)
    {
        drawUnitNormalLine(0.669f, -0.446f, -0.595f);
        drawUnitNormalLine(-0.48, 0.685f, -0.548f);
        drawUnitNormalLine(-0.595, -0.669f, 0.446);
        drawUnitNormalLine(0.597, 0.697f, 0.398f);
    }
}