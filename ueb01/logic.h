#ifndef __LOGIC_H__
#define __LOGIC_H__

/**
 * @file
 * Schnittstelle des Logik-Moduls.
 * Das Modul kapselt die Programmlogik. Wesentliche Programmlogik ist die
 * Verwaltung und Aktualisierung der Position und Bewegung des Schlaegers und des Balls.
 * Die Programmlogik ist weitgehend unabhaengig von Ein-/Ausgabe (io.h/c) und
 * Darstellung (scene.h/c).
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- Header einbinden ---- */
#include "types.h"

/**
 * Berechnet neue Position des Schlaegers.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcPositionRacket (double interval);

/**
 * Berechnet neue Position des Balls.
 * @param interval Dauer der Bewegung in Sekunden.
 */
void calcPositionBall (double interval);

/**
 * Setzt den Bewegunsstatus des Schlaeger.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Schlaeger bewegt sich in Richtung
 * direction, GL_FALSE->Schlaeger bewegt sich nicht in Richtung direction.
 */
void setMovement (CGDirection direction, GLboolean status);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) aller Bloecke und Waende.
 * @return Postion (des Mittelpunktes) der Bloecke und Waende.
 */
CGPoint2f *getQuadCenter (void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Schlaegers.
 * @return Postion (des Mittelpunktes) des Schlaegers.
 */
CGPoint2f *getRacket (void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Balls.
 * @return Postion (des Mittelpunktes) des Balls.
 */
CGPoint2f *getBallCenter (void);

/**
 * Liefert Zeiger auf die aktuelle Farbe eines Eckpunktes des Rechtecks.
 * @param vertex Eckpunkt, dessen Farbe geliefert werden soll
 * @return Zeiger auf die aktuelle Farbe eines Eckpunktes des Rechtecks.
 */
CGColor3f *
getColor (int vertex);


/**
 * Gibt die Dimensionen eines Bricks wieder.
 * @param x die x-Koordinate.
 * @param y die y-Koordinate.
*/
float *
getDimensions (int x, int y);

/**
 * Gibt den Bewegungsvektor zurück.
*/
CGPoint2f *
getBallSpeed(void);

/**
 * Gibt die Anzahl an Leben zurück.
*/
int *
getLifeCount (void);

/**
 * Setzt die Bewegung des Balls.
 * @param angle Der Winkel, in dem der Ball startet.
 */
void
calcRandomStartDirection(float angle);

/**
 * Gibt die Anzahl an Punkten zurück.
*/
int *
getPointCount (void);
#endif
