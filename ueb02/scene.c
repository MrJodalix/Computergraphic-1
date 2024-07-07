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
#include "logic.h"
#include "types.h"
#include "debugGL.h"
#include "levels.h"
#include "stringOutput.h"
#include "snake.h"

/* ---- Globale Daten ---- */

// Mauer-Displaylist-ID
static GLuint g_wallListID = 0;
// Explosions-Displaylist-ID
static GLuint g_explodeListID = 0;
// Schlangenkoerper-Displaylist-ID
static GLuint g_bodyListID = 0;
// wechselt zwischen Food und Bomb
int toggle = 0;
// Levelwahl
int choice = 0;
// Skalierungsvariable fuer Food und Bomb
int scaling = SCALING;
// Timervariable fuer die Dauer der Explosion
int explodingTimer = SCALING;
// Timerkonstante fuer die Skalierung
int const timer = 1000000;
// Das Level
esLevel *level = {FT_FREE};

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats
 */
void drawUnitSquare(void)
{
  /* In der Logik berechnete Position beziehen */
  CGPoint2f *quadCenter = getQuadCenter();

  /* Festlegung eines Einheitsobjektes für Quadrate */
  glBegin(GL_QUADS);
  {
    glVertex2d((*quadCenter)[0] + 0.5, (*quadCenter)[1] + 0.5);
    glVertex2d((*quadCenter)[0] - 0.5, (*quadCenter)[1] + 0.5);
    glVertex2d((*quadCenter)[0] - 0.5, (*quadCenter)[1] - 0.5);
    glVertex2d((*quadCenter)[0] + 0.5, (*quadCenter)[1] - 0.5);
  }
  glEnd();
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
    glVertex2d((*triangleCenter)[0] - 0.5f, (*triangleCenter)[1] - 0.5f);
    glVertex2d((*triangleCenter)[0], (*triangleCenter)[1] + 0.5f);
    glVertex2d((*triangleCenter)[0] + 0.5f, (*triangleCenter)[1] - 0.5f);
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
  glPushMatrix();
  {
    glColor3f(0.44f, 0.44f, 0.44f);
    glScalef(1.8f, 1.8f, 1.0f);
    drawUnitSquare();
  }
  glPopMatrix();
}

/**
 * Transformieren des Einheitsquadrats und der Einheitslinien in eine Wand.
 */
void drawWall(void)
{
  float linePosX = -0.375f;
  float linePosY = -0.375f;
  glPushMatrix();
  {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawUnitSquare();
  }
  glPopMatrix();

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
}

/**
 * Transformieren des Einheitskreises und der Einheitslinien in eine Bombe.
 */
void drawBomb(void)
{
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
  // Kopf
  glPushMatrix();
  {
    glColor3f(0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.0);
    drawUnitCircle();
  }
  glPopMatrix();
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
}

/**
 * Zeichen-Funktion des Schlangenkoerpers
 */
void drawBody(void)
{
  glPushMatrix();
  {
    glColor3f(0.0f, 1.0f, 0.0f);
    drawUnitSquare();
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
  drawString(0.25f, 0.99f, textColor, "Score: %i", score[0]);
  /* Text ausgegeben */
  drawString(0.5f, 0.99f, textColor, "Highscore: %i", highScore[0]);
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
  drawString(0.05f, 0.49f, textColor, "Steuerung der Schlange mit den Pfeiltasten.");
  drawString(0.05f, 0.59f, textColor, "Es wird immer der Schlangenkopf bewegt.");
  drawString(0.05f, 0.69f, textColor, "F1 : Schaltet zwischen normalem und WireFrame/-Mode");
  drawString(0.05f, 0.79f, textColor, "F2 : Waehle zwischen Full-Frame und Window-Frame");
  drawString(0.05f, 0.89f, textColor, "Beliebige normale Taste druecken um neu zu starten.");
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
  int *current = getCurrentLevel();
  esLevel *level = getLevel(*current);

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
void drawScene(void)
{
  int *score = getScore();

  // Soll nur gezeichnet werden, wenn eine Schlange vorhanden ist.
  if (*score != 0)
  {
    int *currentLevel = getCurrentLevel();

    esFieldType fieldType = FT_FREE;

    List snakeList = getSnakeList();

    // Die Wahl des Levels wird abgespeichert
    if (choice != *currentLevel)
    {
      level = getLevel((*currentLevel));
      choice = *currentLevel;
    }

    // Zeichnen freier Felder
    drawFree();

    //Ablaufen des Level-Arrays
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
            glCallList(g_wallListID);
          }
          glPopMatrix();
          break;
        // Zeichnen der Explosion
        case FT_EXPLODED:
          glPushMatrix();
          {
            glTranslatef(FIELDBORDER + x * QUAD_SIDE, FIELDBORDER + y * QUAD_SIDE, 0.0f);
            glScalef(QUAD_SIDE, QUAD_SIDE, 1.0f);
            glCallList(g_explodeListID);
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

    if (toggle == 0)
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
        calcRandomPosition(toggle);
        toggle++;
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
        calcRandomPosition(toggle);
        toggle = 0;
      }
    }

    // Zeichnen des Schlangenkopfes
    fieldType = (*level)[snakeList->snakePart[1]][(snakeList->snakePart)[0]];

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
            glCallList(g_bodyListID);
          }
          glPopMatrix();
        }
      }

      drawGridLines();

      drawText();
    }
    else
    {
      //Verloren falls das Feld nicht frei war
      *score = 0;
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
  /* DEBUG-Ausgabe */
  // GLSTATE;
}

/**
 * Erzeugen der Display-Listen fuer Waende, Explosionen und Koerperteile
 */
static void
initDisplayLists(void)
{
  /* Zwei Display-Listen anfordern */
  g_wallListID = glGenLists((GLsizei)3);
  if (g_wallListID != 0)
  {
    /* Erste Diplay-Liste */
    glNewList(g_wallListID, GL_COMPILE);
    drawWall();
    glEndList();

    /* Zweite Diplay-Liste */
    g_explodeListID = g_wallListID + 1;
    glNewList(g_explodeListID, GL_COMPILE);
    drawExplosion();
    glEndList();

    /* Dritte Diplay-Liste */
    g_bodyListID = g_wallListID + 2;
    glNewList(g_bodyListID, GL_COMPILE);
    drawBody();
    glEndList();
  }
  else
  {
    CG_ERROR(("Konnte Displaylisten nicht erzeuge\n"));
  }
}

/**
 * löscht alle Displaylisten
 */
void 
deleteDisplayLists(void)
{
  glDeleteLists(g_wallListID, 3);
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
  //
  initDisplayLists();
  
  initSnakeList();
  //
  deleteExplode();

  *scale = QUAD_SIDE / 2;
  
  toggle = 0;

  calcRandomPosition(1);

  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
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
