/**
 * @file
 * Logik-Modul.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung der Objekte im Spiel.
 * Die Programmlogik ist weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* ---- Eigene Header einbinden ---- */
#include "logic.h"
#include "scene.h"

/* ---- Globale Daten ---- */
static int numberOfVertices = VERTICE_START;

static int helpToggle = 0;
static int sphereToggle = 1;
static int pause = 0;
static int drawNorm = 0;

GLuint G_VBO;

/**
 * Bewegungsstatus. Fuer alle vier Richtungen wird angegeben, ob
 * sich das Rechteck in die jeweilige Richtung bewegt.
 */
// static GLboolean g_movement[4] = {GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE};

/* ---- Funktionen ---- */

/**
 * Initialisiert die dynamischen Arrays
 */
void initVertices(void)
{
  int *numberOfVertices = getNumberOfVertices();

  Vertices = malloc(sizeof(Vertex) * (*numberOfVertices));
  Indices = malloc(sizeof(Indices) * COUNT_OF_INDICES);
  Normals = malloc(sizeof(Normal) * (*numberOfVertices + 1) * 3);
}

/**
 * Leert alle Arrays und setzt alle dynamischen Arrays auf NULL
 */
void freeAll(void)
{
  free(Vertices);
  Vertices = NULL;
  free(Indices);
  Indices = NULL;
  free(Normals);
  Normals = NULL;
}

/**
 * Setzt die Mesh-Koordinaten, Farbwerte und Texture-Koordinaten
 * @param count  der Index im Array
 * @param coordX die berechnete X-Koordinate des Meshs
 * @param coordY die berechnete Y-Koordinate des Meshs
 * @param coordZ die berechnete Z-Koordinate des Meshs
 */
void setVerticeCoords(int count, double coordX, double coordY, double coordZ)
{
  Vertices[count][CX] = coordX;
  Vertices[count][CY] = coordY;
  Vertices[count][CZ] = coordZ;
  Vertices[count][CR] = 1.0;
  Vertices[count][CG] = 1.0;
  Vertices[count][CB] = 1.0;
  //numberOfVertices = 25   z.B. (7 / 5) / (25 / (5-1))   =>   1 / 6
  Vertices[count][CS] = ((count / (int)sqrt(numberOfVertices)) / ((numberOfVertices / sqrt(numberOfVertices)) - 1));
  //numberOfVertices = 25   z.B. (7 % 5) / (25 / (5-1))   =>   2 / 6
  Vertices[count][CT] = ((count % (int)(sqrt(numberOfVertices))) / ((numberOfVertices / sqrt(numberOfVertices)) - 1));
}

/**
 * Erstellt die Normalen der Vertex-Koordinaten
 * @param count der Index des Normal-Arrays
 * @param x die X-Koordinate des Meshs
 * @param y die Y-Koordinate des Meshs
 * @param z die Z-Koordinate des Meshs
 */
void calcNormalCoords(int count, double x, double y, double z)
{
  double xLeft = x - ((abs(XSTART) + XEND) / (sqrt(numberOfVertices) - 1));
  double xRight = x + ((abs(XSTART) + XEND) / (sqrt(numberOfVertices) - 1));
  double zTop = z + ((abs(ZSTART) + ZEND) / (sqrt(numberOfVertices) - 1));
  double zDown = z - ((abs(ZSTART) + ZEND) / (sqrt(numberOfVertices) - 1));
  // x-Vektor
  double xVec1 = xRight - xLeft;
  double yVec1;
  double zVec1 = z - z;
  // z-Vektor
  double xVec2 = x - x;
  double yVec2;
  double zVec2 = zTop - zDown;
  if(count < sqrt(numberOfVertices) || 
     count > (sqrt(numberOfVertices) * (sqrt(numberOfVertices) - 1)) ||
     (count % (int)sqrt(numberOfVertices)) == 0 ||
      count % (int)(sqrt(numberOfVertices)) == 1){
    yVec1 = y - y;
    yVec2 = y - y;
  } else {
    yVec1 = Vertices[count - 2][CY] - Vertices[count][CY];
    yVec2 = Vertices[count - (int)sqrt(numberOfVertices)][CY] - Vertices[count + (int)sqrt(numberOfVertices)][CY];
  }
  // Kreuzprodukt
  double xVec3 = yVec1 * zVec2 - yVec2 * zVec1;
  double yVec3 = (zVec1 * xVec2 - zVec2 * xVec1) * -1;
  double zVec3 = xVec1 * yVec2 - xVec2 * yVec1;
  // Normierung
  double length = sqrt(pow(xVec3, 2.0) + pow(yVec3, 2.0) + pow(zVec3, 2.0));
  Normals[count][NX] = (xVec3) * (1.0 / length);
  Normals[count][NY] = (yVec3) * (1.0 / length);
  Normals[count][NZ] = (zVec3) * (1.0 / length);
}

/**
 * Führt die Berechnungen der Mesh-Vertices durch und setzt die Indeces
 */
void setWaterPlane(void)
{
  int *numberOfVertices = getNumberOfVertices();
  float x = XSTART;
  float z = ZSTART;
  int count = 0;
  double coordY = 0.0;

  // Speicher für Vertizes reallozieren
  initVertices();


  //Erstellung des Vertices-Array
  for (int j = 0; j < (*numberOfVertices); j += sqrt((*numberOfVertices)))
  {
    for (int i = 0; i < sqrt((*numberOfVertices)); i++)
    {
      setVerticeCoords(i + j, x, coordY, z);

      x += ((abs(XSTART) + XEND) / (sqrt(*numberOfVertices) - 1));
    }
    z += ((abs(ZSTART) + ZEND) / (sqrt(*numberOfVertices) - 1));
    x = XSTART;
  }

  Normals[count][NX] = 0.0;
  Normals[count][NY] = 0.0;
  Normals[count][NZ] = 0.0;

  //Erstellung des Normal-Array
  for (int count = 0; count < (*numberOfVertices); count++)
  {
    calcNormalCoords(count + 1, Vertices[count][CX], Vertices[count][CY], Vertices[count][CZ]);
  }

  //Erstellung des Indices Array
  for (int j = 0; j < (*numberOfVertices) - sqrt(*numberOfVertices); j += sqrt((*numberOfVertices)))
  {
    for (int i = 0; i < (sqrt((*numberOfVertices)) - 1); i++)
    {
      Indices[count++] = i + j;
      Indices[count++] = j + sqrt((*numberOfVertices)) + i;
      Indices[count++] = i + j + 1;
      Indices[count++] = i + j + 1;
      Indices[count++] = j + sqrt((*numberOfVertices)) + i;
      Indices[count++] = i + 1 + sqrt((*numberOfVertices)) + j;
    }
  }
}

/**
 * Initialisiert die Pause (0 oder 1)
 */
void initPause(void)
{
  pause = (pause + 1) % 2;
}

/**
 * Initialisiert die Hilfeausgabe
 */
void initHelp(void)
{
  if (helpToggle % 2 == 1)
  {
    helpToggle = 0;
  }
  else
  {
    helpToggle = 1;
  }
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *getPause(void)
{
  return &pause;
}

/**
 * Getter für die Normalen.
 */
int *getDrawNorm(void)
{
  return &drawNorm;
}

/**
 * Gibt die Anzahl aller Vertices zurueck
 */
int *getNumberOfVertices(void)
{
  return &numberOfVertices;
}

/**
 * gibt den Toggle zu malen der Kugeln zurueck
 */
int *getDrawWaterSpheres(void)
{
  return &sphereToggle;
}

/**
 * gibt den Toggle zu malen der Hilfeausgabe zurueck
 */
int *getDrawHelp(void)
{
  return &helpToggle;
}