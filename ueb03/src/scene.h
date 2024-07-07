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

#include "levels.h"

/**
 * Gibt das Level zurueck, welches ausgewaehlt wurde.
 * @return das ausgewaehlte Level
 */
esLevel *switchLevel(void);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrundfarbe.
 * Initialisiert die Displaylisten, die Schlangenliste.
 * loescht Explosionen im Level, falls noch welche vorhanden sind.
 * scale und toggle werden auf ihren Startwert zurueck gesetzt.
 * Die Position fuer das Food wird initialisiert.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene (void);

/**
 * Gibt den Toggle fuer Nahrung oder Bombe zurueck.
 * @return der Toggle
 */
int *getToggle(void);

/**
 * Gibt die DisplayListe zurueck
 * @param idx der Index der Liste
 * @return die Displayliste an dem Index
*/
GLuint *getDisplayList(int idx);

#endif