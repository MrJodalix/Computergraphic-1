#ifndef __SCENE3D_H__
#define __SCENE3D_H__
/**
 * @file
 * Schnittstelle des Darstellungs-Moduls der 3D-Szene.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/**
 *  Zeichnet die Waende in 3D als Wuerfel
 */
void drawWall3D(int drawNorm);

/**
 * Zeichnet die Explosion in 3D als mehrere bunte Kugeln
 */
void drawExplosion3D(void);

/**
 * Zeichnet den Koerper der Schlange als Wuerfel
 */
void drawSnakeCube(int drawNorm);

/**
 * Hilfsfunktion die die Position für die Animation festlegt
 * @param snakePartX die X-Koordinate des Schlangenkopfes
 * @param snakePartZ die Z-Koordinate des Schlangenkopfes
 * @param snakeDirect die Richtung in die die schlange wandert
 * @return der veraenderte Wert
 */
float getNewPos(int snakePartX, int snakePartZ, int snakeDirect);

/**
 * Zeichen-Funktion der 3D-Szene.
 * Stellt die Waende, die Schlange, die Bombe und die Nahrung dar sowie die
 * Explosionen dar.
 */
void drawScene3D(void);

/**
 * Initialisierung der Lichtquellen.
 * Setzt Eigenschaften der Lichtquellen (Farbe, Oeffnungswinkel, ...)
 */
void
initLight(void);

/**
 * Zeichen-Funktion zur Erstellung einer Einheitslinie.
 */
void drawUnitNormalLine(float x, float y, float z);

/**
 * (De-)aktiviert den Normalen-Modus.
 */
void
toggleNormals(void);

/**
 * (De-)aktiviert das Spotlight.
 */
void
toggleSpotlight(void);

/**
 * (De-)aktiviert die globale Beleuchtung.
 */
void
toggleMainLight(void);

/**
 * (De-)aktiviert den Animations-Modus.
 */
void toggleAnimation(void);
#endif