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
#include "levels.h"
#include "snake.h"

/**
 * Initialisiert die Pause
*/
void
initPause(void);

/**
 * Initialisiert die Hilfeausgabe
 */
void 
initHelp(void);

/**
 * Ueberprueft ob die Schlange getroffen wurde und reduziert die Liste bei einem Treffer
 * @param y die y-Koordinate vom Treffer
 * @param x die x-Koordinate vom Treffer
 * @return wurde getroffen oder nicht
 */
int
damagedSnake(unsigned int y, unsigned int x);

/**
 * Berechnet die neue Position des Schlangenkopfs.
 */
void calcPosition(void);

/**
 * Berechnet die Skalierung des Essens.
 * @param times die Skalierung ueber Zeit
 */
void calcScaleFood(double times);

/**
 * Berechnet die Skalierung der Bombe.
 * @param times die Skalierung ueber Zeit
 */
void calcScaleBomb(double times);

/**
 * Setzt den Bewegunsstatus der Schlange.
 * @param direction Bewegungsrichtung deren Status veraendert werden soll.
 * @param status neuer Status der Bewegung: GL_TRUE->Schlange bewegt sich in Richtung
 * direction, GL_FALSE->Schlange bewegt sich nicht in Richtung direction.
 */
void setMovement(CGDirection direction, GLboolean status);

/**
 * Initialisiert die Liste, die die Schlange abbilden soll.
 */
void initSnakeList(void);

/**
 * Bestimmt an Hand der rand() Operation die neue Position des Foods bzw. der Bomb
 * @param toggle bestimmt ob es sich um Food oder Bomb handelt
 */
void calcRandomPosition(int toggle);


void deleteSnakeList(void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) aller Waende.
 * @return Postion (des Mittelpunktes) der Waende.
 */
CGPoint2f *getQuadCenter(void);

/**
 * Liefert aktuelle Postion im Level-Array des Schlangenkopfes.
 * @return Postion des Schlangenkopfes.
 */
CGPoint3i *getSnakeHeadCenter(void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) der Linie.
 * @return Postion (des Mittelpunktes) der Linie.
 */
CGPoint2f *getLineCenter(void);

/**
 * Liefert aktuelle Postion (des Mittelpunktes) des Dreiecks.
 * @return Postion (des Mittelpunktes) des Dreiecks.
 */
CGPoint2f *getTriangleCenter(void);

/**
 * Liefert aktuelle Postion des Foods im Level-Array.
 * @return Postion des Foods.
 */
CGPoint2i *getFoodCoord(void);

/**
 * Liefert aktuelle Postion im Level-Array der Bomb.
 * @return Postion der Bomb.
 */
CGPoint2i *getBombCoord(void);

/**
 * Gibt den Counter fuer Pause zurueck.
 */
int *getPause(void);

/**
 * Liefert die Skalierung fuer das Essen und die Bombe
 */
float *getScale(void);

/**
 * Liefert die Liste der Schlange
 */
List getSnakeList(void);

/**
 * Gibt die derzeitige Anzahl an Koerperteilen zurück.
 */
int *getScore(void);

/**
 * Liefert den temporaeren Hilfsscore waehrend der Hilfeausgabe 
 */
int *getHelpScore(void);

/**
 * Gibt den HighScore an Koerperteilen zurück.
 */
int *getHighScore(void);

/**
 * Liefert, den Index des aktuellen Levels.
 * @return den Index des aktuellen Levels.
 */
int *getCurrentLevel(void);

#endif
