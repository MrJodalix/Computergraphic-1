/**
 * @file
 * Hauptprogramm. Initialisierung und Starten der Ergeignisverarbeitung.
 *
 * Interaktion
 * -----------
 * - Bewegung des Schlaegers durch die Pfeiltasten.
 * - Umschalten zwischen Vollbild- und Fensterdarstellung mit Taste F1.
 * - Beenden des Programms mit Taste ESC oder q/Q
 *
 * @author Joshua-Scott Schoettke, Ilana Schmara (Gruppe 3).
 */

/* ---- System Header einbinden ---- */
#include <stdio.h>

/* ---- Eigene Header einbinden ---- */
#include "io.h"

/**
 * Hauptprogramm.
 * Initialisierung und Starten der Ereignisbehandlung.
 * @param argc Anzahl der Kommandozeilenparameter (In).
 * @param argv Kommandozeilenparameter (In).
 * @return Rueckgabewert im Fehlerfall ungleich Null.
 */
int
main (int argc, char **argv)
{
	(void)argc;
	(void)argv;
  /* Initialisierung des I/O-Sytems
     (inkl. Erzeugung des Fensters und Starten der Ereignisbehandlung). */
  if (!initAndStartIO
      ("Mein erstes OpenGL-Programm mit Tastatursteuerung", 500, 500))
    {
      fprintf (stderr, "Initialisierung fehlgeschlagen!\n");
      return 1;
    }
  else
    {
      return 0;
    }
}
