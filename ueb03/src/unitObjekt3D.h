#ifndef __UNITOBJEKT3D_H__
#define __UNITOBJEKT3D_H__

/**
 * @file
 * Einheitsobjekt-Modul.
 * Das Modul kapselt die Listenlogik. 
 * Dieses Modul beinhaltet die Operationen zur Verwaltung der Schlangenliste.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- Header einbinden ---- */
#include "types.h"
#include "logic.h"

/**
 * Zeichen-Funktion zur Erstellung einer Einheitslinie.
 */
void drawUnitNormalLine(float x, float y, float z);

/**
 * Zeichen-Funktion zur Erstellung des Einheitsquadrats in 3D
 */
void drawUnitSquare3D(int drawNorm, int subDivs);

/**
 * Zeichen-Funktion zur Erstellung der Einheitskugel.
 */
void drawUnitSphere(void);

/**
 * Zeichen-Funktion fuer die gleichschenkligen Einheitsdreiecke der Pyramide
 */
void drawUnitPyramidTriangle(void);

/**
 * Zeichenfunktion zur Erstellung der Einheitspyramide
 */
void drawUnitPyramid(void);

/**
 * Zeichenfunktion der Pyramide, die um die x-Achse rotiert wurde (270 Grad)
 */
void drawRotatePyramid(void);

/**
 * Zeichenfunktion eines Einheits-Tetraeders mit Radius 1
 */
void drawUnitTetrahedron(void);

/**
 * Zeichnet ein invertiertes Tetraeder
*/
void drawUnitTetrahedronInverse(void);

#endif