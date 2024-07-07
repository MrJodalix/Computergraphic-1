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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* ---- Eigene Header einbinden ---- */
#include "logic.h"

/* ---- Globale Daten ---- */
/** der Mittelpunkt des Quadrats */
static CGPoint2f g_quadCenter = {0.0f, 0.0f};
static CGPoint3i g_snakeHeadCenter = {STARTPOSX, STARTPOSY, STARTROT};
static CGPoint2f g_lineCenter = {0.0f, 0.0f};
static CGPoint2f g_triangleCenter = {0.0f, 0.0f};
static CGPoint2i g_foodCoordinate = {0, 0};
static CGPoint2i g_bombCoordinate = {0, 0};
static float g_scale = QUAD_SIDE / 2;

static int g_score = SCORE;
static int g_highScore = SCORE;
static int g_currentLevel = 1;

static int helpToggle = 0;
static int helpScore = 0;
static int pause = 0;

List g_snakeList = NULL;

/** Fuer 3D Rendering */
static CGPoint3f g_quadCenter3D = {0.0f, 0.0f, 0.0f};
static CGPoint3f g_lineCenter3D = {0.0f, 0.0f, 0.0f};
static CGPoint3f g_triangleCenter3D = {0.0f, 0.0f, 0.0f};
static CGPoint3i g_bombCoordinate3D = {0, 0, 0};
static CGPoint3i g_foodCoordinate3D = {0, 0, 0};

int drawNorm = 0;
int mainLight = 0;
int spotlight = 0;
int animated = 0;

/**
 * Bewegungsstatus der Schlange. Fuer alle vier Richtungen wird angegeben, ob
 * sich das Rechteck in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[4] = {GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE};

/* ---- Funktionen ---- */

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
  helpToggle++;

  if (helpToggle % 2 == 1)
  {
    helpScore = g_score;
    g_score = 0;
  }
  else
  {
    g_score = helpScore;
    helpScore = 0;
  }
}

/**
 * Ueberprueft ob die Schlange getroffen wurde und reduziert die Liste bei einem Treffer
 * @param y die y-Koordinate vom Treffer
 * @param x die x-Koordinate vom Treffer
 * @return wurde getroffen oder nicht
 */
int damagedSnake(unsigned int y, unsigned int x)
{
  List run = g_snakeList;
  unsigned int listX = 0;
  unsigned int listY = 0;
  unsigned int lengthSnake = length(g_snakeList);
  unsigned int i = 0;
  int damaged = 0;

  while (!isEmpty(g_snakeList) && i < lengthSnake)
  {
    listX = (*head(run))[0];
    listY = (*head(run))[1];
    if (x == listX && y == listY)
    {
      for (unsigned int j = 0; j < lengthSnake - i; j++)
      {
        g_snakeList = removeAt(g_snakeList, i);
      }
      g_score = length(g_snakeList);
      i = lengthSnake;
      damaged = 1;
    }
    else
    {
      run = run->next;
    }
    i++;
  }
  if(isEmpty(g_snakeList)){
    damaged = 1;
  }
  return damaged;
}

/**
 * Prueft auf Kollision des Schlangenkopfes mit dem Futter
 * @return ob das Food getroffen wurde oder nicht
 */
int checkFoodCollision(void)
{
  CGPoint3i newPart;
  int result = 1;
  if (g_foodCoordinate[0] == g_snakeList->snakePart[0] &&
      g_foodCoordinate[1] == g_snakeList->snakePart[1])
  {
    newPart[0] = g_foodCoordinate[0];
    newPart[1] = g_foodCoordinate[1];
    newPart[2] = g_snakeList->snakePart[2];
    g_score++;
    if (g_highScore < g_score)
    {
      g_highScore = g_score;
    }
    g_snakeList = insertAt(g_snakeList, 1, newPart);
    result = 0;
    calcRandomPosition(1);
    g_scale = QUAD_SIDE / 2;
  }
  return result;
}

/**
 * Prueft auf Kollision des Schlangenkopfes mit dem eigenen Koerper
 */
void checkBodyCollision(void)
{
  List run = g_snakeList;
  unsigned int snakeLength = length(run);

  for (unsigned int i = 0; i < snakeLength - 1; i++)
  {
    run = run->next;
    if (run->snakePart[0] == g_snakeList->snakePart[0] &&
        run->snakePart[1] == g_snakeList->snakePart[1] &&
        // Schwanzspitze wird ignoriert
        i != snakeLength - 2)
    {
      g_score = 0;
    }
  }
}

/**
 * Berechnet die neue Position des Schlangenkopfs.
 */
void calcPosition(void)
{
  List run = g_snakeList;
  CGPoint3i prevPosition;
  CGPoint3i currentPosition;
  int collision = 1;

  if (run != NULL)
  {
    currentPosition[0] = run->snakePart[0];
    currentPosition[1] = run->snakePart[1];
    currentPosition[2] = run->snakePart[2];
  }

  for (int i = 0; i < g_score; i++)
  {

    prevPosition[0] = currentPosition[0];
    prevPosition[1] = currentPosition[1];
    prevPosition[2] = currentPosition[2];

    if (i == 0)
    {
      collision = checkFoodCollision();
      if (g_movement[dirDown])
      {
        run->snakePart[1] = (run->snakePart[1] == 0) ? FIELDSIZE - 1 : run->snakePart[1] - 1;
        run->snakePart[2] = 0;
      }
      if (g_movement[dirRight])
      {
        run->snakePart[0] = (run->snakePart[0] < FIELDSIZE - 1) ? run->snakePart[0] + 1 : 0;
        run->snakePart[2] = 90;
      }
      if (g_movement[dirUp])
      {
        run->snakePart[1] = (run->snakePart[1] < FIELDSIZE - 1) ? run->snakePart[1] + 1 : 0;
        run->snakePart[2] = 180;
      }
      if (g_movement[dirLeft])
      {
        run->snakePart[0] = (run->snakePart[0] == 0) ? FIELDSIZE - 1 : run->snakePart[0] - 1;
        run->snakePart[2] = 270;
      }
      checkBodyCollision();
    }
    if (g_score != 0 && run->next != NULL && collision)
    {
      currentPosition[0] = run->next->snakePart[0];
      currentPosition[1] = run->next->snakePart[1];
      currentPosition[2] = run->next->snakePart[2];

      run->next->snakePart[0] = prevPosition[0];
      run->next->snakePart[1] = prevPosition[1];
      run->next->snakePart[2] = prevPosition[2];

      run = run->next;
    }
  }
}

/**
 * Berechnet die Skalierung des Essens.
 * @param times die Skalierung ueber Zeit
 */
void calcScaleFood(double times)
{
  g_scale -= times;
}

/**
 * Berechnet die Skalierung der Bombe.
 * @param times die Skalierung ueber Zeit
 */
void calcScaleBomb(double times)
{
  g_scale += times;
}

/**
 * Setzt den Bewegunsstatus der Schlange.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Schlange bewegt sich in Richtung
 * direction, GL_FALSE->Schlange bewegt sich nicht in Richtung direction.
 */
void setMovement(CGDirection direction, GLboolean status)
{
  if (g_score != 1)
  {
    switch (direction)
    {
    case dirLeft:
      if (g_movement[dirRight] == GL_FALSE)
      {
        g_movement[dirUp] = GL_FALSE;
        g_movement[dirDown] = GL_FALSE;
        g_movement[dirLeft] = status;
      }
      break;
    case dirRight:
      if (g_movement[dirLeft] == GL_FALSE)
      {
        g_movement[dirUp] = GL_FALSE;
        g_movement[dirDown] = GL_FALSE;
        g_movement[dirRight] = status;
      }
      break;
    case dirUp:
      if (g_movement[dirDown] == GL_FALSE)
      {
        g_movement[dirLeft] = GL_FALSE;
        g_movement[dirRight] = GL_FALSE;
        g_movement[dirUp] = status;
      }
      break;
    case dirDown:
      if (g_movement[dirUp] == GL_FALSE)
      {
        g_movement[dirLeft] = GL_FALSE;
        g_movement[dirRight] = GL_FALSE;
        g_movement[dirDown] = status;
      }
      break;
    }
  }
  else
  {
    g_movement[dirLeft] = GL_FALSE;
    g_movement[dirRight] = GL_FALSE;
    g_movement[dirUp] = GL_FALSE;
    g_movement[dirDown] = GL_FALSE;

    if (direction <= dirDown)
    {
      g_movement[direction] = status;
    }
  }
}

/**
 * Initialisiert die Liste, die die Schlange abbilden soll.
 */
void initSnakeList(void)
{
  g_score = SCORE;
  g_snakeList = deleteAll(g_snakeList);

  for (int i = 0; i < g_score; i++)
  {
    g_snakeHeadCenter[1] = STARTPOSY + i;

    g_snakeList = insertAt(g_snakeList, i, g_snakeHeadCenter);
  }
  g_snakeHeadCenter[1] -= g_score - 1;
}

/**
 * Bestimmt an Hand der rand() Operation die neue Position des Foods bzw. der Bomb
 * @param toggle bestimmt ob es sich um Food oder Bomb handelt
 */
void calcRandomPosition(int toggle)
{
  int x = -1;
  int y = -1;

  if (!toggle)
  {
    g_foodCoordinate[0] = x;
    g_foodCoordinate[1] = y;
  }
  else
  {
    esLevel *level = getLevel(g_currentLevel);

    esFieldType fieldType = FT_WALL;

    List run = g_snakeList;

    unsigned int lengthList = length(run);

    int listX = -1;
    int listY = -1;
    unsigned int i = 0;

    /* Intialisiert den Zufallsnummerngenerator */
    srand((unsigned)time(0));
    do
    {
      x = (rand() % FIELDSIZE);
      y = (rand() % FIELDSIZE);
      fieldType = (*level)[y][x];
      for (unsigned int i = 0; i < lengthList; i++)
      {
        listX = (*head(run))[0];
        listY = (*head(run))[1];
        if (run->next != NULL)
        {
          run = run->next;
        }
        if (x == listX && y == listY)
        {
          i += lengthList + lengthList;
        }
      }
    } while (fieldType != FT_FREE && i != lengthList - 1);

    g_foodCoordinate[0] = x;
    g_foodCoordinate3D[0] = g_foodCoordinate[0];

    g_foodCoordinate[1] = y;
    g_foodCoordinate3D[2] = g_foodCoordinate[1];

    g_bombCoordinate[0] = g_foodCoordinate[0];
    g_bombCoordinate3D[0] = g_foodCoordinate[0];

    g_bombCoordinate[1] = g_foodCoordinate[1];
    g_bombCoordinate3D[2] = g_foodCoordinate[1];
  }
}

/**
 * Löscht die Liste der Schlange (für IO)
 */
void deleteSnakeList(void)
{
  g_snakeList = deleteAll(g_snakeList);
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) aller Waende.
 * @return Postion (des Mittelpunktes) der Waende.
 */
CGPoint2f *
getQuadCenter(void)
{
  return &g_quadCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) aller Waende.
 * @return Postion (des Mittelpunktes) der Waende.
 */
CGPoint3f *
getQuadCenter3D(void)
{
  return &g_quadCenter3D;
}

/**
 * Liefert aktuelle Postion im Level-Array des Schlangenkopfes.
 * @return Postion des Schlangenkopfes.
 */
CGPoint3i *
getSnakeHeadCenter(void)
{
  return &g_snakeHeadCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) der Linie.
 * @return Postion (des Mittelpunktes) der Linie.
 */
CGPoint2f *
getLineCenter(void)
{
  return &g_lineCenter;
}
/**
 * Liefert aktuelle Postion (des Mittelpunktes) der Linie.
 * @return Postion (des Mittelpunktes) der Linie.
 */
CGPoint3f *
getLineCenter3D(void)
{
  return &g_lineCenter3D;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Dreiecks.
 * @return Postion (des Mittelpunktes) des Dreiecks.
 */
CGPoint2f *
getTriangleCenter(void)
{
  return &g_triangleCenter;
}
/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Dreiecks.
 * @return Postion (des Mittelpunktes) des Dreiecks.
 */
CGPoint3f *
getTriangleCenter3D(void)
{
  return &g_triangleCenter3D;
}

/**
 * Liefert aktuelle Postion des Foods im Level-Array.
 * @return Postion des Foods.
 */
CGPoint2i *
getFoodCoord(void)
{
  return &g_foodCoordinate;
}

/**
 * Liefert aktuelle Postion des Foods im Level-Array im 3D Raum.
 * @return Postion des Foods.
 */
CGPoint3i *
getFoodCoord3D(void)
{
  return &g_foodCoordinate3D;
}

/**
 * Liefert aktuelle Postion im Level-Array der Bombe.
 * @return Postion der Bombe.
 */
CGPoint2i *
getBombCoord(void)
{
  return &g_bombCoordinate;
}

/**
 * Liefert aktuelle Postion im Level-Array der Bombe im 3D Raum.
 * @return Postion der Bombe.
 */
CGPoint3i *
getBombCoord3D(void)
{
  return &g_bombCoordinate3D;
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getPause(void)
{
  return &pause;
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getDrawNorm(void)
{
  return &drawNorm;
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getDrawMainLight(void)
{
  return &mainLight;
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getDrawSpotlight(void)
{
  return &spotlight;
}

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getAnimated(void)
{
  return &animated;
}

/**
 * Liefert die Skalierung fuer das Essen und die Bombe
 */
float *
getScale(void)
{
  return &g_scale;
}

/**
 * Liefert die Liste der Schlange
 */
List getSnakeList(void)
{
  return g_snakeList;
}

/**
 * Gibt die derzeitige Anzahl an Koerperteilen zurück.
 */
int *getScore(void)
{
  return &g_score;
}

/**
 * Liefert den temporaeren Hilfsscore waehrend der Hilfeausgabe
 */
int *getHelpScore(void)
{
  return &helpScore;
}

/**
 * Gibt den HighScore an Koerperteilen zurück.
 */
int *getHighScore(void)
{
  return &g_highScore;
}

/**
 * Liefert, den Index des aktuellen Levels.
 * @return den Index des aktuellen Levels.
 */
int *getCurrentLevel(void)
{
  return &g_currentLevel;
}