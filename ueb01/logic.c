/**
 * @file
 * Logik-Modul.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung des Schlaegers und des Balls.
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

/* ---- Konstanten ---- */
/** Geschwindigkeit (Schritte pro Sekunde) */
#define RACKET_STEPS_PS 3.0f
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ---- Globale Daten ---- */
/** der Mittelpunkt des Quadrats */
static CGPoint2f g_racket = {0.0f, 0.0f};
static CGPoint2f g_quadCenter = {0.0f, 0.0f};
static CGPoint2f g_ballCenter = {0.0f, 0.0f};
static int g_lifeCount = 3;
static int g_pointCount = 0;
static int g_highPointCount = 0;
static int g_SpeedPushTimes = 0;

/** Geschwindigkeitsvektor des Balls. */
const float startSpeed = 25.0f;
const float speedPush = 5.0f;
static CGVector2f g_ballDirection = {0, 0};

/**
 * Bewegungsstatus der Schlaegers. Fuer alle vier Richtungen wird angegeben, ob
 * sich das Rechteck in die jeweilige Richtung bewegt.
 */
static GLboolean g_movement[4] = {GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE};

/* Dimension der Bloecke */
static float g_bricks[ARRAY_WIDTH][ARRAY_LENGTH][ARRAY_DIMENSIONS];

/* ---- Funktionen ---- */

/**
 * Berechnet neue Position des Schlaegers.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcPositionRacket(double interval)
{
  if (g_lifeCount > 0 && g_highPointCount < ARRAY_WIDTH * ARRAY_LENGTH) {
    if (g_movement[dirLeft])
    {
      g_racket[0] -= RACKET_STEPS_PS * (float)interval;
    }
    if (g_movement[dirRight])
    {
      g_racket[0] += RACKET_STEPS_PS * (float)interval;
    }
  }
}

/**
 * Prueft, ob der Ball mit dem Rahmen oder dem Schläger kollidiert ist.
 * @return Seite, mit der kollidiert wurde.
 */
static CGSide
checkCollision(void)
{
  CGSide
      res = sideNone;

  /* Ball fliegt nach rechts und
     die rechte Seite des Quadrats ueberschreitet den rechten Rand */
  if (g_ballDirection[0] > 0.0f &&
      g_ballCenter[0] * (QUAD_HEIGHT / 4) >= BORDER_RIGHT - QUAD_WIDTH / 2)
  {
    res = sideRight;
  }

  /* Ball fliegt nach links und
     die linke Seite des Quadrats ueberschreitet den linken Rand */
  else if (g_ballDirection[0] < 0.0f &&
           g_ballCenter[0] * (QUAD_HEIGHT / 4) <= BORDER_LEFT + QUAD_WIDTH / 2)
  {
    res = sideLeft;
  }

  /* Ball fliegt nach oben und
     die obere Seite des Quadrats ueberschreitet den oberen Rand */
  else if (g_ballDirection[1] > 0.0f &&
           g_ballCenter[1] * (QUAD_HEIGHT / 4) - BORDER_TOP - QUAD_HEIGHT / 2 > BORDER_TOP - QUAD_HEIGHT * 2)
  {
    res = sideTop;
  }

  /* Ball fliegt nach unten und
     die untere Seite des Balls ueberschreitet den Rand des Schlaegers*/
  else if (g_ballDirection[1] < 0.0f &&
           g_ballCenter[1] / (QUAD_HEIGHT / 4) <= BORDER_BOTTOM + QUAD_WIDTH)
  {
    CGPoint2f *posRacket = getRacket();
    if (g_ballCenter[0] * (QUAD_HEIGHT / 4) >= (*posRacket)[0] * (QUAD_WIDTH * 2) - QUAD_WIDTH &&
        g_ballCenter[0] * (QUAD_HEIGHT / 4) <= (*posRacket)[0] * (QUAD_WIDTH * 2) + QUAD_WIDTH &&
        g_ballCenter[1] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM <= (*posRacket)[1] * (QUAD_HEIGHT) + BORDER_BOTTOM + QUAD_HEIGHT &&
        g_ballCenter[1] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM >= (*posRacket)[1] * (QUAD_HEIGHT) + BORDER_BOTTOM - QUAD_HEIGHT)
    {
      float newAngle = ((*posRacket)[0] * (QUAD_WIDTH * 2) - g_ballCenter[0] * (QUAD_HEIGHT / 4)) * 3 * 100 + 90;
      calcRandomStartDirection(newAngle);
    }
    else
    {
      res = sideNone;
      
      if (g_ballCenter[1] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM < -1.2)
      {
        g_lifeCount--;
        if (g_lifeCount == 0) {
          g_ballDirection[0] = 0;
          g_ballDirection[1] = 0;
          fprintf(stdout, "Punkte: %i   Leben: %i\n", g_highPointCount, g_lifeCount);
          fprintf(stdout, "YOU LOSE!\n");
        } else {
          g_ballCenter[0] = (*posRacket)[0] * (QUAD_WIDTH * 107);
          g_ballCenter[1] = (*posRacket)[1] + (QUAD_HEIGHT);
          g_SpeedPushTimes = 0;
          g_pointCount = 0;
          calcRandomStartDirection(0.0);
        }
      }
    }
  }

  return res;
}

/**
 * Prueft, ob der Ball mit den Blöcken kollidiert ist.
 * @return Seite, mit der kollidiert wurde.
 */
static CGSide
checkBrickCollision(void)
{
  CGSide
      res = sideNone;

  for(int x = 0; x < ARRAY_WIDTH; x++){
    for(int y = 0; y < ARRAY_LENGTH; y++){
      if (g_bricks[x][y][3] == 0.0){
        /* Ball fliegt nach rechts und
           das Center des Balls ueberschreitet den linken Rand */
        if (g_ballDirection[0] > 0.0f &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) >= g_bricks[x][y][4] - QUAD_WIDTH / 2 &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) <= g_bricks[x][y][4] + QUAD_WIDTH / 2 &&
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 >= g_bricks[x][y][5] - QUAD_HEIGHT / 2 && 
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 <= g_bricks[x][y][5] + QUAD_HEIGHT / 2)
          {
            g_pointCount++;
            g_highPointCount++;        
            g_bricks[x][y][3] = 1.0;   
            res = sideRight;
          } 
          /* Ball fliegt nach links und
           das Center des Balls ueberschreitet den rechten Rand */
        else if (g_ballDirection[0] < 0.0f &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) - (QUAD_HEIGHT / 4 / 2) >= g_bricks[x][y][4] - QUAD_WIDTH / 2 &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) - (QUAD_HEIGHT / 4 / 2) <= g_bricks[x][y][4] + QUAD_WIDTH / 2 &&
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 >= g_bricks[x][y][5] - QUAD_HEIGHT / 2 && 
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 <= g_bricks[x][y][5] + QUAD_HEIGHT / 2)
          {
            g_pointCount++;
            g_highPointCount++; 
            g_bricks[x][y][3] = 1.0;   
            res = sideLeft;
          }
          /* Ball fliegt nach oben und
           das Center des Balls ueberschreitet den unteren Rand */
        else if (g_ballDirection[1] > 0.0f &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) >= g_bricks[x][y][4] - QUAD_WIDTH / 2 &&
            g_ballCenter[0] * (QUAD_HEIGHT / 4) <= g_bricks[x][y][4] + QUAD_WIDTH / 2 &&
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 >= g_bricks[x][y][5] - QUAD_HEIGHT / 2 && 
            g_ballCenter[1] * (QUAD_HEIGHT / 4) + (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 <= g_bricks[x][y][5] + QUAD_HEIGHT / 2)
          {
            g_pointCount++;
            g_highPointCount++; 
            g_bricks[x][y][3] = 1.0;   
            res = sideTop;
          }
          /* Ball fliegt nach unten und
           das Center des Balls ueberschreitet den oberen Rand */
        else if (g_ballDirection[1] < 0.0f &&
          g_ballCenter[0] * (QUAD_HEIGHT / 4) >= g_bricks[x][y][4] - QUAD_WIDTH / 2 &&
          g_ballCenter[0] * (QUAD_HEIGHT / 4) <= g_bricks[x][y][4] + QUAD_WIDTH / 2 &&
          g_ballCenter[1] * (QUAD_HEIGHT / 4) - (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 >= g_bricks[x][y][5] - QUAD_HEIGHT / 2 && 
          g_ballCenter[1] * (QUAD_HEIGHT / 4) - (QUAD_HEIGHT / 4 / 2) + BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 <= g_bricks[x][y][5] + QUAD_HEIGHT / 2)
        {
            g_pointCount++;
            g_highPointCount++; 
            g_bricks[x][y][3] = 1.0;   
            res = sideBottom;
        }
      }
    }
  }
  return res;
}

/**
 * Reagiert auf Kollisionen des Balls mit dem Rahmen.
 * @param side Rahmenseite, mit der kollidiert wurde.
 */
static void
handleCollision(CGSide side)
{
  /* Bewegung in X-Richtung umkehren */
  if (side == sideLeft || side == sideRight)
  {
    g_ballDirection[0] *= -1;
  }

  /* Bewegung in Y-Richtung umkehren */
  if (side == sideTop || side == sideBottom)
  {
    g_ballDirection[1] *= -1;
  }
}
/**
 * Berechnet die neue Position des Balls.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcPositionBall(double interval)
{
  CGSide
      side = 0;

  side = checkCollision();

  if (side != 0)
  {
    handleCollision(side);
  } else {
    side = checkBrickCollision();
    if (side != 0)
    {
      handleCollision(side);
      if(g_pointCount % 10 == 0){
        g_SpeedPushTimes++;
        if(g_ballDirection[0] < 0) {
          g_ballDirection[0] = g_ballDirection[0] - speedPush;
        } else {
          g_ballDirection[0] = g_ballDirection[0] + speedPush;
        }
        if(g_ballDirection[1] < 0) {
          g_ballDirection[1] = g_ballDirection[1] - speedPush;
        } else {
          g_ballDirection[1] = g_ballDirection[1] + speedPush;
        }
      }
      if(g_pointCount % 20 == 0){
        g_lifeCount++;
      }
      fprintf(stdout, "Punkte: %i   Leben: %i\n", g_highPointCount, g_lifeCount);
      if (g_highPointCount >= ARRAY_WIDTH * ARRAY_LENGTH) {
        g_ballDirection[0] = 0;
        g_ballDirection[1] = 0;
        fprintf(stdout, "\nPunkte: %i   Leben: %i\n", g_highPointCount, g_lifeCount);
        fprintf(stdout, "\nYOU WIN!!!!!!!!!!\n\n");
      }
      
    }
  }

  g_ballCenter[0] += g_ballDirection[0] * (float)interval;
  g_ballCenter[1] += g_ballDirection[1] * (float)interval;
}

/**
 * Setzt die Bewegung des Balls.
 * @param angle Der Winkel, in dem der Ball startet.
 */
void
calcRandomStartDirection(float angle)
{  
  if (angle == 0.0) {
    /* Intialisiert den Zufallsnummerngenerator */
    srand((unsigned) time(0));
    angle = ((rand() % 91) + 45);
  }
  g_ballDirection[0] = (startSpeed + speedPush * g_SpeedPushTimes) * cos(angle * M_PI / 180);
  g_ballDirection[1] = (startSpeed + speedPush * g_SpeedPushTimes) * sin(angle * M_PI / 180);
}

/**
 * Setzt den Bewegunsstatus des Schlaeger.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Schlaeger bewegt sich in Richtung
 * direction, GL_FALSE->Schlaeger bewegt sich nicht in Richtung direction.
 */
void setMovement(CGDirection direction, GLboolean status)
{
    if (direction <= dirDown)
    {  
      g_movement[direction] = status;
    }
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) aller Bloecke und Waende.
 * @return Postion (des Mittelpunktes) der Bloecke und Waende.
 */
CGPoint2f *
getQuadCenter(void)
{
  return &g_quadCenter;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Schlaegers.
 * @return Postion (des Mittelpunktes) des Schlaegers.
 */
CGPoint2f *
getRacket(void)
{
  return &g_racket;
}

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Balls.
 * @return Postion (des Mittelpunktes) des Balls.
 */
CGPoint2f *
getBallCenter(void)
{
  return &g_ballCenter;
}

/**
 * Gibt die Dimensionen eines Bricks wieder.
 * @param x die x-Koordinate.
 * @param y die y-Koordinate.
*/
float *
getDimensions (int x, int y) { 
  return g_bricks[x][y];
}

/**
 * Gibt den Bewegungsvektor zurück.
*/
CGPoint2f *
getBallSpeed (void) { 
  return &g_ballDirection;
}

/**
 * Gibt die Anzahl an Leben zurück.
*/
int *
getLifeCount (void) { 
  return &g_lifeCount;
}

/**
 * Gibt die Anzahl an Leben zurück.
*/
int *
getPointCount (void) { 
  return &g_pointCount;
}