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
 * Malt alle Kugeln an den Positionen der Vertices
 */
void drawWaterSpheres(void);

/**
 * Malt die ganze Szene
 */
void drawScene(void);

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrundfarbe.
 * Initialisiert die Displaylisten, die Schlangenliste.
 * loescht Explosionen im Level, falls noch welche vorhanden sind.
 * scale und toggle werden auf ihren Startwert zurueck gesetzt.
 * Die Position fuer das Food wird initialisiert.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int initScene(void);

/**
 * Gibt die DisplayListe zurueck
 * @param idx der Index der Liste
 * @return die Displayliste an dem Index
 */
GLuint *getDisplayList();

/**
 * (De-)aktiviert den Wireframe-Modus.
 */
void toggleWireframeMode(void);

/**
 * (De-)aktiviert den Normalen-Modus.
 */
void toggleNormals(void);

/**
 * (De-)aktiviert den Kugel-Modus.
 */
void toggleDrawSphere(void);

/**
 * Gibt die DisplayListe zurueck
 * @return die Displayliste
 */
GLuint *
getDisplayList(void);

#endif