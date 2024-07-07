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
#include <GL/gl.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
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

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats
 * @param isRacket Angabe, ob es sich um den Schlaeger handelt
 */
void
drawUnitSquare (int isRacket)
{
  /* In der Logik berechnet Position beziehen */
  CGPoint2f *quadCenter = getQuadCenter ();
  CGPoint2f *racket = getRacket ();

  if (isRacket) {
    quadCenter = racket;
  }
  
  /* Festlegung eines Einheitsobjektes für Quadrate */
  glBegin (GL_QUADS);
  {
    glVertex2d ((*quadCenter)[0] + 0.5, (*quadCenter)[1] + 0.5);
    glVertex2d ((*quadCenter)[0] - 0.5, (*quadCenter)[1] + 0.5);
    glVertex2d ((*quadCenter)[0] - 0.5, (*quadCenter)[1] - 0.5);
    glVertex2d ((*quadCenter)[0] + 0.5, (*quadCenter)[1] - 0.5);
  }
  glEnd ();
}

/**
 * Zeichen-Funktion zur Erstellung des Einheitskreis
 */
void
drawUnitCircle (void)
{  
  CGPoint2f *ballCenter = getBallCenter();
  /* Festlegung eines Einheitsobjektes für Kreise */
  glBegin (GL_POLYGON);
  {
    for (double i = 0; i < 2 * M_PI; i += M_PI / 6)
    {
      glVertex2f((*ballCenter)[0] + cos(i), (*ballCenter)[1] + sin(i));
    }
  }
  glEnd ();
}

/**
 * Transformieren des Einheitsquadrats in einen Schlaeger
 */
void
drawRacket (void)
{
	glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0, BORDER_BOTTOM,0.0);
		glScalef(QUAD_WIDTH * 2, QUAD_HEIGHT, 1.0);
		drawUnitSquare(1);
	glPopMatrix();
	 
}

/**
 * Transformieren des Einheitsquadrats in eine Wand
 */
void
drawWall (void)
{
	/* Die obere Wand */
	glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0, BORDER_TOP,0.0);
		glScalef(BORDER_LEFT * -1 + BORDER_RIGHT + QUAD_HEIGHT, QUAD_HEIGHT, 1.0);
		drawUnitSquare(0);
	glPopMatrix();
	/* Die linke Wand */
	glPushMatrix();
		glTranslatef(BORDER_LEFT, 0.0,0.0);
		glScalef(QUAD_HEIGHT, BORDER_LEFT * -1 + BORDER_RIGHT - QUAD_HEIGHT, 1.0);
		drawUnitSquare(0);
	glPopMatrix();
	/* Die rechte Wand */
	glPushMatrix();
		glTranslatef(BORDER_RIGHT, 0.0,0.0);
		glScalef(QUAD_HEIGHT, BORDER_LEFT * -1 + BORDER_RIGHT - QUAD_HEIGHT, 1.0);
		drawUnitSquare(0);
	glPopMatrix();
	 
}

/** 
 * Transformieren des Einheitsquadrats in kleine Bloecke
 */
void
drawBricks (void)
{
  int i = 0, j = 0;

  float *dimensions = getDimensions(0,0);

  for(float x = BORDER_LEFT + QUAD_WIDTH + QUAD_WIDTH/2; 
         x < BORDER_RIGHT - QUAD_WIDTH; 
              x += QUAD_WIDTH + QUAD_WIDTH / (ARRAY_WIDTH)) {
		for(float y = BORDER_RIGHT - QUAD_WIDTH - QUAD_WIDTH/2; y > BORDER_BOTTOM + 2*QUAD_WIDTH; y -= 0.2) {
			dimensions = getDimensions(i,j);

      
      if (dimensions[4] == 0.0){
        dimensions[4] = x;
      }
      if (dimensions[5] == 0.0) {
        dimensions[5] = y;
      }
      /* Die einzelnen Bloecke */
			glPushMatrix();
        if (dimensions[3] == 0.0)
        {
          glColor3f(dimensions[0],dimensions[1],dimensions[2]);
          glTranslatef(x, y, 0.0);
				  glScalef(QUAD_WIDTH, QUAD_HEIGHT, 1.0);
				  drawUnitSquare(0);
        }
			glPopMatrix();
      j++;
    }
    j = 0;
    i++;
	}
}

/**
 * Zeichen-Funktion.
 * Transformieren des Einheitsquadrats in einen Kreis.
 */
void
drawCircle ()
{
  glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(0.0, BORDER_BOTTOM + QUAD_HEIGHT/4 + QUAD_HEIGHT/2 , 0.0);
    glScalef(QUAD_HEIGHT / 4, QUAD_HEIGHT / 4,0.0);
    drawUnitCircle();
  glPopMatrix();
}

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar mit Waenden, Bloecken, Schlaeger und Ball.
 */
void
drawScene (void)
{
  /* Zeichnen der Waende */
  drawWall();
  
  /* Zeichnen des Schlaegers */
  drawRacket();
  
  /* Zeichnen der Bloecke */
  drawBricks();

  /* Zeichnet den Ball */
  drawCircle();
  
}


/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int
initScene (void)
{
  /* Setzen der Farbattribute */
  /* Hintergrundfarbe */
  glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
  /* Array erstellen */
  initBricks();

  calcRandomStartDirection(0.0);
  /* Alles in Ordnung? */
  return (GLGETERROR == GL_NO_ERROR);
}

/**
 * Initialisiert das Array der Bloecke.
 * Setzt die jeweilige Farbe und den Wert ob der Block bereits getroffen wurde.
 */
void
initBricks (void)
{
  float color1 = 0.0f;
  float color2 = 0.0f;
  float color3 = 0.0f;
  float *dimensions = getDimensions(0,0);

  /* Intialisiert den Zufallsnummerngenerator */
  srand((unsigned) time(0));

	for(long unsigned int x = 0; x < ARRAY_WIDTH; x++) {
		for(long unsigned int y = 0; y < ARRAY_LENGTH; y++) {
      
      color1 = ((float) (rand() % 100)) / 100;
      color2 = ((float) (rand() % 100)) / 100;
      color3 = ((float) (rand() % 100)) / 100;

      dimensions = getDimensions(x,y);

      //Abfangen von zu dunklen Bloecken
      if (color1 < 0.2)
      {
        color1 = 1.0 - color1;
      }
      if (color2 < 0.2)
      {
        color2 = 1.0 - color2;
      }
      if (color3 < 0.2)
      {
        color3 = 1.0 - color3;
      }

      dimensions[0] = color1;
      dimensions[1] = color2;
      dimensions[2] = color3;
      dimensions[3] = 0.0;
		}
	}
}


/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void
toggleWireframeMode (void)
{
  /* Flag: Wireframe: ja/nein */
  static GLboolean wireframe = GL_FALSE;

  /* Modus wechseln */
  wireframe = !wireframe;

  if (wireframe)
    {
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
    }
  else
    {
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    }
}
