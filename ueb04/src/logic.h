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

/* ---- Eigene Header einbinden ---- */
#include "types.h"


/**
 * Initialisiert die dynamischen Arrays
 */
void initVertices(void);

/**
 * Leert alle Arrays und setzt alle dynamischen Arrays auf NULL
 */
void freeAll(void);


/**
 * Erstellt die Normalen der Vertex-Koordinaten
 * @param count der Index des Normal-Arrays
 * @param x die X-Koordinate des Meshs
 * @param y die Y-Koordinate des Meshs
 * @param z die Z-Koordinate des Meshs
 */
void calcNormalCoords(int count, double x, double y, double z);

/**
 * Führt die Berechnungen der Mesh-Vertices durch und setzt die Indeces
 */
void setWaterPlane(void);

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
 * Gibt den Counter fuer Pause zurueck.
 */
int *
getPause(void);

/**
 * Getter für die Normalen.
 */
int *
getDrawNorm(void);

/**
 * Gibt die Anzahl aller Vertices zurueck
 */
int *
getNumberOfVertices(void);

/**
 * gibt den Toggle zu malen der Kugeln zurueck
 */
int *
getDrawWaterSpheres(void);

/**
 * gibt den Toggle zu malen der Hilfeausgabe zurueck
 */
int *
getDrawHelp(void);

#endif
