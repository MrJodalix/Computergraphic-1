#ifndef __SCENE_H__
#define __SCENE_H__
/**
 * @file
 * Schnittstelle des Darstellungs-Moduls.
 * Das Modul kapselt die Rendering-Funktionalitaet (insbesondere der OpenGL-
 * Aufrufe) des Programms.
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/**
 * Zeichen-Funktion.
 * Stellt die Szene dar mit Waenden, Bloecken, Schlaeger und Ball.
 */
void drawScene (void);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene (void);

void
initBricks (void);

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode (void);

#endif
