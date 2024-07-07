/**
 * @file
 * Darstellungs-Modul.
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

/* ---- Globale Daten ---- */
// Mauer-Displaylist-ID
static GLuint g_wallListID2D = 0;
// Explosions-Displaylist-ID
static GLuint g_explodeListID2D = 0;
// Schlangenkoerper-Displaylist-ID
static GLuint g_bodyListID2D = 0;
// Mauer-Displaylist-ID
static GLuint g_wallListID3D = 0;
// Explosions-Displaylist-ID
static GLuint g_explodeListID3D = 0;
// Schlangenkoerper-Displaylist-ID
static GLuint g_bodyListID3D = 0;
// Mauer-Displaylist-ID mit Normalen
static GLuint g_wallListID3DNorm = 0;
// Schlangenkoerper-Displaylist-ID mit Normalen
static GLuint g_bodyListID3DNorm = 0;
// wechselt zwischen Food und Bomb
int toggle = 0;
// Levelwahl
int choice = 0;
// Das Level
esLevel *level = {FT_FREE};

/**
 * Gibt das Level zurueck, welches ausgewaehlt wurde.
 * @return das ausgewaehlte Level
 */
esLevel *switchLevel(void)
{
  int *currentLevel = getCurrentLevel();

  if (choice != *currentLevel)
  {
    level = getLevel((*currentLevel));

    choice = *currentLevel;
  }
  return level;
}

/**
 * Erzeugen der Display-Listen fuer Waende, Explosionen und Koerperteile
 */
static void
initDisplayLists(void)
{
  /* Sechs Display-Listen anfordern */
  g_wallListID2D = glGenLists((GLsizei)8);
  if (g_wallListID2D != 0)
  {
    /* Erste Diplay-Liste */
    glNewList(g_wallListID2D, GL_COMPILE);
    drawWall();
    glEndList();

    /* Zweite Diplay-Liste */
    g_explodeListID2D = g_wallListID2D + 1;
    glNewList(g_explodeListID2D, GL_COMPILE);
    drawExplosion();
    glEndList();

    /* Dritte Diplay-Liste */
    g_bodyListID2D = g_wallListID2D + 2;
    glNewList(g_bodyListID2D, GL_COMPILE);
    drawBody();
    glEndList();

    /* Vierte Diplay-Liste */
    g_wallListID3D = g_wallListID2D + 3;
    glNewList(g_wallListID3D, GL_COMPILE);
    drawWall3D(0);
    glEndList();

    /* Fuenfte Diplay-Liste */
    g_explodeListID3D = g_wallListID2D + 4;
    glNewList(g_explodeListID3D, GL_COMPILE);
    drawExplosion3D();
    glEndList();

    /* Sechste Diplay-Liste */
    g_bodyListID3D = g_wallListID2D + 5;
    glNewList(g_bodyListID3D, GL_COMPILE);
    drawSnakeCube(0);
    glEndList();

    /* Siebte Diplay-Liste */
    g_wallListID3DNorm = g_wallListID2D + 6;
    glNewList(g_wallListID3DNorm, GL_COMPILE);
    drawWall3D(1);
    glEndList();

    /* Achte Diplay-Liste */
    g_bodyListID3DNorm = g_wallListID2D + 7;
    glNewList(g_bodyListID3DNorm, GL_COMPILE);
    drawSnakeCube(1);
    glEndList();
  }
  else
  {
    CG_ERROR(("Konnte Displaylisten nicht erzeuge\n"));
  }
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrundfarbe.
 * Initialisiert die Displaylisten, die Schlangenliste.
 * loescht Explosionen im Level, falls noch welche vorhanden sind.
 * scale und toggle werden auf ihren Startwert zurueck gesetzt.
 * Die Position fuer das Food wird initialisiert.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void)
{
  float *scale = getScale();

  /* Setzen der Farbattribute */
  /* Hintergrundfarbe */
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  /* Z-Buffer-Test aktivieren */
  glEnable(GL_DEPTH_TEST);

  /* Polygonrueckseiten nicht anzeigen */
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  /* Normalen nach Transformationen wieder auf die
   * Einheitslaenge bringen */
  glEnable(GL_NORMALIZE);

  initLight();

  initDisplayLists();

  initSnakeList();
  
  deleteExplode();

  *scale = QUAD_SIDE / 2;

  toggle = 0;

  calcRandomPosition(1);
  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}

/**
 * Gibt den Toggle fuer Nahrung oder Bombe zurueck.
 * @return der Toggle
 */
int *getToggle(void)
{
  return &toggle;
}

/**
 * Gibt die DisplayListe zurueck
 * @param idx der Index der Liste
 * @return die Displayliste an dem Index
*/
GLuint *
getDisplayList(int idx)
{
  switch (idx)
  {
  case 0:
    return &g_wallListID2D;
    break;
  case 1:
    return &g_explodeListID2D;
    break;
  case 2:
    return &g_bodyListID2D;
    break;
  case 3:
    return &g_wallListID3D;
    break;
  case 4:
    return &g_explodeListID3D;
    break;
  case 5:
    return &g_bodyListID3D;
    break;
  case 6:
    return &g_wallListID3DNorm;
    break;
  default:
    return &g_bodyListID3DNorm;
  }
}