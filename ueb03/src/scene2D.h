#ifndef __SCENE2D_H__
#define __SCENE2D_H__
/**
 * @file
 * Schnittstelle des Darstellungs-Moduls der 2D-Szene.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats
 */
void drawUnitSquare(int drawNorm);

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar mit Waenden, Bloecken, Schlange, Essen, Bombe und Explosionen.
 */
void drawScene2D (void);

/**
 * Zeichen-Funktion zur Erstellung einer Einheitslinie.
 */
void drawUnitLine(void);

/**
 * Transformieren des Einheitsquadrats und der Einheitslinien in eine Wand.
 */
void drawWall();

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene (void);

/**
 * löscht alle Displaylisten
 */
void deleteDisplayLists(void);

/*
 * Zeichenfunktion der Explosion
 * mit 8 skalierten Einheitskreisen
 */
void drawExplosion(void);

/**
 * Zeichen-Funktion des Schlangenkoerpers
 */
void drawBody(void);

/**
 * Setzt alle Explosionsfelder wieder zurueck auf freie Felder
 */
void deleteExplode(void);

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode (void);


#endif
